#ifndef JTABLEWIDGETITEM_H
#define JTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QDialog>
#include "items/jitem.h"
#include <QVector>
#include "items/product.h"
#include "items/supplier.h"
#include "items/employee.h"
#include "items/store.h"
#include "items/phone.h"
#include "items/address.h"

class JSpinBox;
class JLineEdit;
class JSpinBox;
class QDialogButtonBox;
class QComboBox;

class AddressEditorDlg : public QDialog
{
  Q_OBJECT

public:
  explicit AddressEditorDlg(QWidget* parent = nullptr);
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

class AddressEditorTableWidgetItem : public QTableWidgetItem
{
public:
  AddressEditorTableWidgetItem();
  void setItem(const Address& o);
  const Address& getItem() const;
  bool selectItem();

private:
  Address m_o;
};









#endif // JTABLEWIDGETITEM_H
