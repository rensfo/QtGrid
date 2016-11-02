#ifndef STATICFILTER_H
#define STATICFILTER_H

#include <QVariant>
#include <QWidget>

#include "GridColumn.h"
#include "Group.h"

class StaticFilter : public QWidget {
  Q_OBJECT
public:
  explicit StaticFilter(QWidget *parent = 0);
  void setFilters(const QList<GridColumn *> &columns);
  QMap<QString, QVariant> getFilters() const;

  bool getHasItem() const;

private:
  void clearLayout();
  Group converFilters();

signals:
  void filtersChanged(QMap<QString, QVariant>);
  void filtersChanged(Group);

public slots:
  void filterItemChanged(QString fieldName, QVariant value);

private:
  QMap<QString, QVariant> filters;
  bool emitChangedSignal = false;
  bool hasItem = false;
  Group group;
};

#endif // STATICFILTER_H
