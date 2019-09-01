#include "chukujiemian.h"
#include "ui_chukujiemian.h"

ChKMainWindow::ChKMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChKMainWindow) {
  parent_dlg = parent;

  ui->setupUi(this);
  //设置自体大小
  QFont font1("Microsoft YaHei", 26, 50, false);
  QFont font2("Microsoft YaHei", 18, 50, false);
  QFont font3("Microsoft YaHei", 18, 50, false);
  QFont font4("Microsoft YaHei", 14, 50, false);
  ui->menubar->setFont(font1);
  ui->toolBar->setFont(font2);
  ui->label->setFont(font3);
  ui->label_2->setFont(font3);
  ui->lineEdit->setFont(font3);
  ui->lineEdit->setFixedSize(600, 70);
  ui->pushButton->setFont(font3);
  ui->pushButton_2->setFont(font3);
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

  ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  this->setWindowState(Qt::WindowMaximized);

  connect(ui->actExit, SIGNAL(triggered()), this, SLOT(on_actExit_triggered()));
}

ChKMainWindow::~ChKMainWindow() { delete ui; }

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
