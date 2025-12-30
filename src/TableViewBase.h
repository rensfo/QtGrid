#ifndef TABLEVIEWBASE_H
#define TABLEVIEWBASE_H

#include "QGrid/GridColumn.h"
#include <QTableView>

class TableViewBase : public QTableView {
public:
  explicit TableViewBase(QWidget *parent = nullptr);
  ~TableViewBase();

  QVector<GridColumn *> getColumns() const;
  virtual void setColumns(QVector<GridColumn *> columns);

  virtual void clearColumns();
  virtual void appendColumns(const QVector<GridColumn *> &newColumns);
  virtual void appendColumn(GridColumn *column);
  virtual void removeColumn(GridColumn *column);
  virtual void removeAllColumns();

protected:
  virtual void showEvent(QShowEvent *event) override;
  void hideColumns();

protected:
  QVector<GridColumn *> mColumns;
};

#endif // TABLEVIEWBASE_H
