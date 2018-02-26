#ifndef ADDRESSPAGEVIEW_H
#define ADDRESSPAGEVIEW_H

#include <QFrame>
#include "address.h"
#include <QVector>
#include <QMetaType>

class JLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class QListWidget;
class QListWidgetItem;

Q_DECLARE_METATYPE(Address)

class AddressPageView : public QFrame
{
  Q_OBJECT

public:
  explicit AddressPageView(QWidget* parent = nullptr);
  QVector<Address> getAddresses();

public slots:
  void clear();


private slots:
  void save();
  void openSelectedAddress();
  void removeSelectedAddress();
  void saveAddress(const Address& address);
  void setAddress(const Address& address);
  void clearInputOnly();
  void updateControls();
  void undo();
  void searchCep();

private:
  QListWidgetItem* m_currentItem;
  Address getAddress();
  JLineEdit* m_edCep;
  QPushButton* m_btnCep;
  JLineEdit* m_edNeighborhood;
  JLineEdit* m_edStreet;
  QSpinBox* m_spnNumber;
  QComboBox* m_cbState;
  JLineEdit* m_edComplement;
  JLineEdit* m_edReference;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnUndo;
  QPushButton* m_btnRemove;
  QListWidget* m_list;
};

#endif // ADDRESSPAGEVIEW_H
