#include "ui_components/basic_gate.h"

void BasicGateUIComponent::paint(QPainter &painter) {
  auto gridDelta = context.gridDelta;

  int h = 2 * component->getNumInputs();

  QRect rect(QPoint(0, 0), QSize(3, h) * gridDelta);
  rect.translate(getPosition() * gridDelta);

  painter.fillRect(rect, Qt::white);
  painter.setPen(Qt::black);
  painter.drawRect(rect);

  auto font = painter.font();

  font.setPixelSize(15);
  painter.setFont(font);

  using Type = mcircuit::BasicGate::Type;

  switch (component->getType()) {
  case Type::Nand:
  case Type::And: {
    painter.drawText(rect, Qt::AlignCenter, "&");
    break;
  }
  case Type::Or:
  case Type::Nor: {
    painter.drawText(rect, Qt::AlignCenter, ">=1");
    break;
  }
  case Type::Xor:
  case Type::Xnor: {
    painter.drawText(rect, Qt::AlignCenter, "=1");
    break;
  }
  }
}

QRect BasicGateUIComponent::getBoundingBox() {
  return QRect(getPosition() * context.gridDelta,
               QSize(component->getNumInputs(), 1) * context.gridDelta);
}

void BasicGateUIComponent::onMove(QPoint oldPosition) {
  int x = oldPosition.x();
  int y = oldPosition.y();

  for (auto i = 0u; i < component->getNumInputs(); i++) {
    //    wireManager.removePin(x, y + 2 * i + 1);
  }

  //  wireManager.removePin(x + 3, y + component->getNumInputs());

  createPins();
}

void BasicGateUIComponent::createPins() {
  for (auto i = 0u; i < component->getNumInputs(); i++) {
    wireManager.placePin(getPosition().x(), getPosition().y() + 2 * i + 1,
                         component, component->getInputId(i));
  }

  wireManager.placePin(getPosition().x() + 3,
                       getPosition().y() + component->getNumInputs(), component,
                       mcircuit::BasicGate::OUTPUT_PIN_ID);
}
