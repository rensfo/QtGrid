#ifndef UNIVERSALDELEGATE_H
#define UNIVERSALDELEGATE_H

#include <QItemDelegate>

#include "GridColumn.h"

class UniversalDelegate : public QItemDelegate {
  Q_OBJECT
public:
  UniversalDelegate(QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override;

  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                     const QModelIndex &index) const override;

signals:
  void initDelegate(QObject *, GridColumn *, QWidget *) const;
  void endEdit(const QModelIndex &index, GridColumn *const column,
               const QVariant value) const;
};

#endif // UNIVERSALDELEGATE_H
