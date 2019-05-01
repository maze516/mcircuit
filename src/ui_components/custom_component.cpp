#include "ui_components/custom_component.h"

void CustomComponentUIComponent::notifySchematicChanged(
    mcircuit::CustomComponent *current) {
  int i = 0;

  for (auto &[pinId, pin] : component->getInputs()) {
    wireManager.removePin(getPosition().x(), getPosition().y() + 2 * i + 1);
    i++;
  }

  i = 0;

  for (auto &[pinId, pin] : component->getOutputs()) {
    wireManager.removePin(getPosition().x() + 4, getPosition().y() + 2 * i + 1);
    i++;
  }

  component = static_cast<mcircuit::CustomComponent *>(current->clone(getId()));
  createPins();
}

void CustomComponentUIComponent::paint(QPainter &painter) {
  auto rect = getBoundingBox();

  painter.fillRect(rect, Qt::white);
  painter.setPen(QPen(Qt::black, 2.5, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
                      Qt::PenJoinStyle::RoundJoin));
  painter.drawRect(rect);
}

QRect CustomComponentUIComponent::getBoundingBox() {
  assert(component != nullptr);

  auto gridDelta = context.gridDelta;

  auto inputs = component->getInputs().size();
  auto outputs = component->getOutputs().size();

  auto w = 4 * gridDelta;
  auto h = (std::max(inputs, outputs) + 1) * gridDelta;

  return QRect(getPosition() * gridDelta, QSize(w, h));
}

void CustomComponentUIComponent::createPins() {
  int i = 0;

  for (auto &[pinId, pin] : component->getInputs()) {
    wireManager.placePin(getPosition().x(), getPosition().y() + 2 * i + 1,
                         component, pinId);
    i++;
  }

  i = 0;

  for (auto &[pinId, pin] : component->getOutputs()) {
    wireManager.placePin(getPosition().x() + 4, getPosition().y() + 2 * i + 1,
                         component, pinId);
    i++;
  }
}
