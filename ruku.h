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

QT_BEGIN_NAMESPACE
namespace Ui {
class RuKuDialog;
}
QT_END_NAMESPACE

class RuKu : public QDialog {
  Q_OBJECT

 public:
  explicit RuKu(QWidget *parent = 0);
  ~RuKu();

  void Display();

 private:
  void Init();
  void OpenCamera();
  void CloseCamera();
  void OpenSerial();
  void CloseSerial();

 private slots:
  void loopOnce();
  void captureImage_trigger();
  void finished_trigger();
  void exit_trigger();
  void scale_readyReadSlot();
  void locker_readyReadSlot();

 private:
  Ui::RuKuDialog *ui;
  QTimer *loop_timer_;

  cv::Mat cur_image_;
  cv::Mat record_image_;

 private:
  // just for debug
  cv::VideoCapture *camera_cap_;
  QSerialPort serial_scale_;
  QSerialPort serial_locker_;
};

#endif  // RUKU_H