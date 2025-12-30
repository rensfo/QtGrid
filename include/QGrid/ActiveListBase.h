#ifndef ACTIVELISTBASE_H
#define ACTIVELISTBASE_H

#include <QObject>

#include "GridViewExport.h"

class QGRID_EXPORT ActiveListBase : public QObject {
  Q_OBJECT

  Q_PROPERTY(size_t count READ count NOTIFY countChanged)

public:
  ActiveListBase(QObject *parent = nullptr) : QObject(parent) {}
  virtual void clear() = 0;
  virtual size_t count() const = 0;
  virtual QObject *qobjectValue(size_t index) = 0;
  virtual size_t qobjectIndexOf(QObject *value) const = 0;

signals:
  void countChanged();
  void inserted(size_t index, size_t count);
  void removed(size_t index, size_t count);
  void cleared();
};

#endif // ACTIVELISTBASE_H
