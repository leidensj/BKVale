#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include <QFrame>
#include "item.h"

class QLabel;
class JLineEdit;
class QDoubleSpinBox;
class QPushButton;

class ItemView : public QFrame
{
  Q_OBJECT

public:
  explicit ItemView(QWidget* parent = 0);
  Item getItem() const;

public slots:
  void setItem(const Item& item);
  void create();

private:
  int m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edPackageUnity;
  QDoubleSpinBox* m_sbPackageAmmount;
  JLineEdit* m_edDetails;
  JLineEdit* m_edMidasCode;
};

#endif // ITEMVIEW_H
