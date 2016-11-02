#include "GridView.h"
#include "HeaderTableDelegate.h"
#include "ui_GridView.h"

#include <QDebug>
#include <QHeaderView>

GridView::GridView(QWidget *parent) : QFrame(parent), ui(new Ui::GridView) {
  ui->setupUi(this);

  ui->staticFilters->hide();
  toolbar = new QToolBar();
  tableModel = new UniversalTableModel(this);
  tableModelHeader = new HeaderTableModel(this);
  ui->tableData->setModel(tableModel);
  ui->tableHeader->setModel(tableModelHeader);

  QBoxLayout *toolboxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
  toolboxLayout->addWidget(toolbar);
  ui->toolbar->setLayout(toolboxLayout);
  ui->verticalScrollBar->setRange(
      ui->tableData->verticalScrollBar()->minimum(),
      ui->tableData->verticalScrollBar()->maximum());
  ui->verticalScrollBar->setValue(ui->tableData->verticalScrollBar()->value());

  connect(ui->staticFilters, SIGNAL(filtersChanged(Group)), this,
          SLOT(onChangedStaticFilters(Group)));
  connect(ui->tableData,
          SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)), this,
          SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)));
  connect(ui->tableData,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)), this,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)));
  connect(ui->tableHeader->horizontalHeader(),
          SIGNAL(sectionResized(int, int, int)), this,
          SLOT(onHeaderTableSectionResized(int, int, int)));
  connect(ui->tableData->horizontalHeader(),
          SIGNAL(sectionResized(int, int, int)), this,
          SLOT(onDataTableSectionResized(int, int, int)));

  connect(tableModel, SIGNAL(rowsInserted()), this,
          SLOT(resizeVerticalHeader()));
  connect(tableModel, SIGNAL(rowsRemoved()), this,
          SLOT(resizeVerticalHeader()));
  connect(tableModel, SIGNAL(datasourceChanged()), this,
          SLOT(resizeVerticalHeader()));
  connect(ui->tableData->horizontalScrollBar(), SIGNAL(valueChanged(int)),
          ui->tableHeader->horizontalScrollBar(), SLOT(setValue(int)));
  connect(ui->tableData->verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
          SLOT(onTableDataVerticalScrollRangeChanged(int, int)));
  connect(ui->tableData->selectionModel(),
          SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(onTableDataSelectedRowChanged(const QModelIndex &,
                                             const QModelIndex &)));
  connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)),
          ui->tableData->verticalScrollBar(), SLOT(setValue(int)));
  connect(ui->tableData->verticalScrollBar(), SIGNAL(valueChanged(int)),
          ui->verticalScrollBar, SLOT(setValue(int)));
  connect(tableModelHeader, SIGNAL(filtersChanged(Group)), this,
          SLOT(onChangedSearchBarFilters(Group)));

  recreateToolBox();

  ui->tableData->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableData->setSelectionBehavior(QAbstractItemView::SelectRows);
}

GridView::~GridView() { delete ui; }

QList<GridColumn *> GridView::getColumns() const { return columns; }

void GridView::setColumns(const QList<GridColumn *> &value) {
  columns = value;
  tableModel->setColumns(columns);
  tableModelHeader->setColumns(columns);
  ui->staticFilters->setFilters(columns);
  ui->staticFilters->setVisible(ui->staticFilters->getHasItem());
  tableModel->setFilters(ui->staticFilters->getFilters());

  ui->tableHeader->appendColumns(columns);
  ui->tableData->appendColumns(columns);

  for (GridColumn *c : columns) {
    if (c->getIsId())
      idColumn = c;
    if (c->getIsParentId())
      parentIdColumn = c;
  }
}

Mode GridView::getMode() const { return mode; }

void GridView::setMode(const Mode &value) {
  if (mode != value) {
    tableModel->setMode(mode);
    mode = value;
    recreateToolBox();
    emit modeChanged(mode);
  }
}

void GridView::setDatasource(ActiveListBase *value) {
  tableModel->setDatasource(value);
  emit currentRowChanged(nullptr);
}

ActiveListBase *GridView::getDatasource() const {
  return tableModel->getDatasource();
}

void GridView::clearDatasource() { setDatasource(nullptr); }

void GridView::reloadDatasource() { ui->tableData->resizeColumnsToContents(); }

void GridView::deleteCurrentRow() {
  emit removeSelectedRow(ui->tableData->currentIndex().row());
}

