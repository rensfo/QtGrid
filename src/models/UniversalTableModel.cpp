#include "UniversalTableModel.h"

#include <QDebug>
#include <QMetaProperty>
#include <QtGlobal>

#include "common/NullableBase.h"

UniversalTableModel::UniversalTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    onObjectPropertyChangedMethod = findOnObjectPropertyChangedMethod();
}

int UniversalTableModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);

    if (!datasource)
        return 0;

    return datasource->count();
}

int UniversalTableModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return columns.count();
}

QVariant UniversalTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= datasource->count() || index.column() >= columns.count())
        return QVariant();

    QVariant result;
    QString fieldName = columns.at(index.column())->getFieldName();
    QVariant currentValue;
    if (datasource->qobjectValue(index.row()))
    {
        currentValue = datasource->qobjectValue(index.row())->property(fieldName.toStdString().data());
    }
    /*  datasource->value(index.row())->property(columns.at(index.column())->getFieldName().toStdString().data());
    if (currentValue.canConvert(QMetaType::type("NullableBase"))) {
      if (currentValue.isValid()) {
        NullableBase nullableBaseValue = currentValue.value<NullableBase>();
        if (&nullableBaseValue)
          currentValue = nullableBaseValue.getVariant();
        else
          currentValue = QVariant();
      }
    }*/
    GridColumn *column = columns[index.column()];
    switch (role)
    {
    case Qt::DisplayRole:
    {
        if (!column->getCheckable())
        {
            if (column->getValueFunc() != nullptr)
            {
                if (!currentValue.isNull() && currentValue.isValid())
                    result = column->getValueFunc()(currentValue);
            }
            else
            {
                result = currentValue;
            }
        }
    }
    break;
    case Qt::EditRole:
    {
        result = currentValue;
    }
    break;
    case Qt::CheckStateRole:
    {
        if (column->getCheckable())
            result = currentValue.toBool() ? Qt::Checked : Qt::Unchecked;
    }
    break;
    default:
        break;
    }

    return result;
}

Qt::ItemFlags UniversalTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (columns[index.column()]->getCheckable())
        flags |= Qt::ItemIsUserCheckable;

    if (columns[index.column()]->getEditable())
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool UniversalTableModel::setData(const QModelIndex &index,
                                  const QVariant &value, int role)
{
    Q_UNUSED(value)

    if (index.isValid() && role == Qt::EditRole)
    {
        auto obj = datasource->qobjectValue(index.row());
        if(obj)
        {
            GridColumn *column = columns.at(index.column());
            QString fieldName = column->getFieldName();
            if(!fieldName.isEmpty() && !fieldName.isNull())
            {
                obj->setProperty(fieldName.toStdString().data(), value);
            }
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QVariant UniversalTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section < columns.count())
        {
            GridColumn *column = columns.at(section);
            if (!column->getHeader().isEmpty() && !column->getHeader().isNull())
                return column->getHeader();
        }
        return QVariant();
    }
    else
    {
        return section + 1;
    }
}

ActiveListBase *UniversalTableModel::getDatasource() const
{
    return datasource;
}

void UniversalTableModel::setDatasource(ActiveListBase *value)
{
    if (datasource != value)
    {
        beginResetModel();
        datasource = value;
        endResetModel();

        subscribeToAllObject(value);

        auto conn = connect(value, &ActiveListBase::countChanged,
                            this, &UniversalTableModel::reloadDatasource,
                            Qt::UniqueConnection);
        if (!conn)
        {
            qWarning() << "Failed to connect to countChanged signal";
        }

        conn = connect(value, &ActiveListBase::inserted,
                       this, &UniversalTableModel::onDatasourceInsert,
                       Qt::UniqueConnection);
        if (!conn)
        {
            qWarning() << "Failed to connect to inserted signal";
        }

        conn = connect(value, &ActiveListBase::removed,
                       this, &UniversalTableModel::onDatasourceRemove,
                       Qt::UniqueConnection);
        if (!conn)
        {
            qWarning() << "Failed to connect to removed signal";
        }

        emit datasourceChanged();
    }
}

GridColumn *UniversalTableModel::getColumn(int index) const
{
    return columns.at(index);
}

