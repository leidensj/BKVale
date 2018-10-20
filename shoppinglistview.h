#ifndef SHOPPINGLISTVIEW_H
#define SHOPPINGLISTVIEW_H

#include <QFrame>
#include <QVector>
#include "shoppinglist.h"
#include "jitemview.h"

class JDatabasePicker;
class QCheckBox;
class JLineEdit;
class ShoppingListTable;
class QPlainTextEdit;
class JSpinBox;

class ShoppingListView : public JItemView
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);
  const JItem& getItem() const;

public slots:
  void create();

private slots:
  void addItem();
  void removeItem();
  void updateControls();

private:
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;

  JLineEdit* m_edTitle;
  JSpinBox* m_snLines;
  QPlainTextEdit* m_teDescription;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_imagePicker;

  ShoppingListTable* m_table;

  QPushButton* m_vbtnMonth[31];
  QPushButton* m_vbtnWeek[7];
  void setItem(const JItem& o);
};

#endif // SHOPPINGLISTVIEW_H
