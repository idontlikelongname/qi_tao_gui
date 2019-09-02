#include <iostream>

#include <QDebug>

#include "qitaojiemian.h"
#include "ui_qitaojiemian.h"

QTMainWindow::QTMainWindow(QJsonObject *bom_data, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QTMainWindow), bom_json_info_(bom_data) {
  parent_dlg = parent;
  ui->setupUi(this);

  cur_selected_ruku_index_ = QModelIndex();

  Init();
}

QTMainWindow::~QTMainWindow() { delete ui; }

void QTMainWindow::Init() {
  //设置自体大小
  QFont font1("Microsoft YaHei", 36, 50, false);
  QFont font2("Microsoft YaHei", 32, 50, false);
  QFont font3("Microsoft YaHei", 28, 50, false);
  QFont font4("Microsoft YaHei", 24, 50, false);

  ui->menubar->setFont(font1);
  ui->toolBar->setFont(font2);
  // ui->toolBar->setIconSize(QSize(50,50));
  // ui->toolBar->setFixedSize(150,150);

  ui->label_3->setFont(font3);
  ui->label_4->setFont(font3);
  ui->lineEdit->setFont(font3);
  ui->searchButton->setFont(font3);
  ui->qingchuButton->setFont(font3);

  ui->rukuButton->setFont(font3);
  ui->rukuButton->setFixedSize(300, 70);

  ui->treeView->setFont(font4);

  ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  this->setWindowState(Qt::WindowMaximized);

  InitTreeView();
  connect(ui->treeView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)), this,
          SLOT(currentChangedShot(const QModelIndex &, const QModelIndex &)));

  UpdateTreeView();

  connect(ui->actExit, SIGNAL(triggered()), this, SLOT(on_actExit_triggered()));
  connect(ui->rukuButton, SIGNAL(clicked()), this, SLOT(on_ruku_clicked()));
  connect(ui->lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(on_search_changed(const QString &)));
  connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(on_search_clicked()));

  ruku_ = new RuKu(bom_json_info_, this);
}

void QTMainWindow::SearchStandardByID(QString search_info) {
  // setExpanded(const QModelIndex &index, bool expanded);

  for (int row = 0; row < bom_model_->rowCount(); ++row) {
    //
    QStandardItem *part_item = bom_model_->item(row, 0);
    QModelIndex part_index = bom_model_->indexFromItem(part_item);
    QModelIndex std_index = QModelIndex();
    bool found_std_under_this_part = false;
    for (int std_idx = 0; std_idx < part_item->rowCount(); ++std_idx) {
      QStandardItem *std_item = part_item->child(std_idx, 0);
      QString std_name_text = std_item->data(Qt::DisplayRole).toString();
      // 找到零件
      if (std_name_text == search_info) {
        found_std_under_this_part = true;
        std_index = bom_model_->indexFromItem(std_item);
      }
    }
    // 展开零件
    if (found_std_under_this_part) {
      ui->treeView->expand(part_index);
    }
  }
}

