#ifndef ADDRESSEDITOR_H
#define ADDRESSEDITOR_H

#include <QDialog>
#include "items/address.h"

class JSpinBox;
class JLineEdit;
class QDialogButtonBox;
class QComboBox;
class QPushButton;

class AddressEditor : public QDialog
{
  Q_OBJECT

public:
  explicit AddressEditor(QWidget* parent = nullptr);
  Address getAddress() const;
  void setAddress(const Address& o);

private slots:
  void updateControls();
  void processPostalCode();

private:
  Id m_currentId;
  JLineEdit* m_edPostalCode;
  QPushButton* m_btnPostalCode;
  JLineEdit* m_edNeighborhood;
  JLineEdit* m_edStreet;
  JSpinBox* m_spnNumber;
  JLineEdit* m_edCity;
  QComboBox* m_cbState;
  JLineEdit* m_edComplement;
  JLineEdit* m_edReference;
  QDialogButtonBox* m_btn;
};

#endif // ADDRESSEDITOR_H
