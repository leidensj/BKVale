#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QFrame>
#include "product.h"
#include <QSqlDatabase>
#include <QLineEdit>

namespace Ui {
class ProductWidget;
}

class FilterLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  FilterLineEdit();

protected:
  void keyPressEvent(QKeyEvent *event);

public slots:
  void toUpper();

signals:
  void downArrowPressedSignal();
  void enterKeyPressedSignal();
  void filterChangedSignal();
};

class ProductWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ProductWidget(bool bEditMode, QWidget *parent = 0);
  ~ProductWidget();
  void setDatabase(QSqlDatabase db);

public slots:
  void refresh();
  void enableControls();
  void removeSelectedProduct();
  void save(bool bSkipConfirmation);
  void discard(bool bSkipConfirmation);
  void create();
  void setFilter();
  void focusFilter();
  void containsPressed();
  void filterDownArrowPressed();
  void filterEnterKeyPressed();
  void productSelected();

private:
  Ui::ProductWidget *ui;
  FilterLineEdit m_filter;
  bool m_bEditMode;
  void confirm();
  Product product() const;
  ProductTableIndex currentSortIndicator() const;

signals:
  void productSelectedSignal(const Product& product);
};

#endif // PRODUCTWIDGET_H
