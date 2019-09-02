#include <QLineEdit>
#include <QVBoxLayout>
#include <QtWidgets>

#include "dialog.h"
#include "wigglywidget.h"

//! [0]
Dialog::Dialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("单发齐套柜"));
  Init();
  this->showMaximized();
}
Dialog::~Dialog() {
  saveJsonFile(bom_json_info_,
               QString("/home/xiaoxiao/Qt/qi_tao_gui/resources/bom.json"));
}
void Dialog::Init() {
  // show text
  WigglyWidget *wigglyWidget = new WigglyWidget;
  wigglyWidget->setText(tr("航天新风_多功能单发齐套柜"));
  QFont font1("Microsoft YaHei", 46, 50, false);
  wigglyWidget->setFont(font1);

  qitaobutton = new QPushButton(tr("齐套"));
  qitaobutton->setFixedHeight(90);
  qitaobutton->setFixedWidth(250);

  chukubutton = new QPushButton(tr("出库"));
  // chukubutton->setIcon();
  chukubutton->setFixedHeight(90);
  chukubutton->setFixedWidth(250);

  yilanbutton = new QPushButton(tr("一览表"));
  yilanbutton->setFixedHeight(90);
  yilanbutton->setFixedWidth(250);

  qingchubutton = new QPushButton(tr("清除所有"));
  qingchubutton->setFixedHeight(90);
  qingchubutton->setFixedWidth(250);

  // signal&slot
  connect(qitaobutton, SIGNAL(clicked(bool)), this, SLOT(OnClickQiTao()));
  connect(chukubutton, SIGNAL(clicked(bool)), this, SLOT(OnClickChuKu()));
  connect(yilanbutton, SIGNAL(clicked(bool)), this, SLOT(showyilantable()));
  connect(qingchubutton, SIGNAL(clicked(bool)), this, SLOT(qingchualldata()));

  horizontalGroupBox = new QGroupBox(tr("选择功能"));
  QFont font2("Microsoft YaHei", 24, 50, false);
  horizontalGroupBox->setFont(font2);
  QHBoxLayout *layout_h = new QHBoxLayout;
  layout_h->addStretch();
  layout_h->addWidget(qitaobutton);
  layout_h->addStretch();
  layout_h->addWidget(chukubutton);
  layout_h->addStretch();
  layout_h->addWidget(yilanbutton);
  layout_h->addStretch();
  layout_h->addWidget(qingchubutton);
  layout_h->addStretch();
  layout_h->setContentsMargins(0, this->height() / 2, this->width() / 30,
                               this->height() / 1.1);

  horizontalGroupBox->setLayout(layout_h);

  QVBoxLayout *layout_v = new QVBoxLayout(this);
  layout_v->addWidget(wigglyWidget);
  layout_v->addWidget(horizontalGroupBox);

  // 从json文件中加载bom表信息
  bom_json_info_ = loadJsonFile(
      QString("/home/xiaoxiao/Qt/qi_tao_gui/resources/bom.json"));

  dialog_chuku = new ChKMainWindow(&bom_json_info_,this);
  dialog_qitao = new QTMainWindow(&bom_json_info_, this);
}

void Dialog::OnClickQiTao() {
  // show qitao dialog
  dialog_qitao->setWindowModality(Qt::ApplicationModal);
  dialog_qitao->setWindowState(dialog_qitao->windowState() |
                               Qt::WindowMaximized);
  dialog_qitao->show();
}
void Dialog::OnClickChuKu() {
  // show chuku dialog
  dialog_chuku->setWindowModality(Qt::ApplicationModal);
  dialog_chuku->setWindowState(dialog_chuku->windowState() |
                               Qt::WindowMaximized);
  dialog_chuku->show();
}

void Dialog::showyilantable() {
  //展示整个表格
}

void Dialog::qingchualldata() {
  QMessageBox::StandardButton result;
  //  QMessageBox::information(this, tr("数据清除"),
  //  tr("清除所有数据，进行下发产品齐套？"));
  QMessageBox::question(this, tr("数据清除"),
                        tr("清除所有数据，进行下发产品齐套？"));

  if (result == QMessageBox::Yes) {
    //清除所有数据
  }
}

void Dialog::paintEvent(QPaintEvent *event) {  //绘制窗口背景图片
  Q_UNUSED(event);
  QPainter painter(this);

  //    painter.drawPixmap(this->width()/10,this->height()/1.4,this->width()/1.25,
  //                       this->height()/3,
  //                       QPixmap(":/images/images/kegong.jpg"));

  painter.drawPixmap(0, this->height() / 1.4, this->width(), this->height() / 3,
                     QPixmap(":/images/images/kegong.jpg"));
}
