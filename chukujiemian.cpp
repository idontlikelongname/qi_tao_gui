#include "chukujiemian.h"
#include "ui_chukujiemian.h"

ChKMainWindow::ChKMainWindow(QJsonObject *bom_data, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChKMainWindow), bom_json_info_(bom_data) {
  parent_dlg = parent;

  ui->setupUi(this);

  Init();
}

ChKMainWindow::~ChKMainWindow() { delete ui; }

void ChKMainWindow::Init() {
  //设置自体大小
  //  QFont font1("Microsoft YaHei", 26, 50, false);
  //  QFont font2("Microsoft YaHei", 18, 50, false);
  //  QFont font3("Microsoft YaHei", 18, 50, false);
  //  QFont font4("Microsoft YaHei", 14, 50, false);

  QFont font1("Microsoft YaHei", 36, 50, false);
  QFont font2("Microsoft YaHei", 32, 50, false);
  QFont font3("Microsoft YaHei", 28, 50, false);
  QFont font4("Microsoft YaHei", 24, 50, false);

  ui->menubar->setFont(font1);
  ui->toolBar->setFont(font2);
  // ui->label->setFont(font3);
  ui->label_2->setFont(font3);
  // ui->lineEdit->setFont(font3);
  // ui->lineEdit->setFixedSize(600, 70);
  // ui->pushButton->setFont(font3);
  ui->openButton->setFont(font3);
  ui->pushButton_3->setFont(font4);
  ui->pushButton_4->setFont(font4);
  ui->pushButton_5->setFont(font4);
  ui->pushButton_6->setFont(font4);
  ui->pushButton_7->setFont(font4);
  ui->pushButton_8->setFont(font4);
  ui->pushButton_9->setFont(font4);
  ui->pushButton_10->setFont(font4);
  ui->pushButton_11->setFont(font4);
  ui->pushButton_12->setFont(font4);
  ui->pushButton_13->setFont(font4);
  ui->pushButton_14->setFont(font4);
  ui->pushButton_15->setFont(font4);
  ui->pushButton_16->setFont(font4);
  ui->pushButton_17->setFont(font4);
  ui->pushButton_18->setFont(font4);
  ui->pushButton_19->setFont(font4);
  ui->pushButton_20->setFont(font4);
  ui->pushButton_21->setFont(font4);
  ui->pushButton_22->setFont(font4);
  ui->pushButton_23->setFont(font4);
  ui->pushButton_24->setFont(font4);
  ui->pushButton_25->setFont(font4);
  ui->pushButton_26->setFont(font4);
  ui->treeView->setFont(font4);

  ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  this->setWindowState(Qt::WindowMaximized);

  InitTreeView();

  UpdateTreeView();

  connect(ui->actExit, SIGNAL(triggered()), this, SLOT(on_actExit_triggered()));
  connect(ui->openButton, SIGNAL(clicked()), this,
          SLOT(on_openButton_clicked()));
}

void ChKMainWindow::InitTreeView() {
  // init the part tree
  bom_model_ = new QStandardItemModel();

  // 读取tree table的标签
  QMap<int, QString> info_tile_map;
  QStringList tree_titles;
  json_titles_ = (*bom_json_info_)["标题"].toArray();
  for (int t_id = 0; t_id < json_titles_.size(); ++t_id) {
    info_tile_map[t_id] = json_titles_.at(t_id).toString();
    tree_titles << json_titles_.at(t_id).toString();
  }

  // 设置标签
  bom_model_->setHorizontalHeaderLabels(tree_titles);

  // 读取bom表的数据
  QJsonObject bom_list = (*bom_json_info_)["信息"].toObject();
  QStringList keys = bom_list.keys();

  for (int k_id = 0; k_id < keys.size(); ++k_id) {
    QString part_name = keys.at(k_id);
    QStandardItem *part = new QStandardItem(part_name);

    QJsonObject standard_part_list = bom_list[part_name].toObject();

    QStringList standard_part_keys = standard_part_list.keys();

    for (int s_id = 0; s_id < standard_part_keys.size(); ++s_id) {
      QString standard_part_name = standard_part_keys[s_id];
      QJsonObject standard_part_info =
          standard_part_list[standard_part_name].toObject();

      QList<QStandardItem *> standard_parts;
      standard_parts << new QStandardItem(standard_part_name);
      // 从1开始，不包含“标准件名称”
      // 1. 编号 string
      standard_parts << new QStandardItem(
          standard_part_info[json_titles_.at(1).toString()].toString());
      // 2. 规格 string
      standard_parts << new QStandardItem(
          standard_part_info[json_titles_.at(2).toString()].toString());
      // 3. 单体重量 float
      standard_parts << new QStandardItem(QString::number(
          standard_part_info[json_titles_.at(3).toString()].toDouble()));
      // 4. 数量 int
      standard_parts << new QStandardItem(QString::number(
          standard_part_info[json_titles_.at(4).toString()].toInt()));
      // 5. 总重量 float
      standard_parts << new QStandardItem(QString::number(
          standard_part_info[json_titles_.at(5).toString()].toDouble()));

      // 6. 入库状态 bool
      if (standard_part_info[json_titles_.at(6).toString()].toBool()) {
        standard_parts << new QStandardItem(QString("是"));
      } else {
        standard_parts << new QStandardItem(QString("否"));
      }
      // 7. 出库状态 bool
      if (standard_part_info[json_titles_.at(7).toString()].toBool()) {
        standard_parts << new QStandardItem(QString("是"));
      } else {
        standard_parts << new QStandardItem(QString("否"));
      }

      part->appendRow(standard_parts);
    }

    bom_model_->appendRow(part);
  }

  ui->treeView->setModel(bom_model_);

  ui->treeView->setColumnWidth(0, 450);
  ui->treeView->setColumnWidth(1, 320);
  ui->treeView->setColumnWidth(2, 220);
  ui->treeView->setColumnWidth(3, 200);
  ui->treeView->setColumnWidth(4, 140);
  ui->treeView->setColumnWidth(5, 140);
  ui->treeView->setColumnWidth(6, 150);
  ui->treeView->setColumnWidth(7, 150);

  ui->treeView->resize(500, 500);
  ui->treeView->show();
  // ui->treeView->expandAll();
}

