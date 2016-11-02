#include "HeaderTableDelegate.h"

#include <QLineEdit>
#include <UniversalTableModel.h>

#include "GridColumn.h"
#include "HeaderTableFilterWidget.h"
#include "HeaderTableModel.h"

HeaderTableDelegate::HeaderTableDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *HeaderTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  auto model = qobject_cast<HeaderTableModel *>(
      const_cast<QAbstractItemModel *>(index.model()));
  GridColumn *column = model->getColumn(index.column());

  HeaderTableFilterWidget *filterWidget = new HeaderTableFilterWidget(parent);
  filterWidget->setMainWidget(new QLineEdit());
  filterWidget->setOperations(column->getOperations());

  return filterWidget;
}

void HeaderTableDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const {
  Q_UNUSED(editor)
  Q_UNUSED(index)
}

void HeaderTableDelegate::setModelData(QWidget *editor,
                                       QAbstractItemModel *model,
                                       const QModelIndex &index) const {
  HeaderTableFilterWidget *filterWidget =
      qobject_cast<HeaderTableFilterWidget *>(editor);
  Filter filter("", filterWidget->getSelectOperation(),
                filterWidget->getData());
  model->setData(index, QVariant::fromValue(filter));
}

void HeaderTableDelegate::updateEditorGeometry(
    QWidget *editor, const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  editor->setGeometry(option.rect);
}
