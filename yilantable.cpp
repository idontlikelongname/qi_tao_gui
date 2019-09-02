#include "yilantable.h"
#include "ui_yilantable.h"

yilantable::yilantable(QJsonObject *bom_data,QWidget *parent) :
    QMainWindow(parent),bom_json_info_(bom_data),
    ui(new Ui::yilantable)
{
    parent_dlg = parent;

    ui->setupUi(this);

    Init();
}



yilantable::~yilantable()
{
    delete ui;
}


void yilantable::Init() {

    //设置自体大小
  //  QFont font1("Microsoft YaHei", 26, 50, false);
  //  QFont font2("Microsoft YaHei", 18, 50, false);
  //  QFont font3("Microsoft YaHei", 18, 50, false);
  //  QFont font4("Microsoft YaHei", 14, 50, false);

    QFont font1("Microsoft YaHei", 36, 50, false);
    //QFont font2("Microsoft YaHei", 32, 50, false);
    QFont font3("Microsoft YaHei", 28, 50, false);
    QFont font4("Microsoft YaHei", 24, 50, false);


    ui->menubar->setFont(font1);
    ui->lineEdit->setFont(font3);
    ui->lineEdit->setFixedSize(500,70);

    ui->exitButton->setFont(font3);
    ui->exitButton->setFixedSize(300,70);

    ui->treeView->setFont(font4);


//    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->setWindowState(Qt::WindowMaximized);

    InitTreeView();

    UpdateTreeView();

    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitButton_triggered()));


}


void yilantable::InitTreeView(){
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
    ui->treeView->expandAll();

}


void yilantable::UpdateTreeView(){
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

void yilantable::on_exitButton_triggered() {
    // show parent dialog
    parent_dlg->show();
    // hide current dialog
    this->hide();

}

void yilantable::paintEvent(QPaintEvent *event) {  //绘制窗口背景图片
  Q_UNUSED(event);
  QPainter painter(this);

  //    painter.drawPixmap(0,ui->toolBar->height(),this->width(),
  //                       this->height()-ui->toolBar->height()-ui->statusbar->height(),
  //                       QPixmap(":/images/images/back2.jpg"));

  painter.drawPixmap(
      0, ui->statusbar->height(), this->width(),
      this->height() - ui->statusbar->height(),
      QPixmap(":/images/images/scenery.jpeg"));
}