void ChKMainWindow::UpdateTreeView() {
  // 读取bom表的数据
  QJsonObject bom_list = (*bom_json_info_)["信息"].toObject();
  QStringList keys = bom_list.keys();
  for (int row = 0; row < bom_model_->rowCount(); ++row) {
    // 部件层
    QStandardItem *part_item = bom_model_->item(row, 0);
    QString part_name_text = part_item->data(Qt::DisplayRole).toString();
    // qDebug() << part_name_text << " " << part_item->rowCount();
    // 如果当前treeview中的part在json中存在，更新treeview中的信息
    // 肯定包含
    if (bom_list.contains(part_name_text)) {
      //
      QJsonObject standard_part_list = bom_list[part_name_text].toObject();
      for (int std_idx = 0; std_idx < part_item->rowCount(); ++std_idx) {
        QStandardItem *std_item = part_item->child(std_idx, 0);
        QString std_name_text = std_item->data(Qt::DisplayRole).toString();
        // qDebug() << std_name_text;
        QJsonObject std_info = standard_part_list[std_name_text].toObject();
        // 1. 编号 string
        part_item->child(std_idx, 1)
            ->setText(std_info[json_titles_.at(1).toString()].toString());
        // 2. 规格 string
        part_item->child(std_idx, 2)
            ->setText(std_info[json_titles_.at(2).toString()].toString());
        // 3. 单体重量 float
        part_item->child(std_idx, 3)
            ->setText(QString::number(
                std_info[json_titles_.at(3).toString()].toDouble()));
        // 4. 数量 int
        part_item->child(std_idx, 4)
            ->setText(QString::number(
                std_info[json_titles_.at(4).toString()].toInt()));
        // 5. 总重量 float
        part_item->child(std_idx, 5)
            ->setText(QString::number(
                std_info[json_titles_.at(5).toString()].toDouble()));
        // 6. 入库状态 bool
        if (std_info[json_titles_.at(6).toString()].toBool()) {
          part_item->child(std_idx, 6)->setText(QString("是"));
        } else {
          part_item->child(std_idx, 6)->setText(QString("否"));
        }
        // 7. 出库状态 bool
        if (std_info[json_titles_.at(7).toString()].toBool()) {
          part_item->child(std_idx, 7)->setText(QString("是"));
        } else {
          part_item->child(std_idx, 7)->setText(QString("否"));
        }
      }
    }
  }
}

void ChKMainWindow::on_actExit_triggered() {
  // show parent dialog
  parent_dlg->show();
  // hide current dialog
  this->hide();
}

void ChKMainWindow::paintEvent(QPaintEvent *event) {  //绘制窗口背景图片
  Q_UNUSED(event);
  QPainter painter(this);

  //    painter.drawPixmap(0,ui->toolBar->height(),this->width(),
  //                       this->height()-ui->toolBar->height()-ui->statusbar->height(),
  //                       QPixmap(":/images/images/back2.jpg"));

  painter.drawPixmap(
      0, ui->toolBar->height(), this->width(),
      this->height() - ui->toolBar->height() - ui->statusbar->height(),
      QPixmap(":/images/images/qingxin1.jpeg"));
}

void ChKMainWindow::on_openButton_clicked() {}
