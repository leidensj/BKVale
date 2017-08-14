#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QFrame>
#include "product.h"
#include <QSqlDatabase>
#include <QLineEdit>

class QCheckBox;
class QTableView;
class QPushButton;

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
  FilterLineEdit* m_filter;
  QCheckBox* m_contains;
  QTableView* m_table;
  QPushButton* m_create;
  QPushButton* m_save;
  QPushButton* m_discard;
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QFrame* m_buttons;
  bool m_bEditMode;
  void confirm();
  Product product() const;
  ProductTableIndex currentSortIndicator() const;

signals:
  void productSelectedSignal(const Product& product);
};

#endif // PRODUCTWIDGET_H
