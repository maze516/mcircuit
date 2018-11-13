#include "widgets/property_editor.h"

PropertyEditorModel::PropertyEditorModel(QObject *parent)
    : QAbstractTableModel(parent) {
  uiComponent = nullptr;
}

int PropertyEditorModel::rowCount(const QModelIndex &parent) const {
  if (uiComponent == nullptr)
    return 0;

  return uiComponent->getProperties().size();
}

int PropertyEditorModel::columnCount(const QModelIndex &parent) const {
  return 1;
}

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return {};

  if (index.row() >= uiComponent->getProperties().size())
    return {};

  if (uiComponent == nullptr)
    return {};

  auto propName = indexToPropertyName.at(index.row());
  auto prop = uiComponent->getProperty(propName);

  return prop;
}

QVariant PropertyEditorModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role != Qt::DisplayRole)
    return {};

  if (orientation != Qt::Vertical)
    return {};

  if (section >= indexToPropertyName.size())
    return {};

  if (uiComponent == nullptr)
    return {};

  return QString::fromStdString(indexToPropertyName.at(section));
}

bool PropertyEditorModel::setData(const QModelIndex &index,
                                  const QVariant &value, int role) {
  if (role != Qt::EditRole)
    return {};

  if (index.row() >= uiComponent->getProperties().size())
    return {};

  if (uiComponent == nullptr)
    return {};

  auto propName = indexToPropertyName.at(index.row());

  uiComponent->processProperty(propName, value);

  emit dataChanged(index, index, {role});

  return true;
}

void PropertyEditorModel::setEditedComponent(UIComponentBase *uiComponent) {
  this->beginResetModel();

  this->uiComponent = uiComponent;

  auto &props = uiComponent->getProperties();
  indexToPropertyName.clear();

  int i = 0;
  for (auto &[name, value] : props) {
    indexToPropertyName[i++] = name;
  }

  this->endResetModel();
}

void PropertyEditorModel::setCircuitView(SchematicView *view) {
  this->circuitView = view;
}

Qt::ItemFlags PropertyEditorModel::flags(const QModelIndex &index) const {
  if (index.row() >= uiComponent->getProperties().size())
    return {};

  if (uiComponent == nullptr)
    return {};

  if (index.column() == 0) {
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }

  return QAbstractTableModel::flags(index);
}
