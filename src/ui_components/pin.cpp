#include "ui_components/pin.h"

void PinUIComponent::setUnderlyingComponent(
    mcircuit::CustomComponent *component) {
  this->component = component;

  initPin();
}

PinUIComponent::Type PinUIComponent::getType() const { return type; }

void PinUIComponent::setType(Type type) { this->type = type; }

void PinUIComponent::paint(QPainter &painter) {
  auto rect = getBoundingBox();
  painter.fillRect(rect, Qt::white);
  painter.setPen(QPen(Qt::black, 2.5, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
                      Qt::PenJoinStyle::RoundJoin));
  painter.drawRect(rect);
}

QRect PinUIComponent::getBoundingBox() {
  return QRect(getPosition() * context.gridDelta,
               QSize(1, 2) * context.gridDelta);
}

mcircuit::Component *PinUIComponent::getComponent() const { return nullptr; }

void PinUIComponent::createPins() {
  if (pin == nullptr)
    return;
  wireManager.placePin(getPosition().x() + 1, getPosition().y() + 1,
                       {component, pin});
}

UIComponentBase *PinUIComponent::clone(unsigned id) { return nullptr; }

void PinUIComponent::processProperty(
    const UIComponentBase::PropertyId &property,
    UIComponentBase::PropertyValue value) {
  if (property == "width") {
    bool ok;
    auto val = value.toUInt(&ok);
    if (ok)
      pin->setWidth(val);
  }
}

void PinUIComponent::init() {
  UIComponentBase::init();
  setProperty("width", 1u);
}

unsigned PinUIComponent::getId() const {
  if (type == Input)
    return 100;
  else
    return 101;
}

void PinUIComponent::initPin() {
  if (type == Input)
    pin = component->addInputPin(mcircuit::Pin(10));
  else
    pin = component->addOutputPin(mcircuit::Pin(11));
}
