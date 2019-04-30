#include "main_window.h"

MainWindow::MainWindow() {
  initUi();
  initComponents();
}

void MainWindow::initUi() {
  resize(1280, 720);

  hbox = new QHBoxLayout(this);
  vbox = new QVBoxLayout;
  vbox2 = new QVBoxLayout;

  toolbar = new QToolBar;

  toolbar->addAction(
      "Edit", [this]() { circuitView->setMode(SchematicView::Mode::Editing); });

  toolbar->addAction("Interact", [this]() {
    circuitView->setMode(SchematicView::Mode::Interactive);
  });

  componentList = new QTreeView(this);
  componentList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  propertyEditor = new QTableView(this);
  propertyEditor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  propertyEditor->horizontalHeader()->hide();

  schematics.emplace_back(new mcircuit::CustomComponent(0));
  schematics.emplace_back(new mcircuit::CustomComponent(1));
  currentSchematic = nullptr;

  auto propertyModel = new PropertyEditorModel(this);
  propertyEditor->setModel(propertyModel);
  circuitView = new SchematicView(context);
  propertyModel->setCircuitView(circuitView);

  auto gatesRoot = new QStandardItem("Gates");
  auto notItem = new QStandardItem("Not");
  notItem->setData(QVariant::fromValue(std::make_tuple(0u, false)));
  gatesRoot->appendRow(notItem);

  auto andItem = new QStandardItem("And");
  andItem->setData(QVariant::fromValue(std::make_tuple(1u, false)));
  gatesRoot->appendRow(andItem);

  auto orItem = new QStandardItem("Or");
  orItem->setData(QVariant::fromValue(std::make_tuple(2u, false)));
  gatesRoot->appendRow(orItem);

  auto xorItem = new QStandardItem("Xor");
  xorItem->setData(QVariant::fromValue(std::make_tuple(3u, false)));
  gatesRoot->appendRow(xorItem);

  auto ioRoot = new QStandardItem("IO");
  auto switchItem = new QStandardItem("Switch");
  switchItem->setData(QVariant::fromValue(std::make_tuple(4u, false)));
  ioRoot->appendRow(switchItem);

  auto ledItem = new QStandardItem("LED");
  ledItem->setData(QVariant::fromValue(std::make_tuple(5u, false)));
  ioRoot->appendRow(ledItem);

  auto ledMatrixItem = new QStandardItem("LED Matrix");
  ledMatrixItem->setData(QVariant::fromValue(std::make_tuple(6u, false)));
  ioRoot->appendRow(ledMatrixItem);

  auto schematicsRoot = new QStandardItem("Schematics");
  auto mainSchematicItem = new QStandardItem("main");
  mainSchematicItem->setData(QVariant::fromValue(std::make_tuple(0u, true)));
  schematicsRoot->appendRow(mainSchematicItem);

  auto customSchematicItem = new QStandardItem("custom");
  customSchematicItem->setData(QVariant::fromValue(std::make_tuple(1u, true)));
  schematicsRoot->appendRow(customSchematicItem);

  model.appendRow(schematicsRoot);
  model.appendRow(gatesRoot);
  model.appendRow(ioRoot);

  componentList->setModel(&model);
  componentList->setHeaderHidden(true);
  componentList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  componentList->expandAll();

  connect(componentList, &QTreeView::doubleClicked,
          [this](const QModelIndex &index) {
            auto item = model.itemFromIndex(index);
            auto data = item->data();

            auto [id, isSchematic] =
                item->data().value<std::tuple<unsigned, bool>>();

            if (isSchematic) {
              currentSchematic = &schematics[id];
              circuitView->setSchematic(currentSchematic);
            }
          });

  connect(componentList, &QTreeView::clicked, [this](const QModelIndex &index) {
    auto item = model.itemFromIndex(index);
    auto data = item->data();

    if (!item->data().canConvert<std::tuple<unsigned, bool>>()) {
        circuitView->setPlacingComponent(nullptr);
        return;
    }
          
    auto [id, isSchematic] = item->data().value<std::tuple<unsigned, bool>>();

    if (isSchematic)
      return;

    auto componentId = id;

    circuitView->setPlacingComponent(registry.create(componentId, lastId++));
  });

  connect(circuitView, &SchematicView::uiComponentSelected,
          [propertyModel](UIComponentBase *component) {
            propertyModel->setEditedComponent(component);
          });

  vbox->addWidget(componentList);
  vbox->addWidget(propertyEditor);
  vbox2->addWidget(toolbar);
  vbox2->addWidget(circuitView);

  hbox->addLayout(vbox);
  hbox->addLayout(vbox2);
}

void MainWindow::initComponents() {
  registerUIComponentHelper<mcircuit::NotGate, NotGateUIComponent>();
  registerUIComponentHelper<mcircuit::BasicGate, BasicGateUIComponent>(
      mcircuit::BasicGate::Type::And);
  registerUIComponentHelper<mcircuit::BasicGate, BasicGateUIComponent>(
      mcircuit::BasicGate::Type::Or);
  registerUIComponentHelper<mcircuit::BasicGate, BasicGateUIComponent>(
      mcircuit::BasicGate::Type::Xor);
  registerUIComponentHelper<mcircuit::Switch, SwitchUIComponent>();
  registerUIComponentHelper<mcircuit::LED, LEDUIComponent>();
  registerUIComponentHelper<mcircuit::LEDMatrix, LEDMatrixUIComponent>();
  registerUIComponentHelper<mcircuit::CustomComponent,
                            CustomComponentUIComponent>();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  circuitView->stopSimulation();
}
