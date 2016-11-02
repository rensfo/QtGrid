#ifndef ACTIVELISTBASE_H
#define ACTIVELISTBASE_H

#include <QLinkedList>
#include <QObject>

class ActiveListBase : public QObject {
  Q_OBJECT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  ActiveListBase(QObject *parent = nullptr) : QObject(parent) {}
  virtual void clear() = 0;
  virtual int count() const = 0;
  virtual QObject *qobjectValue(int index) = 0;
  virtual int qobjectIndexOf(QObject *value) const = 0;

signals:
  void countChanged();
  void inserted(int index, int count);
  void removed(int index, int count);
  void cleared();
};

#endif // ACTIVELISTBASE_H
