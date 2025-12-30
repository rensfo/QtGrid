#pragma once

#include <QString>
#include <QVector>
#include <QObject>

class GridColumn;
typedef QVector<GridColumn *> ColumnsVector;

class ColumnsSerializer
{
public:
    static QString serialize(const ColumnsVector &columns);
    static ColumnsVector deserialize(const QString &data, QObject *parent);
};

