#ifndef JVIEW_H
#define JVIEW_H

#include <QFrame>
#include "items/jitemsql.h"
#include <QPushButton>
#include "widgets/jdatabase.h"
#include <QTabWidget>

class QPushButton;
class JDatabase;
class QTabWidget;
class QDialog;
class QHBoxLayout;

class JItemView : public QFrame
{
  Q_OBJECT

public:
  explicit JItemView(const QString& tableName, QWidget* parent = 0);
  Id getId() const;

public slots:
  virtual void getItem(JItemSQL& o) const = 0;
  virtual void create() = 0;
  virtual void selectItem(const JItemSQL& o);

protected slots:
  virtual void setItem(const JItemSQL& o) = 0;
  virtual void itemsRemoved(const QVector<Id>& ids);
  virtual void save();

signals:
  void saveSignal();

protected:
  Id m_id;
  JDatabase* m_database;
  QTabWidget* m_tab;
  QTabWidget* m_tabDb;
  QHBoxLayout* m_ltButton;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnSearch;

private:
  QDialog* m_dlgDb;
};

#endif // JVIEW_H
