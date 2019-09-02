#ifndef DIALOG_H
#define DIALOG_H

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

//! [0]
QT_BEGIN_NAMESPACE
// class QAction;
class QDialogButtonBox;
class QGroupBox;
class QPushButton;

// class QTextEdit;
QT_END_NAMESPACE

//#include "dialog_chuku.h"
#include "chukujiemian.h"
#include "data_model.h"
#include "qitaojiemian.h"

class Dialog : public QDialog {
  Q_OBJECT
 public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();

 private:
  void Init();
  void paintEvent(QPaintEvent *event);

 private:
  QGroupBox *horizontalGroupBox;
  QPushButton *qitaobutton;
  QPushButton *chukubutton;
  QPushButton *yilanbutton;
  QPushButton *qingchubutton;
  QDialogButtonBox *buttonBox;

 private slots:
  void OnClickQiTao();
  void OnClickChuKu();
  void showyilantable();
  void qingchualldata();

 private:
  QJsonObject bom_json_info_;
  QTMainWindow *dialog_qitao;
  ChKMainWindow *dialog_chuku;
};
//! [0]

#endif
