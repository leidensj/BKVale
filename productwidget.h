#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "product.h"

class JDatabase;
class ProductView;

class ProductWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ProductWidget(QWidget *parent = 0);
  ~ProductWidget();
  void setDatabase(QSqlDatabase db);

private slots:
  void productSelected(int id);
  void removeProduct(int id);
  void saveProduct();
  void searchCategory();
  void searchImage();

private:
  JDatabase* m_database;
  ProductView* m_view;
};

#endif // PRODUCTWIDGET_H
