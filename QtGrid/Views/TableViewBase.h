#ifndef TABLEVIEWBASE_H
#define TABLEVIEWBASE_H

#include <GridColumn.h>
#include <QTableView>

class TableViewBase : public QTableView {
public:
  explicit TableViewBase(QWidget *parent = nullptr);
  ~TableViewBase();

  QList<GridColumn *> getColumns() const;
  virtual void appendColumns(const QList<GridColumn *> &newColumns);
  virtual void appendColumn(GridColumn *column);
  virtual void removeColumn(GridColumn *column);
  virtual void removeAllColumns();

protected:
  virtual void showEvent(QShowEvent *event) override;
  void hideColumns();

protected:
  QList<GridColumn *> columns;
};

#endif // TABLEVIEWBASE_H
