#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

struct CurrentStandard {
  QString std_name;
  QString part_name;
  float weight;
  int count;
  int locker_id;
};

/**
 * 使用递归导出树的节点为 JSON 字符串
 * @param item 树的节点
 * @return 返回树的节点为 JSON 字符串
 */
QJsonObject exportTreeNodeToJson(QStandardItem *item);

/**
 * 导出树为 JSON 字符串
 *
 * @param model 树的 model
 * @return 返回树的 JSON 字符串表示
 */
QString exportTreeToJson(QStandardItemModel *model,
                         QMap<int, QString> info_tile_map);

void saveJsonFile(QJsonObject json_object, QString path);

QJsonObject loadJsonFile(QString json_path);

#endif  // DATA_MODEL_H