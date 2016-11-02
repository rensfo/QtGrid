#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "TableViewBase.h"
#include "UniversalDelegate.h"
#include "UniversalTableModel.h"

class TableView : public TableViewBase {
  Q_OBJECT

public:
  explicit TableView(QWidget *parent = nullptr);
  ~TableView();

  Mode getMode() const;
  void setMode(const Mode &value);

  QObject *getSelectedObject();

signals:
  void modeChanged(Mode);
  void initDelegate(QObject *, GridColumn *, QWidget *) const;
  void endEdit(const QModelIndex &index, GridColumn *const column,
               const QVariant value) const;

private:
  Mode mode;
  UniversalDelegate *universalDelegate;
};

#endif // TABLEVIEW_H
