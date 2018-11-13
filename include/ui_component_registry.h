#ifndef UI_COMPONENT_REGISTRY_H
#define UI_COMPONENT_REGISTRY_H

#include "ui_component.h"

class UIComponentRegistry {
public:
  using Factory = std::function<UIComponentBase *(unsigned)>;

  void registerUIComponent(unsigned componentId, Factory &&factory) {
    factories.emplace(componentId, factory);
  }

  UIComponentBase *create(unsigned componentId, unsigned id) {
    return factories.at(componentId)(id);
  }

  const std::map<unsigned, Factory> &getUIComponents() { return factories; }

private:
  std::map<unsigned, Factory> factories;
};

#endif // UI_COMPONENT_REGISTRY_H
