#include "renderer.h"

Renderer::Renderer(Context &context)
    : gridDelta(context.gridDelta),
      gridPixmap(context.gridPixmapSize, context.gridPixmapSize) {
  createGridPixmap();
}

void Renderer::setPainter(QPainter *painter) { Renderer::painter = painter; }

void Renderer::drawWireDrawingCursor(int x, int y) const {
  painter->setPen(Qt::black);
  painter->drawEllipse(QPoint(x, y) * gridDelta, gridDelta / 4, gridDelta / 4);
}

void Renderer::drawWirePreview(WireDrawingController::State state,
                               std::tuple<QLine, QLine> lines) const {
  QColor color;

  if (state == WireDrawingController::State::Unallowed) {
    color = Qt::red;
  } else {
    color = Qt::black;
  }

  QPen pen(color, 3.0, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
           Qt::PenJoinStyle::RoundJoin);

  painter->setPen(pen);

  auto &line1 = std::get<0>(lines);
  auto &line2 = std::get<1>(lines);

  painter->drawLine(
      QLine(QPoint(line1.p1()) * gridDelta, QPoint(line1.p2() * gridDelta)));
  painter->drawLine(
      QLine(QPoint(line2.p1()) * gridDelta, QPoint(line2.p2() * gridDelta)));
}

void Renderer::drawWires(const mcircuit::WireManager &wireManager) const {
  QPen pen1(Qt::black, 3.0, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
            Qt::PenJoinStyle::RoundJoin);
  QPen pen2(Qt::black, 8.0, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
            Qt::PenJoinStyle::RoundJoin);

  auto &wires = wireManager.getWires();
  auto &overlaps = wireManager.getOverlaps();

  for (auto &[y, points] : wires[mcircuit::WireManager::Horizontal]) {
    for (auto &p : points) {
      if (overlaps.find({p.position, y}) == overlaps.end()) {
        painter->setPen(pen2);
        painter->drawPoint(QPoint(p.position, y) * gridDelta);
      }

      painter->setPen(pen1);
      painter->drawLine(QPoint{p.position, y} * gridDelta,
                        QPoint{p.endPosition, y} * gridDelta);
    }
  }

  for (auto &[x, points] : wires[mcircuit::WireManager::Vertical]) {
    for (auto &p : points) {
      if (overlaps.find({x, p.position}) == overlaps.end()) {
        painter->setPen(pen2);
        painter->drawPoint(QPoint(x, p.position) * gridDelta);
      }

      painter->setPen(pen1);
      painter->drawLine(QPoint{x, p.position} * gridDelta,
                        QPoint{x, p.endPosition} * gridDelta);
    }
  }
}

void Renderer::drawPins(const mcircuit::WireManager &wireManager) const {
  for (const auto &[pinLocation, componentPin] : wireManager.getPins()) {
    drawPin(std::get<0>(pinLocation), std::get<1>(pinLocation));
  }
}

void Renderer::drawGrid(const QPoint &transl) const {
  auto x = (transl.x() / gridDelta) * gridDelta;
  auto y = (transl.y() / gridDelta) * gridDelta;
  auto w = painter->window().width();
  auto h = painter->window().height();
  painter->drawTiledPixmap(QRect(-x - gridDelta, -y - gridDelta,
                                 w + gridDelta * 2, h + gridDelta * 2),
                           gridPixmap);
}

void Renderer::drawPin(int x, int y) const {
  auto pen = QPen(Qt::black, 4.0);
  pen.setCapStyle(Qt::PenCapStyle::RoundCap);

  painter->setPen(pen);

  painter->drawPoint(QPoint(x, y) * gridDelta);
}

void Renderer::createGridPixmap() {
  QPainter painter(&gridPixmap);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(0, 0, gridPixmap.width(), gridPixmap.height(),
                   CIRCUIT_BACKGROUND_COLOR);
  painter.setPen(QPen(Qt::black, 1.5, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
                      Qt::PenJoinStyle::RoundJoin));

  for (auto x = 0; x < gridPixmap.width(); x += gridDelta) {
    for (auto y = 0; y < gridPixmap.height(); y += gridDelta) {
      painter.drawPoint(x, y);
    }
  }
}

void Renderer::drawWireInspectTooltip(
    int x, int y, mcircuit::WireManager &wireManager) const {
  x += 15; // To avoid overlap with cursor

  auto h = painter->fontMetrics().height();

  painter->setPen(Qt::black);
}
