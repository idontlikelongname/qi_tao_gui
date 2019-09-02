#ifndef YILANTABLE_H
#define YILANTABLE_H

#include <QMainWindow>
#include <QTreeView>
#include <QPainter>

#include "data_model.h"

namespace Ui {
class yilantable;
}

class yilantable : public QMainWindow
{
    Q_OBJECT

public:
    explicit yilantable(QJsonObject *bom_data,QWidget *parent = 0);
    ~yilantable();

    QWidget *parent_dlg;

private slots:
 void on_exitButton_triggered();

private:
    Ui::yilantable *ui;

    QStandardItemModel *bom_model_;
    QJsonObject *bom_json_info_;
    QJsonArray json_titles_;


    void Init();
    void InitTreeView();
    void UpdateTreeView();

    void paintEvent(QPaintEvent *event);

};

#endif // YILANTABLE_H
