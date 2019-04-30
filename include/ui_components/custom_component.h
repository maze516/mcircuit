#ifndef CUSTOM_COMPONENT_UI_COMPONENT_H
#define CUSTOM_COMPONENT_UI_COMPONENT_H

#include "ui_component.h"
#include <mcircuit/components/custom_component.h>

class CustomComponentUIComponent
    : public UIComponent<mcircuit::CustomComponent,
                         CustomComponentUIComponent> {
public:
  using UIComponent::UIComponent;

  void notifySchematicChanged(mcircuit::CustomComponent *current);

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;

  void createPins() override;
};

#endif
