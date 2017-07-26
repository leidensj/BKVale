#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QFrame>
#include "product.h"
#include <QSqlDatabase>

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ProductWidget(QWidget *parent = 0);
  ~ProductWidget();
  void setDatabase(QSqlDatabase db);
  Product product() const;

public slots:
  void refresh();
  void enableControls();
  void removeSelectedProduct();
  void save(bool bSkipConfirmation);
  void discard(bool bSkipConfirmation);
  void create();
  void setFilter();
  void contains();

private:
  Ui::ProductWidget *ui;
  void confirm();
  ProductTableIndex currentSortIndicator() const;
};

#endif // PRODUCTWIDGET_H
