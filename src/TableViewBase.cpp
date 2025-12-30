#include "TableViewBase.h"

TableViewBase::TableViewBase(QWidget *parent) : QTableView(parent) {}

TableViewBase::~TableViewBase() {}

QVector<GridColumn *> TableViewBase::getColumns() const { return mColumns; }

void TableViewBase::setColumns(QVector<GridColumn *> columns)
{
    mColumns = columns;
}

void TableViewBase::clearColumns()
{
    mColumns.clear();
}

void TableViewBase::appendColumns(const QVector<GridColumn *> &newColumns) {
  mColumns.append(newColumns);
}

void TableViewBase::appendColumn(GridColumn *column) { mColumns.append(column); }

void TableViewBase::removeColumn(GridColumn *column) {
  mColumns.removeOne(column);
}

void TableViewBase::removeAllColumns() { mColumns.clear(); }

void TableViewBase::showEvent(QShowEvent *event) {
  Q_UNUSED(event)
  hideColumns();
}

void TableViewBase::hideColumns() {
  for (auto column : mColumns)
    setColumnHidden(column->getIndex(), !column->getVisible());
}
