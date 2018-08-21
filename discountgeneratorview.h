#ifndef DISCOUNTGENERATORVIEW_H
#define DISCOUNTGENERATORVIEW_H

/*#include <QFrame>
#include "discount.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JDatabase;
class QRadioButton;
class DiscountTableWidget;

class DiscountGeneratorView : public QFrame
{
  Q_OBJECT

public:
  explicit DiscountGeneratorView(QWidget* parent = 0);
  Discount getDiscount() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

public slots:
  void setDiscount(const Discount& discount);
  void create();

signals:
  void saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edCode;
  QRadioButton* m_rdValue;
  QRadioButton* m_rdPercentage;
  QRadioButton* m_rdProduct;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  DiscountTableWidget* m_table;
  JDatabase* m_database;
};*/

#endif // DISCOUNTGENERATORVIEW_H
