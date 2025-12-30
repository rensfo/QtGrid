#include "QGrid/GridView.h"
#include "delegates/HeaderTableDelegate.h"
#include "ui_GridView.h"
#include "ColumnsSerializer.h"

#include <QDebug>
#include <QHeaderView>
#include <QStringList>

GridView::GridView(QWidget *parent) : QFrame(parent), mUi(new Ui::GridView)
{
    qRegisterMetaType<GridColumn>("GridColumn");
    qRegisterMetaType<ColumnsVector>("ColumnsVector");

    mUi->setupUi(this);

    mUi->staticFilters->hide();
    toolbar = new QToolBar();
    tableModel = new UniversalTableModel(this);
    // tableModelHeader = new HeaderTableModel(this);
    mUi->tableData->setModel(tableModel);
    // mUi->tableHeader->setModel(tableModelHeader);

    QBoxLayout *toolboxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    toolboxLayout->addWidget(toolbar);
    mUi->toolbar->setLayout(toolboxLayout);
    mUi->verticalScrollBar->setRange(
        mUi->tableData->verticalScrollBar()->minimum(),
        mUi->tableData->verticalScrollBar()->maximum());
    mUi->verticalScrollBar->setValue(mUi->tableData->verticalScrollBar()->value());

    connect(mUi->staticFilters, qOverload<Group>(&StaticFilter::filtersChanged), this,
            &GridView::onChangedStaticFilters);
    connect(mUi->tableData,
            &TableView::initDelegate, this,
            &GridView::initDelegate);
    connect(mUi->tableData,
            &TableView::endEdit, this,
            &GridView::endEdit);
    connect(mUi->tableHeader->horizontalHeader(),
            &QHeaderView::sectionResized, this,
            &GridView::onHeaderTableSectionResized);
    connect(mUi->tableData->horizontalHeader(),
            &QHeaderView::sectionResized, this,
            &GridView::onDataTableSectionResized);

    connect(tableModel, &UniversalTableModel::rowsInserted, this,
            &GridView::resizeVerticalHeader);
    connect(tableModel, &UniversalTableModel::rowsRemoved, this,
            &GridView::resizeVerticalHeader);
    connect(tableModel, &UniversalTableModel::datasourceChanged, this,
            &GridView::resizeVerticalHeader);
    connect(mUi->tableData->horizontalScrollBar(), &QScrollBar::valueChanged,
            mUi->tableHeader->horizontalScrollBar(), &QScrollBar::setValue);
    connect(mUi->tableData->verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &GridView::onTableDataVerticalScrollRangeChanged);
    connect(mUi->tableData->selectionModel(),
            &QItemSelectionModel::currentRowChanged, this,
            &GridView::onTableDataSelectedRowChanged);
    connect(mUi->verticalScrollBar, &QScrollBar::valueChanged,
            mUi->tableData->verticalScrollBar(), &QScrollBar::setValue);
    connect(mUi->tableData->verticalScrollBar(), &QScrollBar::valueChanged,
            mUi->verticalScrollBar, &QScrollBar::setValue);
    connect(mUi->tableHeader->getHeaderModel(), qOverload<Group>(&HeaderTableModel::filtersChanged), this,
            &GridView::onChangedSearchBarFilters);

    recreateToolBox();

    mUi->tableData->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->tableData->setSelectionBehavior(QAbstractItemView::SelectRows);
}

GridView::~GridView() { delete mUi; }

bool GridView::getHeaderVisible() const
{
    return mUi->headerText->isVisible();
}

void GridView::setHeaderVisible(bool value)
{
    mUi->headerText->setVisible(value);
}

QString GridView::getHeaderText() const
{
    return mUi->headerText->text();
}

void GridView::setHeaderText(const QString &value)
{
    if (mUi->headerText->text() != value)
    {
        mUi->headerText->setText(value);
    }
}

ColumnsVector GridView::getColumns() const
{
    return mColumns;
}

void GridView::setColumns(const ColumnsVector &value)
{
    mColumns = value;
    tableModel->setColumns(mColumns);
    mUi->tableHeader->setColumns(mColumns);
    mUi->staticFilters->setFilters(mColumns);
    mUi->staticFilters->setVisible(mUi->staticFilters->getHasItem());
    tableModel->setFilters(mUi->staticFilters->getFilters());

    mUi->tableHeader->setColumns(mColumns);
    mUi->tableData->setColumns(mColumns);

    for (GridColumn *c : mColumns)
    {
        if (c->getIsId())
            idColumn = c;
        if (c->getIsParentId())
            parentIdColumn = c;
    }
}

QString GridView::getColumnsData() const
{
    return ColumnsSerializer::serialize(mColumns);
}

