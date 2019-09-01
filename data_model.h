#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

/**
 * 使用递归导出树的节点为 JSON 字符串
 * @param item 树的节点
 * @return 返回树的节点为 JSON 字符串
 */
QJsonObject exportTreeNodeToJson(QStandardItem *item) {
  if (NULL == item) {
    return QJsonObject();
  }

  QJsonArray children;  // 子节点的 JSON 对象数组

  for (int row = 0; row < item->rowCount(); ++row) {
    children.append(exportTreeNodeToJson(item->child(row)));  // 递归导出子节点
  }

  QJsonObject region;
  region.insert("name", item->data(Qt::DisplayRole).toString());
  region.insert("children", children);

  return region;
}

/**
 * 导出树为 JSON 字符串
 *
 * @param model 树的 model
 * @return 返回树的 JSON 字符串表示
 */
QString exportTreeToJson(QStandardItemModel *model,
                         QMap<int, QString> info_tile_map) {
  QJsonObject root;
  for (int parts_id = 0; parts_id < model->rowCount(); ++parts_id) {
    QStandardItem *parts = model->item(parts_id);
    QJsonObject part_json;
    for (int standard_id = 0; standard_id < parts->rowCount(); ++standard_id) {
      QStandardItem *sd = parts->child(standard_id);
      // get standard parts information
      QJsonObject standard_json;
      // without title
      for (int info_id = 1; info_id < parts->columnCount(); ++info_id) {
        QStandardItem *info_it = parts->child(standard_id, info_id);

        // TODO: 编号 数量 重量 规格 ...
        standard_json.insert(info_tile_map[info_id],
                             info_it->data(Qt::DisplayRole).toString());
      }

      part_json.insert(sd->data(Qt::DisplayRole).toString(), standard_json);
    }
    root.insert(parts->data(Qt::DisplayRole).toString(), part_json);
  }

  return QJsonDocument(root).toJson();
}

void saveJsonFile(QJsonObject json_object, QString path) {
  QFile saveFile(path);

  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return;
  }

  QJsonDocument saveDoc(json_object);
  saveFile.write(saveDoc.toJson());

  return;
}

QJsonObject loadJsonFile(QString json_path) {
  QFile loadFile(json_path);

  if (!loadFile.open(QIODevice::ReadOnly)) {
    std::cout << "Couldn't open save file.\n";
    return QJsonObject();
  }

  QByteArray saveData = loadFile.readAll();

  QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

  return loadDoc.object();
}

#endif  // DATA_MODEL_H