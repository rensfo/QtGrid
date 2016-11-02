#include "StaticFilter.h"
#include "StaticFilterItem.h"

StaticFilter::StaticFilter(QWidget *parent) : QWidget(parent) {
  setLayout(new QVBoxLayout());
}

void StaticFilter::setFilters(const QList<GridColumn *> &columns) {
  filters.clear();
  clearLayout();
  hasItem = false;
  for (auto column : columns) {
    if (column->getShowInStaticFilter()) {
      StaticFilterItem *item = new StaticFilterItem();
      connect(item, SIGNAL(changed(QString, QVariant)), this,
              SLOT(filterItemChanged(QString, QVariant)));
      item->setColumn(column);
      item->setEditor(column->createNewEditDelegate());
      layout()->addWidget(item);
      hasItem = true;
    }
  }
  emitChangedSignal = true;
}

void StaticFilter::clearLayout() {}

void StaticFilter::filterItemChanged(QString fieldName, QVariant value) {
  filters[fieldName] = value;
  if (emitChangedSignal)
    emit filtersChanged(converFilters());
}

bool StaticFilter::getHasItem() const { return hasItem; }

Group StaticFilter::converFilters() {
  group.clear();
  for (QVariant value : filters.values()) {
    if (!value.toString().isEmpty()) {
      Filter filter;
      filter.setFieldName(filters.key(value));
      filter.setOperation(Operation::Like);
      filter.addValue(value);
      group.addFilter(filter);
    }
  }

  return group;
}

QMap<QString, QVariant> StaticFilter::getFilters() const { return filters; }
