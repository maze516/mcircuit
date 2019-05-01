#include "schematic_view.h"

#include <chrono>
#include <iostream>

SchematicView::SchematicView(Context &context)
    : context{context}, renderer{context} {
  setMode(Mode::Editing);
  setMouseTracking(true);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  running = true;

  connect(&timer, &QTimer::timeout, [this]() {
    update();

    if (!running)
      timer.stop();
  });

  timer.start(1000 / 70);

  simulationThread = std::thread([this]() {
    using clock = std::chrono::high_resolution_clock;
    using namespace std::chrono_literals;
    auto old = clock::now();
    std::chrono::nanoseconds accum(0);

    while (running) {
      std::chrono::nanoseconds rate((unsigned)(1e9 / 5000.f));

      auto curr = clock::now();
      auto delta = curr - old;
      old = curr;
      accum += std::chrono::duration_cast<std::chrono::nanoseconds>(delta);

      while (accum >= rate) {
        accum -= rate;

        {
          std::lock_guard<std::mutex> lk{mutex};

          if (schematic != nullptr) {
            schematic->component->updateState();
          }
        }

        ticks++;
      }
    }
  });

  connect(&benchTimer, &QTimer::timeout, [this]() {
    freq = ticks;
    ticks = 0;

    if (!running)
      benchTimer.stop();
  });

  benchTimer.start(1000);
}

void SchematicView::paintEvent(QPaintEvent *event) {
  if (schematic == nullptr)
    return;

  std::lock_guard<std::mutex> lk{mutex};

  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);

  renderer.setPainter(&painter);

  painter.save();
  painter.setViewport(rect().translated(transl));
  renderer.drawGrid(transl);

  for (auto &[id, uiComponent] : schematic->uiComponents) {
    uiComponent->paint(painter);
  }

  renderer.drawWires(schematic->wireManager);

  if (wireDrawingController.getState() !=
      WireDrawingController::State::Inactive) {
    auto [w1, w2] = wireDrawingController.getPreviewWire();

    renderer.drawWirePreview(wireDrawingController.getState(), {w1, w2});
  }

  renderer.drawPins(schematic->wireManager);

  if (placingComponent == nullptr && getComponentUnderCursor() == nullptr) {
    renderer.drawWireDrawingCursor(
        std::round((mousePosition.x() - transl.x()) / (float)context.gridDelta),
        std::round((mousePosition.y() - transl.y()) /
                   (float)context.gridDelta));
  }

  if (placingComponent != nullptr) {
    placingComponent->paint(painter);
  }

  painter.restore();

  auto x = (mousePosition.x() + context.gridDelta / 2 - transl.x()) /
           context.gridDelta;
  auto y = (mousePosition.y() + context.gridDelta / 2 - transl.y()) /
           context.gridDelta;

  //    auto &node = wireManager.getNodeAt(x, y);
  //    mcircuit::Pin *pin = nullptr;
  //    if (node.isSource() != nullptr)
  //      pin = std::get<1>(*node.source->data);

  auto mpost = (mousePosition - transl) / context.gridDelta;
  renderer.drawWireInspectTooltip(mpost.x(), mpost.y(), schematic->wireManager);

  painter.drawText(10, 20, QString().sprintf("Frequency %d", freq));
  painter.drawText(10, 20 + fontMetrics().height(),
                   QString().sprintf("%d %d", mpost.x(), mpost.y()));
  painter.drawText(10, 20 + fontMetrics().height() * 2,
                   QString().sprintf("%d %d", transl.x(), transl.y()));

  /*if (wireManager.wireContainsPoint(mpost.x(), mpost.y())) {
    painter.drawText(10, 20 + fontMetrics().height(),
                     QString("Point is in wire"));
  }*/
}

void SchematicView::mouseMoveEvent(QMouseEvent *event) {
  mousePosition = event->pos();

  if (panning) {
    translDelta = mousePosition - panStart;
    transl = translStart + translDelta;
  }

  auto tpos = (mousePosition - transl) / context.gridDelta;

  if (mode == Mode::Editing) {
    if (placingComponent != nullptr) {
      placingComponent->setPosition(tpos);
    }

    wireDrawingController.setWireEndPosition(tpos);
  }
}

void SchematicView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::MiddleButton) {
    panning = true;
    setCursor(Qt::CursorShape::ClosedHandCursor);
    translStart = transl;
    panStart = mousePosition;
    return;
  }

  if (mode == Mode::Editing) {
    if (placingComponent != nullptr) {
      if (event->button() == Qt::LeftButton) {
        bool ok = true;

        for (auto &[id, uiComponent] : schematic->uiComponents) {
          if (uiComponent->getBoundingBox().intersects(
                  placingComponent->getBoundingBox())) {
            ok = false;
            break;
          }
        }

        if (ok) {
          auto component = placingComponent->getComponent();
          if (component != nullptr)
            schematic->component->addComponent(component);
          placingComponent->init();
          schematic->uiComponents.emplace(placingComponent->getId(),
                                          std::move(placingComponent));
        }
      }
    }

    if (event->button() == Qt::LeftButton) {
      wireDrawingController.startWirePlacement(
          WireDrawingController::State::Placing,
          (mousePosition - transl) / context.gridDelta);
    } else if (event->button() == Qt::RightButton) {
      wireDrawingController.startWirePlacement(
          WireDrawingController::State::Removing,
          (mousePosition - transl) / context.gridDelta);
    }
  }
}

void SchematicView::mouseReleaseEvent(QMouseEvent *event) {
  if (panning) {
    panning = false;
    setCursor(Qt::CursorShape::ArrowCursor);
  }

  if (mode == Mode::Editing) {
    auto comp = getComponentUnderCursor();

    if (comp != nullptr) {
      emit uiComponentSelected(comp);
    }

    wireDrawingController.setWireEndPosition((mousePosition - transl) /
                                             context.gridDelta);
    wireDrawingController.endWirePlacement();
  } else {
    if (auto component = getComponentUnderCursor(); component != nullptr) {
      component->onClick(event, transl);
    }
  }
}

void SchematicView::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
  }
}

void SchematicView::setPlacingComponent(UIComponentBase *component) {
  placingComponent.reset(component);
}

void SchematicView::stopSimulation() {
  running = false;
  simulationThread.join();
}

void SchematicView::setMode(SchematicView::Mode mode) {
  this->mode = mode;

  if (mode == Mode::Editing) {
    setCursor(Qt::ArrowCursor);
  } else {
    setCursor(Qt::PointingHandCursor);
  }
}

UIComponentBase *SchematicView::getComponentOnPosition(const QPoint &position) {
  if (schematic == nullptr)
    return nullptr;
  for (auto &[id, uiComponent] : schematic->uiComponents) {
    if (uiComponent->getBoundingBox().contains(position)) {
      return uiComponent.get();
    }
  }

  return nullptr;
}

UIComponentBase *SchematicView::getComponentUnderCursor() {
  return getComponentOnPosition(mousePosition - transl);
}
void SchematicView::setSchematic(Schematic *schematic) {
  this->schematic = schematic;
  wireDrawingController.setWireManager(&schematic->wireManager);
}
