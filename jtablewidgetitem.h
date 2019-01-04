#ifndef JTABLEWIDGETITEM_H
#define JTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QDialog>
#include <jitem.h>
#include <QVector>
#include "product.h"
#include "person.h"
#include "store.h"
#include "phone.h"
#include "address.h"

class JSpinBox;
class JLineEdit;
class JSpinBox;
class QDialogButtonBox;
class QComboBox;

class PhoneEditorDlg : public QDialog
{
  Q_OBJECT

public:
  explicit PhoneEditorDlg(QWidget* parent = nullptr);
  Phone getPhone() const;
  void setPhone(const Phone& o);

private slots:
  void updateControls();

private:
  Id m_currentId;
  JSpinBox* m_spnCountryCode;
  JSpinBox* m_spnCode;
  JLineEdit* m_edNumber;
  JLineEdit* m_edName;
  QDialogButtonBox* m_btn;
};

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

class DoubleTableWidgetItem : public QTableWidgetItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  DoubleTableWidgetItem(JItem::DataType type, Color color, bool bCheckable = false);
  bool evaluate(const QString& exp);
  void evaluate();
  void setValue(double val);
  double getValue();

private:
  const JItem::DataType m_type;
  const Color m_color;
  const bool m_bCheckable;
};

class TimeIntervalsTableWidgetItem : public QTableWidgetItem
{
public:
  TimeIntervalsTableWidgetItem();
  void setItems(const QVector<TimeInterval>& v);
  const QVector<TimeInterval>& getItems() const;
  void selectItem();

private:
  QVector<TimeInterval> m_timeIntervals;
};

class PhoneEditorTableWidgetItem : public QTableWidgetItem
{
public:
  PhoneEditorTableWidgetItem();
  void setItem(const Phone& o);
  const Phone& getItem() const;
  bool selectItem();

private:
  Phone m_o;
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

class PackageTableWidgetItem : public QTableWidgetItem
{
public:
  explicit PackageTableWidgetItem();
  void setItem(const Package& o, const QString& productUnity);
  const Package& getItem() const;
  void selectItem(const QString& productUnity);
private:
  Package m_package;
};

class JItemTableWidgetItem : public QTableWidgetItem
{
public:
  explicit JItemTableWidgetItem();
  virtual void setItem(const JItem& o) = 0;
  virtual const JItem& getItem() const = 0;
  virtual void selectItem(const QString& fixedFilter) = 0;
};

class ProductTableWidgetItem : public JItemTableWidgetItem
{
public:
  void setItem(const JItem& o);
  const JItem& getItem() const;
  void selectItem(const QString& fixedFilter);
  void selectItemByCode(const QString& fixedFilter);

private:
  Product m_product;
};

class PersonTableWidgetItem : public JItemTableWidgetItem
{
public:
  void setItem(const JItem& o);
  const JItem& getItem() const;
  void selectItem(const QString& fixedFilter);

private:
  Person m_person;
};

#endif // JTABLEWIDGETITEM_H
