#include "HeaderTableModel.h"

HeaderTableModel::HeaderTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int HeaderTableModel::rowCount(const QModelIndex &index) const {
  Q_UNUSED(index)

  return 1;
}

int HeaderTableModel::columnCount(const QModelIndex &index) const {
  Q_UNUSED(index)

  return columns.count();
}

QVariant HeaderTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    if (section < columns.count()) {
      GridColumn *column = columns.at(section);
      if (!column->getHeader().isEmpty() && !column->getHeader().isNull())
        return column->getHeader();
    }
    return QVariant();
  } else {
    return QVariant();
  }
}

Qt::ItemFlags HeaderTableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant HeaderTableModel::data(const QModelIndex &index, int role) const {
  Q_UNUSED(role)

  if (!index.isValid())
    return QVariant();

  if (role == Qt::EditRole)
    return filtersValues.value(columns.at(index.column())->getFieldName());

  return QVariant();
}

bool HeaderTableModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  if (index.isValid() && role == Qt::EditRole) {
    QString fieldName = columns.at(index.column())->getFieldName();
    Filter filter = value.value<Filter>();
    QVariant filterValue = filter.getValues().first();
    if (filterValue.toString().isEmpty()) {
      removeFilter(fieldName);
    } else {
      filtersValues[fieldName] = filterValue;
      filter.setFieldName(fieldName);
      updateOrCreateFilter(filter);
    }
  }

  return false;
}

QList<GridColumn *> HeaderTableModel::getColumns() const { return columns; }

void HeaderTableModel::setColumns(const QList<GridColumn *> &value) {
  beginResetModel();
  columns = value;
  endResetModel();
}

GridColumn *HeaderTableModel::getColumn(int index) {
  return columns.value(index);
}

void HeaderTableModel::updateFilters() {
  filters.clear();
  filters.setOperation(GroupOperation::Or);
  for (auto value = filtersValues.begin(); value != filtersValues.end();
       ++value) {
    Filter filter;
    filter.setFieldName(value.key());
    filter.setOperation(Operation::Like);
    filter.addValue(value.value());
    filters.addFilter(filter);
  }
  emit filtersChanged(filters);
}

void HeaderTableModel::updateOrCreateFilter(const Filter &filter) {
  bool existFilter = false;
  for (auto f : filters.getFilters()) {
    if (f->getFieldName() == filter.getFieldName()) {
      if (filter.getValues().first().toString().isEmpty()) {
        filters.removeFilter(f);
      } else {
        f->clearValues();
        f->setOperation(filter.getOperation());
        f->addValue(filter.getValues().first());
        existFilter = true;
      }
      break;
    }
  }
  if (!existFilter) {
    filters.addFilter(filter);
  }
  emit filtersChanged(filters);
}

void HeaderTableModel::removeFilter(const QString &field) {
  for (auto f : filters.getFilters()) {
    if (f->getFieldName() == field)
      filters.removeFilter(f);
  }
  emit filtersChanged(filters);
}
