#ifndef LED_UI_COMPONENT_H
#define LED_UI_COMPONENT_H

#include "ui_component.h"
#include <QtWidgets>
#include <mcircuit/components/led.h>

class LEDUIComponent : public UIComponent<mcircuit::LED, LEDUIComponent> {
public:
  using UIComponent::UIComponent;

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;

  void onMove(QPoint oldPosition) override;

  void createPins() override;
};

#endif // LED_H
