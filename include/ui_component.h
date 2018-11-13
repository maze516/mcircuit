#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "context.h"
#include "mcircuit/wire_manager.h"
#include <QtWidgets>
#include <any>
#include <mcircuit/component.h>

class UIComponentBase {
public:
  using PropertyId = std::string;
  using PropertyValue = QVariant;
  using Properties = std::map<PropertyId, PropertyValue>;

  UIComponentBase(Context &context, mcircuit::WireManager &wireManager);

  virtual void paint(QPainter &painter) = 0;
  virtual QRect getBoundingBox() = 0;

  virtual void init();

  const Properties &getProperties() const;
  const PropertyValue &getProperty(const PropertyId &id);
  void setProperty(const PropertyId &property, PropertyValue value);
  virtual void processProperty(const PropertyId &property, PropertyValue value);

  unsigned getId() const;

  virtual ~UIComponentBase() = default;

  void setPosition(QPoint position);
  void moveTo(QPoint position);

  virtual void onMove(QPoint oldPosition);

  const QPoint &getPosition() const;

  virtual mcircuit::Component *getComponent() const = 0;

  virtual void onClick(QMouseEvent *event, QPoint translation);

  virtual void createPins();

  virtual UIComponentBase *clone(unsigned id) = 0;

protected:
  Context &context;
  mcircuit::WireManager &wireManager;

private:
  QPoint position;
  Properties properties;
};

template <typename ComponentType, typename ClonedType>
class UIComponent : public UIComponentBase {
public:
  UIComponent(ComponentType *component, Context &context,
              mcircuit::WireManager &wireManager)
      : UIComponentBase(context, wireManager), component(component) {}

  virtual mcircuit::Component *getComponent() const override {
    return component;
  }

  virtual UIComponentBase *clone(unsigned id) override {
    return new ClonedType(static_cast<ComponentType *>(component->clone(id)),
                          context, wireManager);
  }

protected:
  ComponentType *component;
};

#endif // UI_COMPONENT_H
