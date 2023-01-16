#ifndef FORMEDITOR_H
#define FORMEDITOR_H

#include <QWidget>
#include "items/form.h"
#include "tables/phonetable.h"

class QLabel;
class QRadioButton;
class JLineEdit;
class QDateEdit;
class DatabasePicker;
class QCheckBox;
class QPushButton;
class AddressTable;
class JAddRemoveButtons;
class JPlainTextEdit;
class JTable;
class QTableWidgetItem;

class FormInfoEditor : public QWidget
{
  Q_OBJECT

public:
  explicit FormInfoEditor(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;
  QDateEdit* m_dtCreationDate;
  QWidget* m_type;
  QRadioButton* m_rdoPerson;
  QRadioButton* m_rdoCompany;
  JLineEdit* m_edName;
  JLineEdit* m_edAlias;
  QLabel* m_lblType;
  QLabel* m_lblCreationDate;
  QLabel* m_lblName;
  QLabel* m_lblAlias;
  QWidget* m_sex;
  QLabel* m_lblSex;
  QRadioButton* m_rdoMale;
  QRadioButton* m_rdoFemale;

public slots:
  void setCompany(bool b);
  void addWidget(const QString& text, QWidget* w);

private slots:
  void switchUserType();

signals:
  void userTypeChangedSignal(bool bCompany);
};

class FormDetailsEditor : public QWidget
{
  Q_OBJECT

  bool m_bPreviousWasCompany;
  QString m_strPreviousCpfCnpj;
  QString m_strPreviousRgIE;

public:
  explicit FormDetailsEditor(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;

  JLineEdit* m_edCpfCnpj;
  QLabel* m_lblCpfCnpj;
  JLineEdit* m_edRgIE;
  QLabel* m_lblRgIE;
  JLineEdit* m_edEmail;
  JPlainTextEdit* m_teDetails;
  QDateEdit* m_dtBirthDate;
  QCheckBox* m_cbBirthDate;
  DatabasePicker* m_imagePicker;

private slots:
  void updateControls();

public slots:
  void switchUserType(bool bCompany);
};

class FormPhoneEditor : public QWidget
{
  Q_OBJECT

public:
  explicit FormPhoneEditor(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;

  JAddRemoveButtons* m_btnAddRemove;
  PhoneTable* m_tbPhone;
};

class FormAddressEditor : public QWidget
{
  Q_OBJECT

public:
  explicit FormAddressEditor(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;

  JAddRemoveButtons* m_btnAddRemove;
  AddressTable* m_tbAddress;
};

#endif // FORMEDITOR_H
