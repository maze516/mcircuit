#ifndef MCIRCUIT_MAINWINDOW_H
#define MCIRCUIT_MAINWINDOW_H

#include "schematic.h"
#include "schematic_view.h"
#include "ui_component_registry.h"
#include "ui_components/basic_gate.h"
#include "ui_components/custom_component.h"
#include "ui_components/led.h"
#include "ui_components/led_matrix.h"
#include "ui_components/not_gate.h"
#include "ui_components/switch.h"
#include "widgets/property_editor.h"
#include <QtCore>
#include <mcircuit/components/basic_gate.h>
#include <mcircuit/components/custom_component.h>
#include <mcircuit/components/led.h>
#include <mcircuit/components/led_matrix.h>
#include <mcircuit/components/not_gate.h>
#include <mcircuit/components/switch.h>
#include <tuple>

using Tupl = std::tuple<unsigned, bool>;
Q_DECLARE_METATYPE(Tupl)

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow();

private:
  void initUi();

  void initComponents();

  template <typename ComponentType, typename UIComponentType>
  void registerUIComponentHelper() {
    registry.registerUIComponent(lastRegisteredId++, [this](unsigned id) {
      auto component = new ComponentType(id);
      return new UIComponentType(component, context,
                                 currentSchematic->wireManager);
    });
  }

  template <typename ComponentType, typename UIComponentType, typename... Args>
  void registerUIComponentHelper(Args &&... args) {
    registry.registerUIComponent(
        lastRegisteredId++, [this, args...](unsigned id) {
          auto component = new ComponentType(id, args...);
          return new UIComponentType(component, context,
                                     currentSchematic->wireManager);
        });
  }

  QHBoxLayout *hbox;
  QVBoxLayout *vbox;
  QVBoxLayout *vbox2;
  QToolBar *toolbar;
  QTreeView *componentList;
  SchematicView *circuitView;
  QTableView *propertyEditor;
  QStandardItemModel model;
  Context context;
  unsigned lastId = 1;
  unsigned lastRegisteredId = 0;

  UIComponentRegistry registry;

  std::vector<Schematic> schematics;
  Schematic *currentSchematic;

protected:
  void closeEvent(QCloseEvent *event) override;
};

#endif
