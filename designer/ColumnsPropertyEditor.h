#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerTaskMenuExtension>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QAction>
#include <QList>

#include "QGrid/GridView.h"
#include "QGrid/GridColumn.h"
#include "ColumnsActiveList.h"

class ColumnsPropertyEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColumnsPropertyEditor(QDesignerFormEditorInterface *core, QWidget *parent);
    void setGridView(GridView *gridView);
    GridView *getGridView() const { return mGridView; }

    void openEditor();

signals:
    void columnsChanged();

private slots:
    void onEditColumns();

    void onInsertRow();
    void onRemoveRow(size_t index);

private:
    QDesignerFormEditorInterface *mCore = nullptr;
    GridView *mGridView = nullptr;
    ColumnsActiveList *mActiveList = nullptr;
    QPushButton *mEditButton = nullptr;
    QDialog *mDialog = nullptr;
    GridView *mEditorGridView = nullptr;
};

class ColumnsTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)

public:
    explicit ColumnsTaskMenu(GridView *gridView, QDesignerFormEditorInterface *core, QObject *parent = nullptr);

    QList<QAction *> taskActions() const override;

private slots:
    void editColumns();

private:
    GridView *mGridView;
    QDesignerFormEditorInterface *mCore;
    QAction *mEditColumnsAction;
};

class ColumnsTaskMenuFactory : public QExtensionFactory
{
    Q_OBJECT

public:
    explicit ColumnsTaskMenuFactory(QDesignerFormEditorInterface *core, QExtensionManager *parent = nullptr);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const override;

private:
    QDesignerFormEditorInterface *mCore;
};

