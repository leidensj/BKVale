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

public slots:
  void refresh();
  void enableControls();
  void removeSelectedProduct();
  void save();
  void revert();
  void create();

private:
  Ui::ProductWidget *ui;
};

#endif // PRODUCTWIDGET_H
