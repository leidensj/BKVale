#ifndef FORMWIDGET_H
#define FORMWIDGET_H

#include <QWidget>
#include "items/form.h"
#include "tables/phonetable.h"

class QLabel;
class QRadioButton;
class JLineEdit;
class QDateEdit;
class JDatabasePicker;
class QCheckBox;
class QPushButton;
class AddressTable;
class JAddRemoveButtons;
class JPlainTextEdit;
class JTable;
class QTableWidgetItem;

class FormInfoWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FormInfoWidget(QWidget* parent = 0);
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

public slots:
  void setCompany(bool b);
  void addWidget(const QString& text, QWidget* w);

private slots:
  void switchUserType();

signals:
  void userTypeChangedSignal(bool bCompany);
};

class FormDetailsWidget : public QWidget
{
  Q_OBJECT

  bool m_bPreviousWasCompany;
  QString m_strPreviousCpfCnpj;
  QString m_strPreviousRgIE;

public:
  explicit FormDetailsWidget(QWidget* parent = 0);
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
  JDatabasePicker* m_imagePicker;

private slots:
  void updateControls();

public slots:
  void switchUserType(bool bCompany);
};

class FormPhoneWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FormPhoneWidget(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;

  JAddRemoveButtons* m_btnAddRemove;
  PhoneTable* m_tbPhone;
};

class FormAddressWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FormAddressWidget(QWidget* parent = 0);
  void setForm(const Form& o);
  void fillForm(Form& o) const;

  JAddRemoveButtons* m_btnAddRemove;
  AddressTable* m_tbAddress;
};

#endif // FORMWIDGET_H
