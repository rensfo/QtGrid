#ifndef GRIDCOLUMN_H
#define GRIDCOLUMN_H

#include <QObject>
#include <QVariant>
#include <functional>

#include "Operation.h"

class FactoryBase {
public:
  virtual ~FactoryBase() {}
  virtual QWidget *createNewInstance() const = 0;
};

template <class T> class Factory : public FactoryBase {
public:
  Factory<T>() : FactoryBase() {}
  virtual ~Factory<T>() override {}
  QWidget *createNewInstance() const { return new T(); }
};

class GridColumn : public QObject {
  Q_OBJECT

  Q_FLAGS(Operations)

public:
  explicit GridColumn(QObject *parent = nullptr);
  GridColumn(const GridColumn &value, QObject *parent = nullptr);

  QString getHeader() const;
  void setHeader(const QString &value);

  QString getFieldName() const;
  void setFieldName(const QString &value);

  bool getCheckable() const;
  void setCheckable(bool value);

  bool getVisible() const;
  void setVisible(bool value);

  int getIndex() const;
  void setIndex(int value);

  bool getIsId() const;
  void setIsId(bool value);

  bool getShowInStaticFilter() const;
  void setShowInStaticFilter(bool value);

  QVariant getDefaultValue() const;
  void setDefaultValue(const QVariant &value);

  template <class T> void setEditDelegate() {
    if (factoryEditDelegate != nullptr)
      delete factoryEditDelegate;

    this->factoryEditDelegate = new Factory<T>();
  }

  bool hasEditDelegate();

  QWidget *createNewEditDelegate();

  bool getEditable() const;
  void setEditable(bool value);

  std::function<QVariant(QVariant)> getValueFunc() const;
  void setValueFunc(const std::function<QVariant(QVariant)> &value);

  bool getShowDelegateInView() const;
  void setShowDelegateInView(bool value);

  Operations getOperations() const;
  void setOperations(const Operations &value);

  bool getShowInSearchBar() const;
  void setShowInSearchBar(bool value);

  FactoryBase *getFactoryEditDelegate() const;

  bool getIsParentId() const;
  void setIsParentId(bool value);

  GridColumn &operator=(const GridColumn &value);

private:
  QString header;
  QString fieldName;
  bool editable = false;
  bool checkable = false;
  bool visible = true;
  int index;
  bool isId = false;
  bool isParentId = false;
  bool showInStaticFilter = false;
  bool showInSearchBar = false;
  QVariant defaultValue;
  FactoryBase *factoryEditDelegate = nullptr;
  std::function<QVariant(QVariant)> valueFunc;
  bool showDelegateInView = false;
  Operations operations = Operation::None;
};

#endif // GRIDCOLUMN_H
