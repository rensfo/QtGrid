#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QFrame>
#include <QToolBar>

#include "HeaderTableModel.h"
#include "UniversalTableModel.h"

namespace Ui {
class GridView;
}

class GridView : public QFrame {
  Q_OBJECT

public:
  explicit GridView(QWidget *parent = 0);
  ~GridView();

  QList<GridColumn *> getColumns() const;
  void setColumns(const QList<GridColumn *> &value);

  Mode getMode() const;
  void setMode(const Mode &value);

  void setDatasource(ActiveListBase *value);
  ActiveListBase *getDatasource() const;
  void clearDatasource();

  bool getSearchBarShow() const;
  void setSearchBarShow(bool value);

  QString getTableName() const;
  void setTableName(const QString &value);

  GridColumn *getIdColumn() const;
  GridColumn *getParentIdColumn() const;

  GridView *getParentGrid() const;
  void setParentGrid(GridView *value);

  QObject *getSelectedObject();

  bool getToolboxVisible() const;
  void setToolboxVisible(bool value);

  bool getGridHeaderVisible() const;
  void setGridHeaderVisible(bool value);

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
  void removeSelectedRow(int row);
  void insertRow();
  void filtersChanged(Group);
  void refresh(Group);
  void currentRowChanged(QObject *value);

private:
  Ui::GridView *ui;
  QList<GridColumn *> columns;
  Mode mode = Mode::ReadWrite;
  UniversalTableModel *tableModel = nullptr;
  HeaderTableModel *tableModelHeader = nullptr;
  QToolBar *toolbar = nullptr;
  bool searchBarShow = false;
  Group staticFilters;
  Group searchBarFilters;
  Group parentFilter;
  QString tableName;
  GridColumn *idColumn = nullptr;
  GridColumn *parentIdColumn = nullptr;
  GridView *parentGrid = nullptr;
};

#endif // GRIDVIEW_H
