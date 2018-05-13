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

class ShoppingListView : public QFrame
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);
  void setDatabase(QSqlDatabase db);

private slots:
  /*bool save();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);*/

  void addItem();
  void removeItem();
  void editItem();
  void updateControls();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;

  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QPushButton* m_btnEdit;

  JDatabase* m_database;
  JLineEdit* m_edTitle;
  JLineEdit* m_edDescription;
  JDatabasePicker* m_supplierPicker;

  QCheckBox* m_cbPrintAmmount;
  QCheckBox* m_cbPrintPrice;

  QCheckBox* m_cbSupplierCalls;
  QCheckBox* m_cbCallSupplier;
  QCheckBox* m_cbWhatsapp;
  QCheckBox* m_cbVisit;

  ShoppingListTable* m_table;

  QVector<QPushButton*> m_vbtnMonth;
  QVector<QPushButton*> m_vbtnWeek;
};

#endif // SHOPPINGLISTVIEW_H