void GridView::setColumnsData(const QString &value)
{
    if (mUpdatingColumnsData)
    {
        return;
    }
    
    mUpdatingColumnsData = true;
    
    ColumnsVector newColumns = ColumnsSerializer::deserialize(value, this);
    setColumns(newColumns);
    
    mUpdatingColumnsData = false;
}

Mode GridView::getMode() const { return mode; }

void GridView::setMode(const Mode &value)
{
    if (mode != value)
    {
        tableModel->setMode(mode);
        mode = value;
        recreateToolBox();
        emit modeChanged(mode);
    }
}

void GridView::setDatasource(ActiveListBase *value)
{
    tableModel->setDatasource(value);
    emit currentRowChanged(nullptr);
}

ActiveListBase *GridView::getDatasource() const
{
    return tableModel->getDatasource();
}

void GridView::clearDatasource() { setDatasource(nullptr); }

void GridView::reloadDatasource() { mUi->tableData->resizeColumnsToContents(); }

void GridView::deleteCurrentRow()
{
    emit removeSelectedRow(mUi->tableData->currentIndex().row());
}

void GridView::onHeaderTableSectionResized(const int &index, const int &oldSize,
                                           const int &newSize)
{
    Q_UNUSED(oldSize)

    int currentSize = mUi->tableData->horizontalHeader()->sectionSize(index);
    if (currentSize != newSize)
        mUi->tableData->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::onDataTableSectionResized(const int &index, const int &oldSize,
                                         const int &newSize)
{
    Q_UNUSED(oldSize)

    int currentSize = mUi->tableHeader->horizontalHeader()->sectionSize(index);
    if (currentSize != newSize)
        mUi->tableHeader->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::resizeVerticalHeader()
{
    int currentSizeHeader = mUi->tableHeader->verticalHeader()->size().width();
    int currentSizeData = mUi->tableData->verticalHeader()->sizeHint().width();
    if (currentSizeHeader != currentSizeData)
    {
        mUi->tableHeader->verticalHeader()->setFixedWidth(currentSizeData);
        QMetaObject::invokeMethod(mUi->tableHeader, "updateGeometries");
    }
}

void GridView::onChangedStaticFilters(Group filters)
{
    staticFilters = filters;
    emit filtersChanged(mergeFilters());
}

void GridView::onChangedSearchBarFilters(Group filters)
{
    searchBarFilters = filters;
    emit filtersChanged(mergeFilters());
}

void GridView::onParentCurrentRowChanged(QObject *object)
{
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
                                             const QModelIndex &previous)
{
    Q_UNUSED(previous)

    emit currentRowChanged(tableModel->getObjectByIndex(current.row()));
}

void GridView::onTableDataVerticalScrollRangeChanged(int min, int max)
{
    mUi->verticalScrollBar->setRange(min, max);
}

void GridView::recreateToolBox()
{
    toolbar->clear();
    createToolbox();
}

void GridView::createToolbox()
{
    QIcon refreshIcon = style()->standardIcon(QStyle::SP_BrowserReload);
    QAction *refresh = new QAction(refreshIcon, "Обновить данные", this);
    connect(refresh, SIGNAL(triggered(bool)), this, SLOT(onRefreshButtonClick()));
    toolbar->addAction(refresh);
    toolbar->addSeparator();
    if (mode == Mode::ReadWrite)
    {
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

void GridView::resizeHeader()
{
    int headerHeight = mUi->tableHeader->horizontalHeader()->height();
    int filterRowHeight = mSearchBarShow ? mUi->tableHeader->rowHeight(0) : 0;
    mUi->tableHeader->setMaximumHeight(headerHeight + filterRowHeight);
}

Group GridView::mergeFilters()
{
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

bool GridView::getSearchBarShow() const { return mSearchBarShow; }

void GridView::setSearchBarShow(bool value)
{
    mSearchBarShow = value;
    resizeHeader();
}

void GridView::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    resizeHeader();
    resizeVerticalHeader();
}

bool GridView::getToolboxVisible() const { return mUi->toolbar->isVisible(); }

void GridView::setToolboxVisible(bool value) { mUi->toolbar->setVisible(value); }

GridView *GridView::getParentGrid() const { return parentGrid; }

void GridView::setParentGrid(GridView *value)
{
    if (parentGrid != value)
    {
        parentGrid = value;
        connect(parentGrid, SIGNAL(currentRowChanged(QObject *)),
                SLOT(onParentCurrentRowChanged(QObject *)));
    }
}

QObject *GridView::getSelectedObject()
{
    return mUi->tableData->getSelectedObject();
}

GridColumn *GridView::getParentIdColumn() const { return parentIdColumn; }

GridColumn *GridView::getIdColumn() const { return idColumn; }
