#ifndef SHOPPINGLISTVIEW_H
#define SHOPPINGLISTVIEW_H

#include <QFrame>
#include <QVector>
#include "shoppinglist.h"

class JDatabase;
class QPushButton;
class JDatabasePicker;
class QCheckBox;
class JLineEdit;
class ShoppingListTable;
class QTabWidget;
class QPlainTextEdit;

class ShoppingListView : public QFrame
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);
  void setShoppingList(const ShoppingList& lst);
  ShoppingList getShoppingList() const;

private slots:
  void save();
  void itemSelected(const JItem& jItem);
  void itemsRemoved(const QVector<qlonglong>& ids);

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
  QPlainTextEdit* m_teDescription;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_imagePicker;

  ShoppingListTable* m_table;
  QTabWidget* m_tabWidget;

  QPushButton* m_vbtnMonth[31];
  QPushButton* m_vbtnWeek[7];
};

#endif // SHOPPINGLISTVIEW_H
