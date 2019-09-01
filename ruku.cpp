#include <QDebug>
#include <iostream>

#include "ruku.h"
#include "ui_ruku.h"

RuKu::RuKu(QWidget *parent)
    : QDialog(parent), ui(new Ui::RuKuDialog), camera_cap_(NULL) {
  // 禁止调整窗口大小 移动窗口
  // this->setWindowFlags(this->windowFlags() | (Qt::WindowMaximizeButtonHint |
  //                                             Qt::WindowMinimizeButtonHint |
  //                                             Qt::WindowCloseButtonHint));
  // this->setFixedSize(this->width(), this->height());
  ui->setupUi(this);
  Init();
}

RuKu::~RuKu() { delete ui; }

void RuKu::Init() {
  loop_timer_ = new QTimer(this);
  loop_timer_->setInterval(100);
  connect(loop_timer_, SIGNAL(timeout()), this, SLOT(loopOnce()));

  connect(ui->captureButton, SIGNAL(clicked()), this,
          SLOT(captureImage_trigger()));
  connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finished_trigger()));
  connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(exit_trigger()));
}

void RuKu::Display() {
  // TODO: try catch
  OpenCamera();
  OpenSerial();

  loop_timer_->start();

  // reset the state
  if (!record_image_.empty()) {
    record_image_.release();
  }
}

void RuKu::OpenCamera() {
  //
  cur_image_ = cv::imread("/home/yangxx/codes/Qt/qi_tao_gui/images/back2.jpg");
  camera_cap_ = new cv::VideoCapture("/dev/video1");
}

void RuKu::CloseCamera() {
  camera_cap_->release();
  camera_cap_ = NULL;
}

void RuKu::OpenSerial() {
  // qDebug() << "Open Serial";
  // foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
  //   qDebug() << "Name:" << info.portName();
  //   qDebug() << "Description:" << info.description();
  //   qDebug() << "SerialNumber:" << info.serialNumber();
  //   qDebug() << "Product Identifier:" << info.productIdentifier();
  // }

  // 打开台秤串口
  connect(&serial_scale_, SIGNAL(readyRead()), this,
          SLOT(scale_readyReadSlot()));
  serial_scale_.setPortName("scale_port");
  serial_scale_.setBaudRate(QSerialPort::Baud9600);
  serial_scale_.setParity(QSerialPort::NoParity);
  serial_scale_.setDataBits(QSerialPort::Data8);
  serial_scale_.setStopBits(QSerialPort::OneStop);
  serial_scale_.setFlowControl(QSerialPort::NoFlowControl);
  if (serial_scale_.open(QIODevice::ReadWrite)) {
    qDebug() << "Open Scale Serial Success";
    serial_scale_.clearError();
    serial_scale_.clear();
  }

  // 打开柜门串口
  connect(&serial_locker_, SIGNAL(readyRead()), this,
          SLOT(locker_readyReadSlot()));
  serial_locker_.setPortName("locker_port");
  serial_locker_.setBaudRate(QSerialPort::Baud19200);
  serial_locker_.setParity(QSerialPort::NoParity);
  serial_locker_.setDataBits(QSerialPort::Data8);
  serial_locker_.setStopBits(QSerialPort::OneStop);
  serial_locker_.setFlowControl(QSerialPort::NoFlowControl);
  if (serial_locker_.open(QIODevice::ReadWrite)) {
    qDebug() << "Open Locker Serial Success";
    serial_locker_.clearError();
    serial_locker_.clear();
  }
}

void RuKu::CloseSerial() {
  serial_scale_.close();
  serial_locker_.close();
}

void RuKu::loopOnce() {
  // 获取和更新图像
  if (camera_cap_ && camera_cap_->isOpened()) {
    // 从摄像头读取图像
    *camera_cap_ >> cur_image_;

    cv::Mat show_camera_img =
        cv::Mat::zeros(cur_image_.size(), cur_image_.type());
    cv::cvtColor(cur_image_, show_camera_img, CV_BGR2RGB);
    QPixmap pixmap = QPixmap::fromImage(
        QImage(show_camera_img.data, show_camera_img.cols, show_camera_img.rows,
               show_camera_img.step, QImage::Format_RGB888));
    pixmap.scaled(ui->cameraImage->size(), Qt::KeepAspectRatio);
    ui->cameraImage->setScaledContents(true);
    ui->cameraImage->setPixmap(pixmap);
  }

  // 根据台秤返回的重量数据，更新显示界面
}

void RuKu::captureImage_trigger() {
  qDebug() << "capture one image";
  if (!cur_image_.empty()) {
    record_image_ = cur_image_;
    // 更新预览图
    cv::Mat show_img =
        cv::Mat::zeros(record_image_.size(), record_image_.type());
    cv::cvtColor(record_image_, show_img, CV_BGR2RGB);
    QPixmap pixmap =
        QPixmap::fromImage(QImage(show_img.data, show_img.cols, show_img.rows,
                                  show_img.step, QImage::Format_RGB888));
    pixmap.scaled(ui->previousImage->size(), Qt::KeepAspectRatio);
    ui->previousImage->setScaledContents(true);
    ui->previousImage->setPixmap(pixmap);
  }
}

void RuKu::finished_trigger() { qDebug() << "finished ruku"; }

void RuKu::exit_trigger() {
  qDebug() << "Close Ruku dialog";
  // 首先关闭定时器
  loop_timer_->stop();

  // 关闭外部接口
  CloseCamera();
  CloseSerial();

  // 重置变量
  if (!record_image_.empty()) {
    record_image_.release();
  }

  this->hide();
}

void RuKu::scale_readyReadSlot() {}

void RuKu::locker_readyReadSlot() {}
