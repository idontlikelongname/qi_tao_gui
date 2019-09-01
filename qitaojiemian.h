#ifndef QITAOJIEMIAN_H
#define QITAOJIEMIAN_H

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

#include "ruku.h"

namespace Ui {
class QTMainWindow;
}

class QTMainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit QTMainWindow(QWidget *parent = 0);
  ~QTMainWindow();

 private:
  void Init();
  void InitTreeView();
  void UpdateTreeView();
  void SearchStandardByID();

  void paintEvent(QPaintEvent *event);

 private slots:
  void on_actExit_triggered();
  void currentChangedShot(const QModelIndex &selected,
                          const QModelIndex &deselected);
  void on_ruku_clicked();

 private:
  QWidget *parent_dlg;
  Ui::QTMainWindow *ui;

  QStandardItemModel *bom_model_;
  QJsonObject bom_json_info_;
  QJsonArray json_titles_;

  // 当前选中的零件
  QModelIndex cur_selected_ruku_index_;

  // 入库界面
  RuKu *ruku_;
};

#endif  // QITAOJIEMIAN_H
