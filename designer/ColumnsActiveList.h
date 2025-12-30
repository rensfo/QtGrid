#pragma once

#include "QGrid/ActiveListBase.h"
#include "QGrid/GridColumn.h"
#include <QVector>

class ColumnsActiveList : public ActiveListBase
{
    Q_OBJECT

public:
    explicit ColumnsActiveList(QObject *parent = nullptr);
    
    void clear() override;
    size_t count() const override;
    QObject *qobjectValue(size_t index) override;
    size_t qobjectIndexOf(QObject *value) const override;
    
    void setColumns(const QVector<GridColumn*> &columns);
    QVector<GridColumn*> getColumns() const;

    GridColumn *at(size_t index) const;

    void appendColumn(GridColumn *column);
    void removeColumn(size_t index);


private:
    QVector<GridColumn*> mColumns;
};