void GridView::onHeaderTableSectionResized(const int &index, const int &oldSize,
                                           const int &newSize) {
  Q_UNUSED(oldSize)

  int currentSize = ui->tableData->horizontalHeader()->sectionSize(index);
  if (currentSize != newSize)
    ui->tableData->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::onDataTableSectionResized(const int &index, const int &oldSize,
                                         const int &newSize) {
  Q_UNUSED(oldSize)

  int currentSize = ui->tableHeader->horizontalHeader()->sectionSize(index);
  if (currentSize != newSize)
    ui->tableHeader->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::resizeVerticalHeader() {
  int currentSizeHeader = ui->tableHeader->verticalHeader()->size().width();
  int currentSizeData = ui->tableData->verticalHeader()->sizeHint().width();
  if (currentSizeHeader != currentSizeData) {
    ui->tableHeader->verticalHeader()->setFixedWidth(currentSizeData);
    QMetaObject::invokeMethod(ui->tableHeader, "updateGeometries");
  }
}

void GridView::onChangedStaticFilters(Group filters) {
  staticFilters = filters;
  emit filtersChanged(mergeFilters());
}

void GridView::onChangedSearchBarFilters(Group filters) {
  searchBarFilters = filters;
  emit filtersChanged(mergeFilters());
}

void GridView::onParentCurrentRowChanged(QObject *object) {
  parentFilter.clear();

  QString parentIdFieldName = parentGrid->getIdColumn()->getFieldName();
  QVariant value;
  if (object)
    value = object->property(parentIdFieldName.toStdString().data());
  parentFilter.addFilterEqual(parentIdColumn->getFieldName(), value);

  emit filtersChanged(mergeFilters());
}

void GridView::onRefreshButtonClick() { emit refresh(mergeFilters()); }

void GridView::onTableDataSelectedRowChanged(const QModelIndex &current,
                                             const QModelIndex &previous) {
  Q_UNUSED(previous)

  emit currentRowChanged(tableModel->getObjectByIndex(current.row()));
}

void GridView::onTableDataVerticalScrollRangeChanged(int min, int max) {
  ui->verticalScrollBar->setRange(min, max);
}

void GridView::recreateToolBox() {
  toolbar->clear();
  createToolbox();
}

void GridView::createToolbox() {
  QIcon refreshIcon = style()->standardIcon(QStyle::SP_BrowserReload);
  QAction *refresh = new QAction(refreshIcon, "Обновить данные", this);
  connect(refresh, SIGNAL(triggered(bool)), this, SLOT(onRefreshButtonClick()));
  toolbar->addAction(refresh);
  toolbar->addSeparator();
  if (mode == Mode::ReadWrite) {
    QIcon insertIcon = style()->standardIcon(QStyle::SP_FileIcon);
    QAction *insert = new QAction(insertIcon, "Добавить", this);
    toolbar->addAction(insert);
    connect(insert, SIGNAL(triggered(bool)), this, SIGNAL(insertRow()));

    QIcon delIcon = style()->standardIcon(QStyle::SP_BrowserStop);
    QAction *del = new QAction(delIcon, "Удалить", this);
    toolbar->addAction(del);
    connect(del, SIGNAL(triggered(bool)), this, SLOT(deleteCurrentRow()));
  }
}

void GridView::resizeHeader() {
  int headerHeight = ui->tableHeader->horizontalHeader()->height();
  int filterRowHeight = searchBarShow ? ui->tableHeader->rowHeight(0) : 0;
  ui->tableHeader->setMaximumHeight(headerHeight + filterRowHeight);
}

Group GridView::mergeFilters() {
  Group filters;
  filters.setOperation(GroupOperation::And);

  if (!staticFilters.isEmpty())
    filters.addGroup(staticFilters);
  if (!searchBarFilters.isEmpty())
    filters.addGroup(searchBarFilters);
  if (!parentFilter.isEmpty())
    filters.addGroup(parentFilter);

  return filters;
}

bool GridView::getSearchBarShow() const { return searchBarShow; }

void GridView::setSearchBarShow(bool value) {
  searchBarShow = value;
  resizeHeader();
}

void GridView::showEvent(QShowEvent *event) {
  Q_UNUSED(event)

  resizeHeader();
}

bool GridView::getGridHeaderVisible() const {
  return ui->tableName->isVisible();
}

void GridView::setGridHeaderVisible(bool value) {
  ui->tableName->setVisible(value);
}

bool GridView::getToolboxVisible() const { return ui->toolbar->isVisible(); }

void GridView::setToolboxVisible(bool value) { ui->toolbar->setVisible(value); }

GridView *GridView::getParentGrid() const { return parentGrid; }

void GridView::setParentGrid(GridView *value) {
  if (parentGrid != value) {
    parentGrid = value;
    connect(parentGrid, SIGNAL(currentRowChanged(QObject *)),
            SLOT(onParentCurrentRowChanged(QObject *)));
  }
}

QObject *GridView::getSelectedObject() {
  return ui->tableData->getSelectedObject();
}

GridColumn *GridView::getParentIdColumn() const { return parentIdColumn; }

GridColumn *GridView::getIdColumn() const { return idColumn; }

QString GridView::getTableName() const { return tableName; }

void GridView::setTableName(const QString &value) {
  if (tableName != value) {
    tableName = value;
    ui->tableName->setText(tableName);
  }
}
