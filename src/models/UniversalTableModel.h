#ifndef UNIVERSALTABLEMODEL_H
#define UNIVERSALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMetaProperty>
#include <QSignalMapper>

#include "QGrid/ActiveListBase.h"
#include "QGrid/GridColumn.h"

enum class Mode { Read, ReadWrite };

class UniversalTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  UniversalTableModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &index) const;
  int columnCount(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);

  Mode getMode() const;
  void setMode(const Mode &value);
  QVector<GridColumn *> getColumns() const;
  void setColumns(const QVector<GridColumn *> &value);

  ActiveListBase *getDatasource() const;
  void setDatasource(ActiveListBase *value);

  GridColumn *getColumn(int index) const;

  QObject *getObjectByIndex(int index);

protected:
  QMetaMethod findOnObjectPropertyChangedMethod();
  QString getPropertyName(QObject *sender, int senderSignalIndex);

private:
  void subscribeToAllObject(ActiveListBase *value);

signals:
  void datasourceChanged();
  void rowsInserted();

  void rowsRemoved();
public slots:
  void setFilters(QMap<QString, QVariant> value);
  void reloadDatasource();

protected slots:
  void onDatasourceCountChanged();
  void onDatasourceInsert(size_t index, size_t count);
  void onDatasourceRemove(size_t index, size_t count);
  void onObjectPropertyChanged();

private:
  QString table;
  QVector<GridColumn *> columns;
  QMap<QString, QVariant> filters;
  Mode mode;
  GridColumn *idColumn = nullptr;
  ActiveListBase *datasource = nullptr;
  QMetaMethod onObjectPropertyChangedMethod;
};

#endif // UNIVERSALTABLEMODEL_H
