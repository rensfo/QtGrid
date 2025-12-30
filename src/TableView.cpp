#include "TableView.h"

#include <QDebug>
#include <QHeaderView>
#include <QLabel>

TableView::TableView(QWidget *parent) : TableViewBase(parent)
{
    universalDelegate = new UniversalDelegate(this);
    setItemDelegate(universalDelegate);
    connect(universalDelegate,
            SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)), this,
            SIGNAL(initDelegate(QObject *, GridColumn *, QWidget *)));
    connect(universalDelegate, &UniversalDelegate::endEdit, this, &TableView::onEndEdit);

    //  horizontalHeader()->setSectionsMovable(true);
}

TableView::~TableView() {}

Mode TableView::getMode() const { return mode; }

void TableView::setMode(const Mode &value)
{
    if (mode != value)
    {
        mode = value;
        emit modeChanged(mode);
    }
}

QObject *TableView::getSelectedObject()
{
    int currentRow = currentIndex().row();
    if (currentRow == -1)
        return nullptr;

    auto m = qobject_cast<UniversalTableModel *>(model());

    return m->getObjectByIndex(currentRow);
}

void TableView::onEndEdit(const QModelIndex &index, GridColumn *const column, const QVariant value) const
{
    if(column->getDirectEdit())
    {
        model()->setData(index, value, Qt::EditRole);
    }
    else
    {
        emit endEdit(index, column, value);
    }
}