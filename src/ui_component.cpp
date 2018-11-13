#include "ui_component.h"

UIComponentBase::UIComponentBase(Context &context,
                                 mcircuit::WireManager &wireManager)
    : context(context), wireManager(wireManager) {}

void UIComponentBase::init() { createPins(); }

const UIComponentBase::Properties &UIComponentBase::getProperties() const {
  return properties;
}

const UIComponentBase::PropertyValue &
UIComponentBase::getProperty(const UIComponentBase::PropertyId &id) {
  return properties[id];
}

void UIComponentBase::setProperty(const UIComponentBase::PropertyId &property,
                                  UIComponentBase::PropertyValue value) {
  properties[property] = value;
}

void UIComponentBase::processProperty(
    const UIComponentBase::PropertyId &property,
    UIComponentBase::PropertyValue value) {}

unsigned UIComponentBase::getId() const { return getComponent()->getId(); }

void UIComponentBase::setPosition(QPoint position) {
  this->position = std::move(position);
}

void UIComponentBase::moveTo(QPoint position) {
  const QPoint oldPosition(std::move(this->position));
  this->position = std::move(position);

  onMove(oldPosition);
}

void UIComponentBase::onMove(QPoint oldPosition) {}

const QPoint &UIComponentBase::getPosition() const { return position; }

void UIComponentBase::onClick(QMouseEvent *event, QPoint translation) {}

void UIComponentBase::createPins() {}
