#include <QDebug>
#include <iostream>

#include "ruku.h"
#include "ui_ruku.h"

RuKu::RuKu(QJsonObject *bom_json, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::RuKuDialog),
      camera_cap_(NULL),
      bom_json_info_(bom_json),
      open_locker_loop_time_(-1) {
  // 禁止调整窗口大小 移动窗口
  // this->setWindowFlags(this->windowFlags() | (Qt::WindowMaximizeButtonHint |
  //                                             Qt::WindowMinimizeButtonHint |
  //                                             Qt::WindowCloseButtonHint));
  // this->setFixedSize(this->width(), this->height());
  ui->setupUi(this);
  this->setWindowTitle(tr("入库对话框"));

  // shezhizitidaxiao
  QFont font1("Microsoft YaHei", 30, 50, false);
  QFont font2("Microsoft YaHei", 20, 50, false);
  QFont font3("Microsoft YaHei", 26, 50, false);

  ui->captureButton->setFont(font2);
  ui->captureButton->setFixedSize(200, 60);

  ui->exitButton->setFont(font2);
  ui->exitButton->setFixedSize(200, 60);

  ui->finishButton->setFont(font2);
  ui->finishButton->setFixedSize(200, 60);

  ui->label->setFont(font3);
  ui->label_2->setFont(font3);
  ui->weightLine->setFont(font1);
  ui->weightLine->setFixedSize(300, 60);
  ui->countLine->setFont(font1);
  ui->countLine->setFixedSize(300, 60);
  ui->groupBox_2->setFont(font2);

  Init();
}

RuKu::~RuKu() { delete ui; }

void RuKu::Init() {
  loop_timer_ = new QTimer(this);
  loop_timer_->setInterval(100);
  connect(loop_timer_, SIGNAL(timeout()), this, SLOT(loopOnce()));

  open_locker_timer_ = new QTimer(this);
  open_locker_timer_->setInterval(700);
  connect(open_locker_timer_, SIGNAL(timeout()), this,
          SLOT(open_locker_loop()));

  connect(ui->captureButton, SIGNAL(clicked()), this,
          SLOT(captureImage_trigger()));
  connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finished_trigger()));
  connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(exit_trigger()));
}

void RuKu::Display(CurrentStandard cur_standard_info) {
  current_scale_ = 0.0;
  scale_data_buffer_.clear();
  cur_selected_standard_info_ = cur_standard_info;
  ui->previousImage->clear();

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
  serial_scale_.setPortName("ttyUSB0");
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
  serial_locker_.setPortName("ttyUSB1");
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
  serial_locker_.clearError();
  serial_locker_.clear();
  serial_scale_.clearError();
  serial_scale_.clear();
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
  // 解析读取的台秤数据，提取其中的重量数据
  // qDebug() << scale_data_buffer_.size();
  // qDebug() << scale_data_buffer_;
  int32_t scale_data = 0;
  if (scale_data_buffer_.size() > 18) {
    int read_length = 0;
    for (int bf_i = 0; bf_i < 9; ++bf_i) {
      // 肯定能读到某帧数据
      if (scale_data_buffer_.at(bf_i) == 0x01 &&
          scale_data_buffer_.at(bf_i + 1) == 0x03 &&
          scale_data_buffer_.at(bf_i + 2) == 0x04) {
        // 读取接下来的4byte数据
        scale_data |= (0xff & scale_data_buffer_.at(bf_i + 3)) << 24;
        scale_data |= (0xff & scale_data_buffer_.at(bf_i + 4)) << 16;
        scale_data |= (0xff & scale_data_buffer_.at(bf_i + 5)) << 8;
        scale_data |= (0xff & scale_data_buffer_.at(bf_i + 6));
        // qDebug() << "Read Valid scale:" << static_cast<int32_t>(scale_data);
        read_length = bf_i + 6;
        current_scale_ = scale_data / 100.0;

        current_estimate_count_ =
            round(current_scale_ / cur_selected_standard_info_.weight);

        ui->weightLine->setText(QString::number(current_scale_));
        ui->countLine->setText(QString::number(current_estimate_count_));
        // TODO: 更新重量栏
      }
    }

    if (read_length > 0) {
      scale_data_buffer_.remove(0, read_length);
    }
  }

  // 发送读取重量的请求，在scale_readyReadSlot中读取重量数据
  serial_scale_.write(READ_SCALE, 8);
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

void RuKu::finished_trigger() {
  qDebug() << "finished ruku";
  // TODO: 首先检查是否符合开门要求，不符合开门要求，弹窗警告
  if (current_estimate_count_ != cur_selected_standard_info_.count) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("单发齐套柜");
    msgBox.setText("请确认入库数量与标准数据是否一致");
    // msgBox.setInformativeText("Do you want to start a new deck?");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setStyleSheet(
        "QLabel{width:300 px; font-size: 30px;} "
        "QPushButton{ width:60 px; font-size: 20px;}");

    if (msgBox.exec() != QMessageBox::Yes) {
      return;
    }
  }

  // 根据输入的柜门ID，打开相应的柜门
  // qDebug() << "Open the locker:" << cur_selected_standard_info_.locker_id;
  // char sender_cmd[13];
  // strncpy(sender_cmd, OPEN_LOCKER, 13);
  // sender_cmd[10] = static_cast<char>(cur_selected_standard_info_.locker_id);
  // serial_locker_.write(sender_cmd, 13);
  open_locker_loop_time_ = 3;
  open_locker_timer_->start();

  // 更新bom表中的信息
  QJsonObject bom_list = (*bom_json_info_)["信息"].toObject();
  QJsonObject part_info =
      bom_list[cur_selected_standard_info_.part_name].toObject();
  QJsonObject standard_info =
      part_info[cur_selected_standard_info_.std_name].toObject();
  // 更新信息，入库状态，柜子信息
  standard_info["入库状态"] = true;
  // 更新该设备下的所有标准件的柜子编号
  part_info[cur_selected_standard_info_.std_name] = standard_info;
  for (QJsonObject::Iterator std_it = part_info.begin();
       std_it != part_info.end(); ++std_it) {
    //
    QString std_name = std_it.key();
    QJsonObject std_info = std_it.value().toObject();
    std_info["仓库编号"] = cur_selected_standard_info_.locker_id;
    part_info[std_name] = std_info;
  }
  // 更新设备信息
  bom_list[cur_selected_standard_info_.part_name] = part_info;
  (*bom_json_info_)["信息"] = bom_list;
}

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
  current_scale_ = 0.0;

  this->hide();
}

void RuKu::scale_readyReadSlot() {
  QByteArray arr = serial_scale_.readLine();

  for (int i = 0; i < arr.size(); ++i) {
    scale_data_buffer_.append(arr.at(i));
  }
}

void RuKu::locker_readyReadSlot() {}

void RuKu::open_locker_loop() {
  if (open_locker_loop_time_ <= 0) {
    open_locker_loop_time_ = -1;
    open_locker_timer_->stop();
    return;
  }

  // 根据输入的柜门ID，打开相应的柜门
  // qDebug() << "Open Loop Time:" << open_locker_loop_time_;
  qDebug() << "Open the locker:" << cur_selected_standard_info_.locker_id;
  char sender_cmd[13];
  memcpy(sender_cmd, OPEN_LOCKER, 13);
  sender_cmd[10] = 0x00 + cur_selected_standard_info_.locker_id;
  serial_locker_.write(sender_cmd, 13);
  open_locker_loop_time_--;
}
