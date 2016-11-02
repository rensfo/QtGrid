#ifndef HEADERTABLEFILTERWIDGET_H
#define HEADERTABLEFILTERWIDGET_H

#include <QStringListModel>
#include <QWidget>

#include "Filter.h"

namespace Ui {
class HeaderTableFilterWidget;
}

class HeaderTableFilterWidget : public QWidget {
  Q_OBJECT

  Q_FLAGS(Operations)

public:
  explicit HeaderTableFilterWidget(QWidget *parent = 0);
  ~HeaderTableFilterWidget();

  void setMainWidget(QWidget *widget);

  Operations getOperations() const;
  void setOperations(const Operations &value);

  QVariant getData();

  Operation getSelectOperation();

protected slots:
  void onDataChanged();

protected:
  void recreateComboboxItem();

protected:
  Operations operations = Operation::None;
  QStringListModel cbModel;
  QWidget *mainWidget = nullptr;
  bool notUpdate = false;

private:
  Ui::HeaderTableFilterWidget *ui;
};

#endif // HEADERTABLEFILTERWIDGET_H