void QTMainWindow::UpdateTreeView() {
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

void QTMainWindow::InitTreeView() {
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

  ui->treeView->setColumnWidth(0, 500);
  ui->treeView->setColumnWidth(1, 320);
  ui->treeView->setColumnWidth(2, 220);
  ui->treeView->setColumnWidth(3, 200);
  ui->treeView->setColumnWidth(4, 200);
  ui->treeView->setColumnWidth(5, 200);
  ui->treeView->setColumnWidth(6, 200);

  ui->treeView->resize(500, 500);
  ui->treeView->show();
  // ui->treeView->expandAll();
}

void QTMainWindow::currentChangedShot(const QModelIndex &selected,
                                      const QModelIndex &deselected) {
  // reset当前选定的标准件index
  cur_selected_ruku_index_ = QModelIndex();

  QModelIndex index = ui->treeView->selectionModel()->currentIndex();
  // 如果当前item不存在child节点，说明当前节点为零件节点
  QStandardItem *cur_item = bom_model_->itemFromIndex(index);
  if (cur_item->hasChildren()) {
    qDebug() << "不是标准件分支";
    return;
  }
  // 获取当前零件的所有信息
  // 以及字典中的key
  // value，包括<部件名称> ，零件名称，方便后续检索以及状态更新
  QModelIndex std_item = index.sibling(index.row(), 0);

  // 获取当前出库入库状态
  QModelIndex ruku_index = index.sibling(index.row(), 6);
  QModelIndex chuku_index = index.sibling(index.row(), 7);
  bool is_chuku =
      (chuku_index.data(Qt::DisplayRole).toString() == QString("是"));
  bool is_ruku = (ruku_index.data(Qt::DisplayRole).toString() == QString("是"));

  // for DEBUG
  cur_selected_standard_info_.std_name =
      std_item.data(Qt::DisplayRole).toString();
  cur_selected_standard_info_.part_name =
      std_item.parent()
          .sibling(std_item.parent().row(), 0)
          .data(Qt::DisplayRole)
          .toString();
  cur_selected_standard_info_.weight =
      std_item.sibling(std_item.row(), 3).data(Qt::DisplayRole).toDouble();
  cur_selected_standard_info_.count =
      std_item.sibling(std_item.row(), 4).data(Qt::DisplayRole).toInt();
  qDebug() << cur_selected_standard_info_.std_name << ","
           << cur_selected_standard_info_.part_name << ","
           << cur_selected_standard_info_.weight << ","
           << cur_selected_standard_info_.count;

  // std_info.append("  出库状态 ");
  // if (is_chuku) {
  //   std_info.append("[是]");
  // } else {
  //   std_info.append("[否]");
  // }
  // std_info.append("  入库状态 ");
  // if (is_ruku) {
  //   std_info.append("[是]");
  // } else {
  //   std_info.append("[否]");
  // }

  // QStandardItem *cur_item = bom_model_->itemFromIndex(index);
  // cur_item->setText(QString("点击"));

  // 获取当前标准件信息

  // 当前标准件还未入库
  if (!is_chuku && !is_ruku) {
    cur_selected_ruku_index_ = std_item;
  }
}

void QTMainWindow::on_ruku_clicked() {
  //
  // 如果当前未选中有效的index
  if (QModelIndex() == cur_selected_ruku_index_) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("单发齐套柜");
    msgBox.setText("入库前，请选中有效的标准件");
    // msgBox.setInformativeText("Do you want to start a new deck?");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setStyleSheet(
        "QLabel{width:300 px; font-size: 30px;} "
        "QPushButton{ width:130 px; font-size: 35px;}");
    int r = msgBox.exec();
    return;
  }

  // 读取bom表的数据
  QJsonObject bom_list = (*bom_json_info_)["信息"].toObject();
  qDebug() << "select locker";
  // 查找当前所有空柜子，随机分配一个柜子
  int locker_ids[24] = {0,  1,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
                        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
  std::map<int, bool> locker_state_map;
  for (QJsonObject::Iterator it = bom_list.begin(); it != bom_list.end();
       it++) {
    QJsonObject parts = it.value().toObject();
    for (QJsonObject::Iterator std_it = parts.begin(); std_it != parts.end();
         std_it++) {
      QJsonObject standard = std_it.value().toObject();
      if (standard.contains("仓库编号")) {
        int locker_id = standard["仓库编号"].toInt();
        if (locker_id >= 0) {
          locker_state_map[locker_id] = true;
        }
      }
    }
  }
  int selected_locker_id = -1;
  for (int l_id = 0; l_id < 24; ++l_id) {
    int locker_id = locker_ids[l_id];
    if (locker_state_map.count(locker_id) <= 0) {
      // 找到第一个空闲的柜子
      selected_locker_id = locker_id;
    }
  }
  qDebug() << "select locker end:" << selected_locker_id;

  // TODO: 弹出入库窗口，进行入库操作
  ruku_->setWindowModality(Qt::ApplicationModal);
  Qt::WindowFlags flags = Qt::Dialog;
  // flags |= Qt::WindowMaximizeButtonHint;
  flags |= Qt::WindowMinimizeButtonHint;
  ruku_->setWindowFlags(flags);
  // 入库的柜门编号，入库的标准件重量
  ruku_->Display(cur_selected_standard_info_);
  ruku_->exec();

  // 最后更新当前列表
  UpdateTreeView();
}

void QTMainWindow::on_actExit_triggered() {
  // show parent dialog
  parent_dlg->show();
  // hide current dialog
  this->hide();
}

void QTMainWindow::on_search_clicked() {
  // 收起所有treeview
  // 首先收回所有expand
  ui->treeView->collapseAll();
  // 读取搜索框中的内容
  QString search_info = ui->lineEdit->text();
  if (!search_info.isEmpty()) {
    SearchStandardByID(search_info);
    // 清空搜索框
    ui->lineEdit->setText("");
  }
}

void QTMainWindow::on_search_changed(const QString &text) {
  QMap<QString, QString> STD_NAME_MAP;
  STD_NAME_MAP["dianquan_4"] = "垫圈4";
  STD_NAME_MAP["luoding_M_4_10"] = "螺钉M4*10";
  STD_NAME_MAP["luoding_M_4_22"] = "螺钉M4*22";
  STD_NAME_MAP["luoding_M_6_12"] = "螺钉M6*12";

  if (STD_NAME_MAP.contains(text)) {
    ui->lineEdit->setText(STD_NAME_MAP[text]);
  }
}

void QTMainWindow::on_clear_clicked() { ui->lineEdit->setText(""); }

void QTMainWindow::paintEvent(QPaintEvent *event) {
  //绘制窗口背景图片
  Q_UNUSED(event);
  QPainter painter(this);

  painter.drawPixmap(
      0, ui->toolBar->height(), this->width(),
      this->height() - ui->toolBar->height() - ui->statusbar->height(),
      QPixmap(":/images/images/back2.jpg"));
}
