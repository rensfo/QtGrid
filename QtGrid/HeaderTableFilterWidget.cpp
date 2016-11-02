#include "HeaderTableFilterWidget.h"
#include "ui_HeaderTableFilterWidget.h"

#include <QKeyEvent>
#include <QMetaProperty>

HeaderTableFilterWidget::HeaderTableFilterWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::HeaderTableFilterWidget) {
  ui->setupUi(this);

  ui->cb_operations->setModel(&cbModel);
  connect(ui->cb_operations, SIGNAL(currentIndexChanged(int)),
          SLOT(onDataChanged()));
}

HeaderTableFilterWidget::~HeaderTableFilterWidget() { delete ui; }

void HeaderTableFilterWidget::setMainWidget(QWidget *widget) {
  if (mainWidget != widget) {
    mainWidget = widget;
    int slotIndex = this->metaObject()->indexOfMethod("onDataChanged()");
    connect(mainWidget, mainWidget->metaObject()->userProperty().notifySignal(),
            this, this->metaObject()->method(slotIndex));

    QLayout *lo = ui->mainWidgetContainer->layout();
    if (lo)
      lo->deleteLater();

    lo = new QHBoxLayout();
    lo->setContentsMargins(0, 0, 0, 0);
    ui->mainWidgetContainer->setLayout(lo);
    lo->addWidget(widget);
  }
}

Operations HeaderTableFilterWidget::getOperations() const { return operations; }

void HeaderTableFilterWidget::setOperations(const Operations &value) {
  if (operations != value) {
    operations = value;
    recreateComboboxItem();
  }
}

QVariant HeaderTableFilterWidget::getData() {
  auto userPropertyName = mainWidget->metaObject()->userProperty().name();
  return mainWidget->property(userPropertyName);
}

Operation HeaderTableFilterWidget::getSelectOperation() {
  QString currentText = ui->cb_operations->currentText();
  return OperationSimbols.key(currentText);
}

void HeaderTableFilterWidget::onDataChanged() {
  if (!notUpdate) {
    QKeyEvent *event =
        new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
    QCoreApplication::postEvent(this, event);
  }
}

void HeaderTableFilterWidget::recreateComboboxItem() {
  QStringList result;
  if (operations.testFlag(Operation::Like))
    result.append(OperationSimbols[Operation::Like]);

  if (operations.testFlag(Operation::Equal))
    result.append(OperationSimbols[Operation::Equal]);

  if (operations.testFlag(Operation::NotEqual))
    result.append(OperationSimbols[Operation::NotEqual]);

  cbModel.setStringList(result);

  notUpdate = true;
  ui->cb_operations->setCurrentIndex(0);
  notUpdate = false;
}
