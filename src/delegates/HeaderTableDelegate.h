#ifndef HEADERTABLEDELEGATE_H
#define HEADERTABLEDELEGATE_H

#include <QItemDelegate>
#include <QObject>

class HeaderTableDelegate : public QItemDelegate {
  Q_OBJECT
public:
  HeaderTableDelegate(QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override;
};

#endif // HEADERTABLEDELEGATE_H
