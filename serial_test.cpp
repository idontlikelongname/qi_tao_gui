#include "serial_test.h"

void SerialTest::Open() {
  // 打开柜门串口
  serial_locker_.setPortName("ttyUSB0");
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

  timer_.setInterval(700);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(timer_slot()));
  timer_.start();
}

void SerialTest::timer_slot() {
  int locker_id = 2;
  qDebug() << "Open the locker:" << locker_id;
  char sender_cmd[13] = {0xA5, 0x5A, 0x00, 0x00, 0x01, 0xA0, 0x00,
                         0x01, 0x01, 0x00, 0x01, 0xAB, 0xCD};

  // sender_cmd[9] = TO_HEX(static_cast<byte>(locker_id & 0x00FF));
  sender_cmd[10] = 0x00 + locker_id;
  serial_locker_.write(sender_cmd, 13);
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  SerialTest serial_test;
  serial_test.Open();

  return app.exec();
}