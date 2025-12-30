#ifndef HEADERTABLEVIEW_H
#define HEADERTABLEVIEW_H

#include <QObject>

#include "QGrid/GridColumn.h"
#include "delegates/HeaderTableDelegate.h"
#include "models/HeaderTableModel.h"
#include "TableViewBase.h"

class HeaderTableView : public TableViewBase
{
    Q_OBJECT

public:
  HeaderTableView(QWidget *parent = nullptr);

  void openEditors();
  virtual void appendColumns(const QVector<GridColumn *> &value) override;
  virtual void setColumns(QVector<GridColumn *> columns) override;

  HeaderTableModel *getHeaderModel() const;

  protected:
  virtual void showEvent(QShowEvent *event) override;

private:
  HeaderTableDelegate *filterDelegate;
  HeaderTableModel *tableModelHeader;
};

#endif // HEADERTABLEVIEW_H
