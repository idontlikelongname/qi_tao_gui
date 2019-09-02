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

#include "data_model.h"

namespace Ui {
class ChKMainWindow;
}

class ChKMainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit ChKMainWindow(QJsonObject *bom_data, QWidget *parent = 0);
  ~ChKMainWindow();

  QWidget *parent_dlg;

 private slots:
  void on_actExit_triggered();
  void currentChangedShot(const QModelIndex &selected,
                          const QModelIndex &deselected);
  void on_chuku_clicked();


 private:
  Ui::ChKMainWindow *ui;

  QStandardItemModel *bom_model_;
  QJsonObject *bom_json_info_;
  QJsonArray json_titles_;

  // 当前选中的零件
  QModelIndex cur_selected_chuku_index_;
  //CurrentStandard cur_selected_standard_info_;


  void Init();
  void InitTreeView();
  void UpdateTreeView();
  void paintEvent(QPaintEvent *event);
};

#endif  // CHUKUJIEMIAN_H
