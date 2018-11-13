#ifndef SWITCH_UI_COMPONENT_H
#define SWITCH_UI_COMPONENT_H

#include "ui_component.h"
#include <QtWidgets>
#include <mcircuit/components/switch.h>

class SwitchUIComponent
    : public UIComponent<mcircuit::Switch, SwitchUIComponent> {
public:
  using UIComponent::UIComponent;

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;
  void onMove(QPoint oldPosition) override;

  void onClick(QMouseEvent *event, QPoint translation) override;

  void createPins() override;

  void processProperty(const PropertyId &property,
                       PropertyValue value) override;

  void init() override;
};

#endif // SWITCH_H
