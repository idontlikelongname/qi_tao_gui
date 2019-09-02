#ifndef RUKU_H
#define RUKU_H

#include <memory>

#include <QDialog>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "data_model.h"
#include "serial_constant.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class RuKuDialog;
}
QT_END_NAMESPACE

class RuKu : public QDialog {
  Q_OBJECT

 public:
  explicit RuKu(QJsonObject *bom_json, QWidget *parent = 0);
  ~RuKu();

  void Display(CurrentStandard cur_standard_info);

 private:
  void Init();
  void OpenCamera();
  void CloseCamera();
  void OpenSerial();
  void CloseSerial();

 private slots:
  void loopOnce();
  void open_locker_loop();
  void captureImage_trigger();
  void finished_trigger();
  void exit_trigger();
  void scale_readyReadSlot();
  void locker_readyReadSlot();

 private:
  QJsonObject *bom_json_info_;
  Ui::RuKuDialog *ui;
  QTimer *loop_timer_;
  QTimer *open_locker_timer_;

  cv::Mat cur_image_;
  cv::Mat record_image_;

  // 标准件重量
  CurrentStandard cur_selected_standard_info_;

 private:
  // just for debug
  cv::VideoCapture *camera_cap_;
  QSerialPort serial_scale_;
  QSerialPort serial_locker_;
  float current_scale_;
  int current_estimate_count_;
  QByteArray scale_data_buffer_;

  int open_locker_loop_time_;
};

#endif  // RUKU_H
