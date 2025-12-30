#include "ColumnsPropertyEditor.h"
#include "ColumnsActiveList.h"

#include <QtDesigner/QDesignerTaskMenuExtension>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerPropertySheetExtension>
#include <QAction>
#include <QLineEdit>
#include <QSpinBox>
#include <QApplication>
#include <QEvent>

ColumnsPropertyEditor::ColumnsPropertyEditor(QDesignerFormEditorInterface *core, QWidget *parent)
    : QWidget(parent)
    , mCore(core)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    mEditButton = new QPushButton("Edit columns...", this);
    mEditButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(mEditButton);
    
    connect(mEditButton, &QPushButton::clicked, this, &ColumnsPropertyEditor::onEditColumns);
}

void ColumnsPropertyEditor::setGridView(GridView *gridView)
{
    mGridView = gridView;
}

void ColumnsPropertyEditor::openEditor()
{
    onEditColumns();
}

void ColumnsPropertyEditor::onEditColumns()
{
    if (!mGridView)
        return;
    
    if (!mDialog)
    {
        mDialog = new QDialog(this);
        mDialog->setWindowTitle("Edit columns");
        mDialog->setMinimumSize(800, 600);
        
        auto *dialogLayout = new QVBoxLayout(mDialog);
        
        mEditorGridView = new GridView(mDialog);
        mEditorGridView->setHeaderVisible(false);
        mEditorGridView->setToolboxVisible(true);
        mEditorGridView->setSearchBarShow(false);
        dialogLayout->addWidget(mEditorGridView);
        
        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, mDialog);

        connect(buttonBox, &QDialogButtonBox::accepted, mDialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, mDialog, &QDialog::reject);
        
        dialogLayout->addWidget(buttonBox);
    }

    connect(mEditorGridView, &GridView::insertRow, this, &ColumnsPropertyEditor::onInsertRow);
    connect(mEditorGridView, &GridView::removeSelectedRow, this, &ColumnsPropertyEditor::onRemoveRow);

    ColumnsVector columns = mGridView->getColumns();

    mActiveList = new ColumnsActiveList(mEditorGridView);
    mActiveList->setColumns(columns);

    QVector<GridColumn*> displayColumns;
    auto *headerColumn = new GridColumn(mEditorGridView);
    headerColumn->setHeader("Header");
    headerColumn->setFieldName("header");
    headerColumn->setIndex(0);
    headerColumn->setEditable(true);
    headerColumn->setEditDelegate<QLineEdit>();
    headerColumn->setDirectEdit(true);
    displayColumns.append(headerColumn);
    
    auto *fieldNameColumn = new GridColumn(mEditorGridView);
    fieldNameColumn->setHeader("Field name");
    fieldNameColumn->setFieldName("fieldName");
    fieldNameColumn->setIndex(1);
    fieldNameColumn->setEditable(true);
    fieldNameColumn->setEditDelegate<QLineEdit>();
    fieldNameColumn->setDirectEdit(true);
    displayColumns.append(fieldNameColumn);
    
    auto *indexColumn = new GridColumn(mEditorGridView);
    indexColumn->setHeader("Index");
    indexColumn->setFieldName("index");
    indexColumn->setIndex(2);
    indexColumn->setEditable(true);
    indexColumn->setEditDelegate<QSpinBox>();
    indexColumn->setDirectEdit(true);
    displayColumns.append(indexColumn);
    
    auto *visibleColumn = new GridColumn(mEditorGridView);
    visibleColumn->setHeader("Visible");
    visibleColumn->setFieldName("visible");
    visibleColumn->setIndex(3);
    visibleColumn->setCheckable(true);
    visibleColumn->setDirectEdit(true);
    displayColumns.append(visibleColumn);
    
    auto *editableColumn = new GridColumn(mEditorGridView);
    editableColumn->setHeader("Editable");
    editableColumn->setFieldName("editable");
    editableColumn->setIndex(4);
    editableColumn->setCheckable(true);
    editableColumn->setDirectEdit(true);
    displayColumns.append(editableColumn);
    
    auto *checkableColumn = new GridColumn(mEditorGridView);
    checkableColumn->setHeader("Checkable");
    checkableColumn->setFieldName("checkable");
    checkableColumn->setIndex(5);
    checkableColumn->setCheckable(true);
    checkableColumn->setDirectEdit(true);
    displayColumns.append(checkableColumn);
    
    auto *isIdColumn = new GridColumn(mEditorGridView);
    isIdColumn->setHeader("ID column");
    isIdColumn->setFieldName("isId");
    isIdColumn->setIndex(6);
    isIdColumn->setCheckable(true);
    isIdColumn->setDirectEdit(true);
    displayColumns.append(isIdColumn);
    
    auto *isParentIdColumn = new GridColumn(mEditorGridView);
    isParentIdColumn->setHeader("Parent ID");
    isParentIdColumn->setFieldName("isParentId");
    isParentIdColumn->setIndex(7);
    isParentIdColumn->setCheckable(true);
    isParentIdColumn->setDirectEdit(true);
    displayColumns.append(isParentIdColumn);
    
    auto *showInStaticFilterColumn = new GridColumn(mEditorGridView);
    showInStaticFilterColumn->setHeader("In static filter");
    showInStaticFilterColumn->setFieldName("showInStaticFilter");
    showInStaticFilterColumn->setIndex(8);
    showInStaticFilterColumn->setCheckable(true);
    showInStaticFilterColumn->setDirectEdit(true);
    displayColumns.append(showInStaticFilterColumn);
    
    auto *showInSearchBarColumn = new GridColumn(mEditorGridView);
    showInSearchBarColumn->setHeader("In search");
    showInSearchBarColumn->setFieldName("showInSearchBar");
    showInSearchBarColumn->setIndex(9);
    showInSearchBarColumn->setCheckable(true);
    showInSearchBarColumn->setDirectEdit(true);
    displayColumns.append(showInSearchBarColumn);
    
    mEditorGridView->setColumns(displayColumns);
    mEditorGridView->setDatasource(mActiveList);
    
    if (mDialog->exec() == QDialog::Accepted)
    {
        QVector<GridColumn*> newColumns = mActiveList->getColumns();
        mGridView->setColumns(newColumns);

        // emit columnsChanged();
    }
}

