#include "UniversalDelegate.h"
#include "../Models/UniversalTableModel.h"

#include <QMetaProperty>

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QPainter>

UniversalDelegate::UniversalDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *UniversalDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
  Q_UNUSED(option)

  auto model = qobject_cast<UniversalTableModel *>(
      const_cast<QAbstractItemModel *>(index.model()));
  GridColumn *column = model->getColumn(index.column());
  auto object = model->getObjectByIndex(index.row());
  QWidget *widget = column->createNewEditDelegate();
  widget->setParent(parent);
  emit initDelegate(object, column, widget);
  return widget;
}

void UniversalDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const {
  const char *userPropertyName = editor->metaObject()->userProperty().name();
  QVariant data = index.model()->data(index, Qt::EditRole);
  auto model = static_cast<const UniversalTableModel *>(index.model());
  GridColumn *column = model->getColumn(index.column());
  if (column->getValueFunc() != nullptr)
    data = column->getValueFunc()(data);

  editor->setProperty(userPropertyName, data);
}

void UniversalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
  auto universalModel = static_cast<const UniversalTableModel *>(model);
  GridColumn *const column = universalModel->getColumn(index.column());
  QComboBox *cmbx = qobject_cast<QComboBox *>(editor);
  if (cmbx) {
    QVariant value = QVariant(cmbx->currentIndex());
    emit endEdit(index, column, value);
  } else {
    const char *userPropertyName = editor->metaObject()->userProperty().name();
    emit endEdit(index, column, editor->property(userPropertyName));
  }
}

void UniversalDelegate::updateEditorGeometry(QWidget *editor,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  editor->setGeometry(option.rect);
}

void UniversalDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {
  auto model = qobject_cast<UniversalTableModel *>(
      const_cast<QAbstractItemModel *>(index.model()));
  GridColumn *column = model->getColumn(index.column());
  if (!column->getShowDelegateInView()) {
    QItemDelegate::paint(painter, option, index);
    return;
  }

  QWidget *widget = createEditor(nullptr, option, index);
  setEditorData(widget, index);

  widget->resize(option.rect.size());
#if QT_VERSION >= 0x050000
  painter->drawPixmap(option.rect, widget->grab());
#else
  QPixmap pixmap(option.rect.size());
  widget->render(&pixmap);
  painter->drawPixmap(option.rect, pixmap);
#endif

  widget->deleteLater();
}
