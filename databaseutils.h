#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <QVector>
#include "user.h"
#include "note.h"
#include "product.h"
#include "consumption.h"
#include "reminder.h"
#include "settings.h"
#include "category.h"
#include "image.h"
#include "person.h"
#include "phone.h"
#include "address.h"

class UserLoginSQL
{
public:
  UserLoginSQL();

  bool login(const QString& strUser,
             const QString& strPassword,
             QString& error);

  void setDatabase(QSqlDatabase db);
  QSqlDatabase getDatabase() const;

private:
  QSqlDatabase m_db;
  User m_user;

public:
  qlonglong getId() const { return m_user.m_id; }
  bool isValid() const { return IS_VALID_ID(m_user.m_id); }
  QString strUser() const { return m_user.m_strUser; }
  bool hasAccessToNote() const { return isValid() && m_user.m_bAccessNote; }
  bool hasAccessToReminder() const { return isValid() && m_user.m_bAccessReminder; }
  bool hasAccessToCalculator() const { return isValid() && m_user.m_bAccessCalculator; }
  bool hasAccessToShop() const { return isValid() && m_user.m_bAccessShop; }
  bool hasAccessToConsumption() const { return isValid() && m_user.m_bAccessConsumption; }
  bool hasAccessToUsers() const { return isValid() && m_user.m_bAccessUser; }
  bool hasAccessToProducts() const { return isValid() && m_user.m_bAccessProduct; }
  bool hasAccessToSettings() const { return isValid() && m_user.m_bAccessSettings; }
};

namespace BaitaSQL
{
  bool isOpen(QSqlDatabase db,
              QString& error);

  bool open(QSqlDatabase db,
            const QString& path,
            QString& error);

  void close(QSqlDatabase db);

  bool init(QSqlDatabase db,
            QString& error);
}

namespace UserSQL
{
  bool insert(QSqlDatabase db,
              const User& user,
              QString& error);

  bool update(QSqlDatabase db,
              const User& user,
              QString& error);

  bool select(QSqlDatabase db,
              User& user,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);
}

namespace NoteSQL
{
  qlonglong nextNumber(QSqlDatabase db);

  bool insert(QSqlDatabase db,
              const Note& note,
              QString& error);

  bool update(QSqlDatabase db,
              const Note& note,
              QString& error);

  bool select(QSqlDatabase db,
              Note& note,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);
}

namespace ProductSQL
{
bool execSelect(QSqlQuery& query,
                Product& product,
                QString& error);

bool select(QSqlDatabase db,
            Product& product,
            QString& error);

bool insert(QSqlDatabase db,
            const Product& product,
            QString& error);

bool update(QSqlDatabase db,
            const Product& product,
            QString& error);

bool remove(QSqlDatabase db,
            qlonglong id,
            QString& error);
}

namespace CategorySQL
{
 bool execSelect(QSqlQuery& query,
                 Category& category,
                 QString& error);

  bool select(QSqlDatabase db,
              Category& category,
              QString& error);

  bool insert(QSqlDatabase db,
              const Category& category,
              QString& error);

  bool update(QSqlDatabase db,
              const Category& category,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);
}

namespace ImageSQL
{
  bool execSelect(QSqlQuery& query,
                  Image& image,
                  QString& error);

  bool select(QSqlDatabase db,
              Image& image,
              QString& error);

  bool insert(QSqlDatabase db,
              const Image& image,
              QString& error);

  bool update(QSqlDatabase db,
              const Image& image,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);
}

namespace ReminderSQL
{
  bool execSelect(QSqlQuery& query,
                  Reminder& reminder,
                  QString& error);

  bool insert(QSqlDatabase db,
              const Reminder& reminder,
              QString& error);

  bool update(QSqlDatabase db,
              const Reminder& reminder,
              QString& error);

  bool select(QSqlDatabase db,
              Reminder& reminder,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);
}

namespace ConsumptionSQL
{
  bool selectByDate(QSqlDatabase db,
                    qint64 date,
                    QVector<Consumption>& consumptions,
                    QString& error);

  bool selectTotal(QSqlDatabase db,
                   const Consumption::Filter& filter,
                   double& total,
                   QString& error);

  bool selectSubTotal(QSqlDatabase db,
                      const Consumption::Filter& filter,
                      QVector<qint64>& dates,
                      QVector<double>& totals,
                      QString &error);

  void getConsumption(QSqlDatabase db,
                      qint64 date,
                      QVector<Consumption>& vConsumption,
                      QVector<Product>& vProduct);

  double getTotal(QSqlDatabase db,
                  const Consumption::Filter& filter);

  double getTotal(QSqlDatabase db,
                  qint64 date);
}

namespace PersonSQL
{
  bool execSelect(QSqlQuery& query,
                  Person& person,
                  QString& error);

  bool execByPinCodeSelect(QSqlQuery& query,
                           const QString& pincode,
                           Person& person,
                           QString& error);

  bool select(QSqlDatabase db,
              Person& person,
              QString& error);

  bool insert(QSqlDatabase db,
              const Person& person,
              QString& error);

  bool update(QSqlDatabase db,
              const Person& person,
              QString& error);

  bool remove(QSqlDatabase db,
              qlonglong id,
              QString& error);

  bool isValidPinCode(QSqlDatabase db,
                      const QString& pincode,
                      Person& person,
                      QString& error);
}

#endif // DATABASEUTILS_H
