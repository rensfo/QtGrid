#include "TableView.h"

#include <QDebug>
#include <QHeaderView>
#include <QLabel>

TableView::TableView(QWidget *parent) : TableViewBase(parent) {
  universalDelegate = new UniversalDelegate(this);
  setItemDelegate(universalDelegate);
  connect(universalDelegate,
          SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)), this,
          SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)));
  connect(universalDelegate,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)), this,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)));

  //  horizontalHeader()->setSectionsMovable(true);
}

TableView::~TableView() {}

Mode TableView::getMode() const { return mode; }

void TableView::setMode(const Mode &value) {
  if (mode != value) {
    mode = value;
    emit modeChanged(mode);
  }
}

QObject *TableView::getSelectedObject() {
  int currentRow = currentIndex().row();
  if (currentRow == -1)
    return nullptr;

  auto m = qobject_cast<UniversalTableModel *>(model());

  return m->getObjectByIndex(currentRow);
}