QObject *UniversalTableModel::getObjectByIndex(int index)
{
    return datasource->qobjectValue(index);
}

QMetaMethod UniversalTableModel::findOnObjectPropertyChangedMethod()
{
    QMetaMethod result;
    for (int m = 0; m < this->metaObject()->methodCount(); m++)
    {
        QMetaMethod method = this->metaObject()->method(m);
        if (QString(method.name()) == "onObjectPropertyChanged" &&
            method.parameterCount() == 0)
        {
            result = method;
        }
    }

    return result;
}

QString UniversalTableModel::getPropertyName(QObject *sender,
                                             int senderSignalIndex)
{
    QMetaMethod senderSignal = sender->metaObject()->method(senderSignalIndex);
    QMetaProperty senderProperty;
    const QMetaObject *senderMeta = sender->metaObject();
    for (int i = 0; i < senderMeta->propertyCount(); i++)
    {
        if (senderMeta->property(i).hasNotifySignal())
        {
            if (senderMeta->property(i).notifySignal() == senderSignal)
            {
                senderProperty = senderMeta->property(i);
                break;
            }
        }
    }

    return QString(senderProperty.name());
}

void UniversalTableModel::subscribeToAllObject(ActiveListBase *value)
{
    if (!value)
        return;

    for (int i = 0; i < value->count(); ++i)
    {
        QObject *obj = value->qobjectValue(i);
        for (auto col : columns)
        {
            int propertyIndex = obj->metaObject()->indexOfProperty(
                col->getFieldName().toStdString().data());
            if (propertyIndex > -1)
            {
                if (obj->metaObject()->property(propertyIndex).hasNotifySignal())
                {
                    connect(obj,
                            obj->metaObject()->property(propertyIndex).notifySignal(),
                            this, onObjectPropertyChangedMethod);
                }
            }
        }
    }
}

void UniversalTableModel::setFilters(QMap<QString, QVariant> value)
{
    filters = value;
}

void UniversalTableModel::reloadDatasource()
{
    beginResetModel();
    endResetModel();

    emit datasourceChanged();

    // subscribeToAllObject();
}

void UniversalTableModel::onDatasourceCountChanged() {}

void UniversalTableModel::onDatasourceInsert(size_t index, size_t count)
{
    int startIndex = static_cast<int>(index);
    int endIndex = static_cast<int>(index + count - 1);

    beginInsertRows(QModelIndex(), startIndex, endIndex);
    insertRows(static_cast<int>(index), static_cast<int>(count));
    endInsertRows();

    emit rowsInserted();

    // TODO subscribe to new objects
}

void UniversalTableModel::onDatasourceRemove(size_t index, size_t count)
{
    int startIndex = static_cast<int>(index);
    int endIndex = static_cast<int>(index + count - 1);
    beginRemoveRows(QModelIndex(), startIndex, endIndex);
    removeRows(static_cast<int>(index), static_cast<int>(count));
    endRemoveRows();

    emit rowsRemoved();

    // TODO unsubscribe from removed objects
}

void UniversalTableModel::onObjectPropertyChanged()
{
    QObject *sender = this->sender();
    if (sender)
    {
        QString senderPropertyName = getPropertyName(sender, senderSignalIndex());
        int rowIndex = datasource->qobjectIndexOf(sender);

        if (rowIndex < 0)
            return;

        GridColumn *col = nullptr;
        for (GridColumn *c : columns)
        {
            if (c->getFieldName() == senderPropertyName)
            {
                col = c;
                break;
            }
        }

        int columnIndex = columns.indexOf(col);
        QModelIndex modelIndex = index(rowIndex, columnIndex);
        emit dataChanged(modelIndex, modelIndex);
    }
}

QVector<GridColumn *> UniversalTableModel::getColumns() const { return columns; }

void UniversalTableModel::setColumns(const QVector<GridColumn *> &value)
{
    beginResetModel();

    columns = value;

    for (auto column : value)
        if (column->getIsId())
            idColumn = column;

    endResetModel();
}

Mode UniversalTableModel::getMode() const { return mode; }

void UniversalTableModel::setMode(const Mode &value) { mode = value; }
