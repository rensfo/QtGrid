#include "GridColumn.h"

GridColumn::GridColumn(QObject *parent) : QObject(parent) {}

GridColumn::GridColumn(const GridColumn &value, QObject *parent)
    : GridColumn(parent) {
  operator=(value);
}

QString GridColumn::getHeader() const { return header; }

void GridColumn::setHeader(const QString &value) { header = value; }

QString GridColumn::getFieldName() const { return fieldName; }

void GridColumn::setFieldName(const QString &value) {
  fieldName = value;
  if (header.isEmpty())
    header = fieldName;
}

bool GridColumn::getCheckable() const { return checkable; }

void GridColumn::setCheckable(bool value) { checkable = value; }

bool GridColumn::getVisible() const { return visible; }

void GridColumn::setVisible(bool value) { visible = value; }

int GridColumn::getIndex() const { return index; }

void GridColumn::setIndex(int value) { index = value; }

bool GridColumn::getIsId() const { return isId; }

void GridColumn::setIsId(bool value) { isId = value; }

bool GridColumn::getShowInStaticFilter() const { return showInStaticFilter; }

void GridColumn::setShowInStaticFilter(bool value) {
  showInStaticFilter = value;
}

QVariant GridColumn::getDefaultValue() const { return defaultValue; }

void GridColumn::setDefaultValue(const QVariant &value) {
  defaultValue = value;
}

bool GridColumn::hasEditDelegate() { return factoryEditDelegate != nullptr; }

QWidget *GridColumn::createNewEditDelegate() {
  return factoryEditDelegate->createNewInstance();
}

bool GridColumn::getEditable() const { return editable; }

void GridColumn::setEditable(bool value) { editable = value; }

std::function<QVariant(QVariant)> GridColumn::getValueFunc() const {
  return valueFunc;
}

void GridColumn::setValueFunc(const std::function<QVariant(QVariant)> &value) {
  valueFunc = value;
}

bool GridColumn::getShowDelegateInView() const { return showDelegateInView; }

void GridColumn::setShowDelegateInView(bool value) {
  showDelegateInView = value;
}

Operations GridColumn::getOperations() const { return operations; }

void GridColumn::setOperations(const Operations &value) {
  if (operations != value)
    operations = value;
}

bool GridColumn::getShowInSearchBar() const { return showInSearchBar; }

void GridColumn::setShowInSearchBar(bool value) { showInSearchBar = value; }

FactoryBase *GridColumn::getFactoryEditDelegate() const {
  return factoryEditDelegate;
}

GridColumn &GridColumn::operator=(const GridColumn &value) {
  header = value.getHeader();
  fieldName = value.getFieldName();
  editable = value.getEditable();
  checkable = value.getCheckable();
  visible = value.getVisible();
  index = value.getIndex();
  isId = value.getIsId();
  showInStaticFilter = value.getShowInStaticFilter();
  showInSearchBar = value.getShowInSearchBar();
  defaultValue = value.getDefaultValue();
  factoryEditDelegate = value.getFactoryEditDelegate();
  valueFunc = value.getValueFunc();
  showDelegateInView = value.getShowDelegateInView();
  operations = value.getOperations();

  return *this;
}

bool GridColumn::getIsParentId() const { return isParentId; }

void GridColumn::setIsParentId(bool value) { isParentId = value; }
