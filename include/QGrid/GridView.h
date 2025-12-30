#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QFrame>
#include <QToolBar>

#include "models/HeaderTableModel.h"
#include "models/UniversalTableModel.h"

#include "QGrid/GridViewExport.h"

namespace Ui
{
    class GridView;
}

Q_DECLARE_METATYPE(GridColumn);

typedef QVector<GridColumn *> ColumnsVector;
Q_DECLARE_METATYPE(ColumnsVector);

class QGRID_EXPORT GridView : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(bool headerVisible READ getHeaderVisible WRITE setHeaderVisible)
    Q_PROPERTY(QString headerText READ getHeaderText WRITE setHeaderText)
    Q_PROPERTY(bool toolboxVisible READ getToolboxVisible WRITE setToolboxVisible)
    Q_PROPERTY(bool searchbarVisible READ getSearchBarShow WRITE setSearchBarShow)
    Q_PROPERTY(ColumnsVector columns READ getColumns WRITE setColumns DESIGNABLE true)

public:
    explicit GridView(QWidget *parent = 0);
    ~GridView();

    bool getHeaderVisible() const;
    void setHeaderVisible(bool value);

    QString getHeaderText() const;
    void setHeaderText(const QString &value);

    bool getToolboxVisible() const;
    void setToolboxVisible(bool value);

    bool getSearchBarShow() const;
    void setSearchBarShow(bool value);

    ColumnsVector getColumns() const;
    void setColumns(const ColumnsVector &value);

    Mode getMode() const;
    void setMode(const Mode &value);

    void setDatasource(ActiveListBase *value);
    ActiveListBase *getDatasource() const;
    void clearDatasource();

    GridColumn *getIdColumn() const;
    GridColumn *getParentIdColumn() const;

    GridView *getParentGrid() const;
    void setParentGrid(GridView *value);

    QObject *getSelectedObject();

public slots:
    void reloadDatasource();
    void deleteCurrentRow();

protected slots:
    void onHeaderTableSectionResized(const int &index, const int &oldSize,
                                     const int &newSize);
    void onDataTableSectionResized(const int &index, const int &oldSize,
                                   const int &newSize);
    void resizeVerticalHeader();
    void onChangedStaticFilters(Group filters);
    void onChangedSearchBarFilters(Group filters);
    void onParentCurrentRowChanged(QObject *object);
    void onRefreshButtonClick();
    void onTableDataSelectedRowChanged(const QModelIndex &current,
                                       const QModelIndex &previous);

    void onTableDataVerticalScrollRangeChanged(int min, int max);

protected:
    void recreateToolBox();
    void createToolbox();
    void resizeHeader();
    Group *converFilters(const QMap<QString, QVariant> &filters);
    Group mergeFilters();
    void showEvent(QShowEvent *event);

signals:
    void modeChanged(Mode);
    void initDelegate(QObject *, GridColumn *, QWidget *) const;
    void endEdit(const QModelIndex &index, GridColumn *const column,
                 const QVariant value) const;
    void removeSelectedRow(size_t row);
    void insertRow();
    void filtersChanged(Group);
    void refresh(Group);
    void currentRowChanged(QObject *value);

private:
    Ui::GridView *mUi;

    bool mSearchBarShow = false;

    QVector<GridColumn *> mColumns;
    Mode mode = Mode::ReadWrite;
    UniversalTableModel *tableModel = nullptr;
    HeaderTableModel *tableModelHeader = nullptr;
    QToolBar *toolbar = nullptr;

    Group staticFilters;
    Group searchBarFilters;
    Group parentFilter;
    GridColumn *idColumn = nullptr;
    GridColumn *parentIdColumn = nullptr;
    GridView *parentGrid = nullptr;
};

#endif // GRIDVIEW_H