void ColumnsPropertyEditor::onInsertRow()
{
    auto *newColumn = new GridColumn();
    newColumn->setHeader("Header");
    newColumn->setFieldName("FieldName");
    newColumn->setIndex(mActiveList->getColumns().size());
    newColumn->setEditable(false);
    newColumn->setCheckable(false);
    newColumn->setIsId(false);
    newColumn->setIsParentId(false);
    newColumn->setShowInStaticFilter(false);
    newColumn->setShowInSearchBar(false);

    mActiveList->appendColumn(newColumn);
}

void ColumnsPropertyEditor::onRemoveRow(size_t index)
{
    auto column = mActiveList->at(index);
    mActiveList->removeColumn(index);
    delete column;
}

ColumnsTaskMenu::ColumnsTaskMenu(GridView *gridView, QDesignerFormEditorInterface *core, QObject *parent)
    : QObject(parent)
    , mGridView(gridView)
    , mCore(core)
    , mEditColumnsAction(nullptr)
{
    mEditColumnsAction = new QAction(tr("Edit columns..."), this);
    connect(mEditColumnsAction, &QAction::triggered, this, &ColumnsTaskMenu::editColumns);
}

QList<QAction *> ColumnsTaskMenu::taskActions() const
{
    return QList<QAction *>() << mEditColumnsAction;
}

void ColumnsTaskMenu::editColumns()
{
    if (!mGridView)
        return;
    
    ColumnsPropertyEditor editor(mCore, nullptr);
    editor.setGridView(mGridView);
    editor.openEditor();
}

ColumnsTaskMenuFactory::ColumnsTaskMenuFactory(QDesignerFormEditorInterface *core, QExtensionManager *parent)
    : QExtensionFactory(parent)
    , mCore(core)
{
}

QObject *ColumnsTaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return nullptr;
    
    if (GridView *gridView = qobject_cast<GridView*>(object))
    {
        return new ColumnsTaskMenu(gridView, mCore, parent);
    }
    
    return nullptr;
}

