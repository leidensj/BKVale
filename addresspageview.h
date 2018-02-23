#ifndef ADDRESSPAGEVIEW_H
#define ADDRESSPAGEVIEW_H

#include <QFrame>
#include "address.h"

class JLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

class AddressPageView : public QFrame
{
public:
  explicit AddressPageView(QWidget* parent = nullptr);

private:
  JLineEdit* m_edCep;
  QPushButton* m_btnCep;
  JLineEdit* m_edNeighborhood;
  JLineEdit* m_edStreet;
  QSpinBox* m_spnNumber;
  QComboBox* m_cbState;
  JLineEdit* m_edComplement;
  JLineEdit* m_edReference;
};

#endif // ADDRESSPAGEVIEW_H
