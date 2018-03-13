#ifndef PHONEPAGEVIEW_H
#define PHONEPAGEVIEW_H

#include <QFrame>
#include "phone.h"
#include <QVector>
#include <QMetaType>

class JLineEdit;
class QSpinBox;
class QPushButton;
class QListWidget;
class QListWidgetItem;

Q_DECLARE_METATYPE(Phone)

class PhonePageView : public QFrame
{
  Q_OBJECT

public:
  explicit PhonePageView(QWidget* parent = nullptr);
  QVector<Phone> getPhones() const;
  QVector<qlonglong> getRemovedPhones() const;
  void setPhones(const QVector<Phone>& vPhone);

public slots:
  void clear();

signals:
  void changedSignal();

private slots:
  void save();
  void openSelectedPhone();
  void removeSelectedPhone();
  void savePhone(const Phone& phone);
  void setPhone(const Phone& phone);
  void clearInputOnly();
  void updateControls();
  void undo();

private:
  Phone getPhone();
  QListWidgetItem* m_currentItem;
  QVector<qlonglong> m_vRemovedPhoneId;
  QSpinBox* m_spnCountryCode;
  QSpinBox* m_spnCode;
  JLineEdit* m_edNumber;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnUndo;
  QPushButton* m_btnRemove;
  QListWidget* m_list;
};

#endif // ADDRESSPAGEVIEW_H
