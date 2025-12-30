#include "HeaderTableView.h"
#include <QHeaderView>

HeaderTableView::HeaderTableView(QWidget *parent) : TableViewBase(parent) {
  tableModelHeader = new HeaderTableModel(this);
  setModel(tableModelHeader);
  filterDelegate = new HeaderTableDelegate(this);
  setItemDelegateForRow(0, filterDelegate);
  //  verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void HeaderTableView::showEvent(QShowEvent *event) {
  TableViewBase::showEvent(event);
  openEditors();
}

void HeaderTableView::appendColumns(const QVector<GridColumn *> &value) {
  TableViewBase::appendColumns(value);
  openEditors();
}

void HeaderTableView::setColumns(QVector<GridColumn *> columns)
{
    TableViewBase::setColumns(columns);
    openEditors();
    update();
}

void HeaderTableView::openEditors() {
  for (int i = 0; i < mColumns.count(); ++i)
    if (mColumns.at(i)->getShowInSearchBar())
      openPersistentEditor(model()->index(0, i));
}
