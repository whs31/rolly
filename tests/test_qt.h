#pragma once

#include <qobject.h>

class QtObjectModule : public QObject {
  Q_OBJECT

 public:
  explicit QtObjectModule(QObject* parent = nullptr);
};

class QtGadgetModule {
  Q_GADGET

 public:
  explicit QtGadgetModule();
};
