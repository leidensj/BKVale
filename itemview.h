#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include <QFrame>
#include "item.h"
#include "category.h"

class QLabel;
class JLineEdit;
class QDoubleSpinBox;
class QPushButton;
class QCheckBox;

class ItemView : public QFrame
{
  Q_OBJECT

public:
  explicit ItemView(QWidget* parent = 0);
  ~ItemView();
  Item getItem() const;
  void setCategory(int categoryId,
                   const QString& categoryName);

private slots:
  void emitSearchCategorySignal();
  void emitSaveSignal();
  void clearCategory();
  void updateControls();

public slots:
  void setItem(const Item& item,
               const QString& categoryName);
  void create();

signals:
  searchCategorySignal();
  saveSignal();

private:
  int m_currentId;
  int m_currentCategoryId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edPackageUnity;
  QDoubleSpinBox* m_sbPackageAmmount;
  JLineEdit* m_edDetails;
  JLineEdit* m_edCode;
  QCheckBox* m_cbAvailableAtNotes;
  QCheckBox* m_cbAvailableAtShop;
  QCheckBox* m_cbAvailableAtConsumption;
  JLineEdit* m_edCategory;
  QPushButton* m_btnSearchCategory;
  QPushButton* m_btnClearCategory;
};

#endif // ITEMVIEW_H
