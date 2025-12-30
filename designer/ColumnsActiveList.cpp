#include "ColumnsActiveList.h"

ColumnsActiveList::ColumnsActiveList(QObject *parent)
    : ActiveListBase(parent)
{
}

void ColumnsActiveList::clear()
{
    mColumns.clear();
    emit cleared();
    emit countChanged();
}

size_t ColumnsActiveList::count() const
{
    return mColumns.size();
}

QObject *ColumnsActiveList::qobjectValue(size_t index)
{
    if (index < 0 || index >= mColumns.size())
        return nullptr;
    
    return mColumns[index];
}

size_t ColumnsActiveList::qobjectIndexOf(QObject *value) const
{
    GridColumn *column = qobject_cast<GridColumn*>(value);
    if (!column)
        return -1;
    
    return mColumns.indexOf(column);
}

void ColumnsActiveList::setColumns(const QVector<GridColumn*> &columns)
{
    mColumns = columns;
    emit countChanged();
}

QVector<GridColumn*> ColumnsActiveList::getColumns() const
{
    return mColumns;
}

GridColumn *ColumnsActiveList::at(size_t index) const
{
    if (index >= static_cast<size_t>(mColumns.size()))
        return nullptr;

    return mColumns[index];
}

void ColumnsActiveList::appendColumn(GridColumn *column)
{
    mColumns.append(column);
    emit inserted(mColumns.size() - 1, 1);
}

void ColumnsActiveList::removeColumn(size_t index)
{
    if (index >= static_cast<size_t>(mColumns.size()))
        return;

    mColumns.removeAt(static_cast<int>(index));
    emit removed(index, 1);
}
