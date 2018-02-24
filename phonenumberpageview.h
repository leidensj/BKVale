#ifndef PHONENUMBERPAGEVIEW_H
#define PHONENUMBERPAGEVIEW_H

#include <QFrame>

class JLineEdit;
class QRadioButton;
class QSpinBox;
class QListWidget;
class QPushButton;

class PhoneNumberPageView : public QFrame
{
  Q_OBJECT
public:
  explicit PhoneNumberPageView(QWidget* parent = nullptr);

private slots:
  void updateControls();

private:
  JLineEdit* m_edNumber;
  QRadioButton* m_rdoComercial;
  QRadioButton* m_rdoMobile;
  QSpinBox* m_spnCountryCode;
  QSpinBox* m_spnCode;
  JLineEdit* m_edDescription;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QListWidget* m_list;
};

#endif // PHONENUMBERPAGEVIEW_H
