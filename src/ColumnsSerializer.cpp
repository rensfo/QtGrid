#include "ColumnsSerializer.h"
#include "QGrid/GridColumn.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>

QString ColumnsSerializer::serialize(const ColumnsVector &columns)
{
    QJsonArray columnsArray;
    
    for (const GridColumn *column : columns)
    {
        QJsonObject columnObj;
        columnObj["header"] = column->getHeader();
        columnObj["fieldName"] = column->getFieldName();
        columnObj["checkable"] = column->getCheckable();
        columnObj["visible"] = column->getVisible();
        columnObj["index"] = column->getIndex();
        columnObj["isId"] = column->getIsId();
        columnObj["isParentId"] = column->getIsParentId();
        columnObj["editable"] = column->getEditable();
        columnObj["showInStaticFilter"] = column->getShowInStaticFilter();
        columnObj["showInSearchBar"] = column->getShowInSearchBar();
        
        QVariant defaultValue = column->getDefaultValue();
        if (defaultValue.isValid())
        {
            columnObj["defaultValue"] = QJsonValue::fromVariant(defaultValue);
        }
        
        columnObj["directEdit"] = column->getDirectEdit();
        columnObj["showDelegateInView"] = column->getShowDelegateInView();
        columnObj["operations"] = static_cast<int>(column->getOperations());
        
        columnsArray.append(columnObj);
    }
    
    QJsonDocument doc(columnsArray);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return QString::fromLatin1(jsonData.toBase64());
}

ColumnsVector ColumnsSerializer::deserialize(const QString &data, QObject *parent)
{
    ColumnsVector columns;
    
    if (data.isEmpty())
    {
        return columns;
    }
    
    QByteArray jsonData = QByteArray::fromBase64(data.toLatin1());
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
    
    if (error.error != QJsonParseError::NoError || !doc.isArray())
    {
        return columns;
    }
    
    QJsonArray columnsArray = doc.array();
    
    for (const QJsonValue &columnValue : columnsArray)
    {
        if (!columnValue.isObject())
        {
            continue;
        }
        
        QJsonObject columnObj = columnValue.toObject();
        GridColumn *column = new GridColumn(parent);
        
        if (columnObj.contains("header"))
        {
            column->setHeader(columnObj["header"].toString());
        }
        if (columnObj.contains("fieldName"))
        {
            column->setFieldName(columnObj["fieldName"].toString());
        }
        if (columnObj.contains("checkable"))
        {
            column->setCheckable(columnObj["checkable"].toBool());
        }
        if (columnObj.contains("visible"))
        {
            column->setVisible(columnObj["visible"].toBool());
        }
        if (columnObj.contains("index"))
        {
            column->setIndex(columnObj["index"].toInt());
        }
        if (columnObj.contains("isId"))
        {
            column->setIsId(columnObj["isId"].toBool());
        }
        if (columnObj.contains("isParentId"))
        {
            column->setIsParentId(columnObj["isParentId"].toBool());
        }
        if (columnObj.contains("editable"))
        {
            column->setEditable(columnObj["editable"].toBool());
        }
        if (columnObj.contains("showInStaticFilter"))
        {
            column->setShowInStaticFilter(columnObj["showInStaticFilter"].toBool());
        }
        if (columnObj.contains("showInSearchBar"))
        {
            column->setShowInSearchBar(columnObj["showInSearchBar"].toBool());
        }
        if (columnObj.contains("defaultValue"))
        {
            QVariant defaultValue = columnObj["defaultValue"].toVariant();
            column->setDefaultValue(defaultValue);
        }
        if (columnObj.contains("directEdit"))
        {
            column->setDirectEdit(columnObj["directEdit"].toBool());
        }
        if (columnObj.contains("showDelegateInView"))
        {
            column->setShowDelegateInView(columnObj["showDelegateInView"].toBool());
        }
        if (columnObj.contains("operations"))
        {
            column->setOperations(static_cast<Operations>(columnObj["operations"].toInt()));
        }
        
        columns.append(column);
    }
    
    return columns;
}

