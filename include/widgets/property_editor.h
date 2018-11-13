
#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include "schematic_view.h"
#include "ui_component.h"
#include <QtWidgets>
#include <mutex>
#include <thread>

class PropertyEditorModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit PropertyEditorModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  void setEditedComponent(UIComponentBase *uiComponent);
  void setCircuitView(SchematicView *view);

  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  UIComponentBase *uiComponent;
  SchematicView *circuitView;
  std::map<int, std::string> indexToPropertyName;
};

#endif // PROPERTY_EDITOR_H
