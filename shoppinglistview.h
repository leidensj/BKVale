#ifndef SHOPPINGLISTVIEW_H
#define SHOPPINGLISTVIEW_H

#include <QFrame>
#include <QVector>
#include <QSqlDatabase>
#include "shoppinglist.h"

class JDatabase;
class QPushButton;
class JDatabasePicker;
class QCheckBox;
class JLineEdit;
class ShoppingListTable;
class QTabWidget;

class ShoppingListView : public QFrame
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);
  void setDatabase(QSqlDatabase db);
  void setShoppingList(const ShoppingList& lst);
  ShoppingList getShoppingList() const;

private slots:
  void save();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);

  void create();
  void addItem();
  void removeItem();
  void editProduct();
  void editPackage(const Package& package,
                   const QString& productUnity);
  void updateControls();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;

  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;

  JDatabase* m_database;
  JLineEdit* m_edTitle;
  JLineEdit* m_edDescription;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_imagePicker;

  QCheckBox* m_cbPrintAmmount;
  QCheckBox* m_cbPrintPrice;

  QCheckBox* m_cbSupplierCalls;
  QCheckBox* m_cbCallSupplier;
  QCheckBox* m_cbWhatsapp;
  QCheckBox* m_cbVisit;

  ShoppingListTable* m_table;
  QTabWidget* m_tabWidget;

  QPushButton* m_vbtnMonth[31];
  QPushButton* m_vbtnWeek[7];
};

#endif // SHOPPINGLISTVIEW_H
