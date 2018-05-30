#ifndef RESERVATIONVIEW_H
#define RESERVATIONVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "reservation.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JDatabase;
class QDateTimeEdit;
class JSpinBox;
class QDockWidget;

class ReservationView : public QFrame
{
  Q_OBJECT

public:
  explicit ReservationView(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  Reservation getReservation() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();
  void showSearch();

public slots:
  void setReservation(const Reservation& reservation);
  void create();

signals:
  void saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSearch;
  JSpinBox* m_snNumber;
  JLineEdit* m_edName;
  JLineEdit* m_edLocation;
  QDateTimeEdit* m_dateTimeEdit;
  JSpinBox* m_snAmmount;
  JLineEdit* m_edObservation;
  QDockWidget* m_dock;
  JDatabase* m_database;
};

#endif // RESERVATIONVIEW_H
