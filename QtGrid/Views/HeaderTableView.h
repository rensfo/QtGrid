#ifndef HEADERTABLEVIEW_H
#define HEADERTABLEVIEW_H

#include <QObject>

#include "GridColumn.h"
#include "HeaderTableDelegate.h"
#include "HeaderTableModel.h"
#include "TableViewBase.h"

class HeaderTableView : public TableViewBase {
  Q_OBJECT

public:
  HeaderTableView(QWidget *parent = nullptr);

  void openEditors();
  virtual void appendColumns(const QList<GridColumn *> &value) override;

protected:
  virtual void showEvent(QShowEvent *event) override;

private:
  HeaderTableDelegate *filterDelegate;
  HeaderTableModel *tableModelHeader;
};

#endif // HEADERTABLEVIEW_H
