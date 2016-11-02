#include "TableViewBase.h"

TableViewBase::TableViewBase(QWidget *parent) : QTableView(parent) {}

TableViewBase::~TableViewBase() {}

QList<GridColumn *> TableViewBase::getColumns() const { return columns; }

void TableViewBase::appendColumns(const QList<GridColumn *> &newColumns) {
  columns.append(newColumns);
}

void TableViewBase::appendColumn(GridColumn *column) { columns.append(column); }

void TableViewBase::removeColumn(GridColumn *column) {
  columns.removeOne(column);
}

void TableViewBase::removeAllColumns() { columns.clear(); }

void TableViewBase::showEvent(QShowEvent *event) {
  Q_UNUSED(event)
  hideColumns();
}

void TableViewBase::hideColumns() {
  for (auto column : columns)
    setColumnHidden(column->getIndex(), !column->getVisible());
}
