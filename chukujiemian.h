#ifndef CHUKUJIEMIAN_H
#define CHUKUJIEMIAN_H

#include <QDialog>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QPainter>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

namespace Ui {
class ChKMainWindow;
}

class ChKMainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit ChKMainWindow(QWidget *parent = 0);
  ~ChKMainWindow();

  QWidget *parent_dlg;

 private slots:
  void on_actExit_triggered();

 private:
  Ui::ChKMainWindow *ui;
  void paintEvent(QPaintEvent *event);
};

#endif  // CHUKUJIEMIAN_H
