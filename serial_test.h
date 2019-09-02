#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "serial_constant.h"

#include <QTimer>

class SerialTest : public QObject {
  Q_OBJECT
 public:
  void Open();
  //
 private slots:
  void timer_slot();

 private:
  QSerialPort serial_locker_;
  QTimer timer_;
};