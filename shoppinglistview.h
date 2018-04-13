#ifndef SHOPPINGLISTVIEW_H
#define SHOPPINGLISTVIEW_H

#include <QFrame>
#include <QVector>
#include "shoppinglist.h"

class JDatabase;
class QPushButton;
class JDatabasePicker;
class QCheckBox;
class QPlainTextEdit;

class ShoppingListView : public QFrame
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);

private slots:
  /*bool save();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);*/

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;

  JDatabase* m_database;
  JDatabasePicker* m_supplierPicker;
  QPlainTextEdit* m_teDescription;

  QCheckBox* m_cbSupplierCalls;
  QCheckBox* m_cbCallSupplier;
  QCheckBox* m_cbWhatsapp;
  QCheckBox* m_cbVisit;

  QVector<QPushButton*> m_vbtnMonth;
  QVector<QPushButton*> m_vbtnWeek;
};

#endif // SHOPPINGLISTVIEW_H
