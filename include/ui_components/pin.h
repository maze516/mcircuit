#ifndef PIN_UI_COMPONENT_H
#define PIN_UI_COMPONENT_H

#include "ui_component.h"
#include <mcircuit/pin.h>

class PinUIComponent : public UIComponentBase {
public:
  enum Type { Input, Output };

  using UIComponentBase::UIComponentBase;

  void setUnderlyingComponent(mcircuit::CustomComponent *component);
  Type getType() const;
  void setType(Type type);
  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;
  mcircuit::Component *getComponent() const override;
  void createPins() override;
  UIComponentBase *clone(unsigned id) override;
  void processProperty(const PropertyId &property,
                       PropertyValue value) override;
  void init() override;
  unsigned getId() const override;

private:
  void initPin();

  Type type;
  mcircuit::CustomComponent *component;
  mcircuit::Pin *pin;
};

#endif // PIN_UI_COMPONENT_H
