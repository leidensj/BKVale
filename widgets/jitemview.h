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
  virtual const JItemSQL& getItem() const = 0;
  virtual Id getId() const = 0;

protected slots:
  virtual void itemsRemoved(const QVector<Id>& ids);
  virtual void save();

public slots:
  virtual void selectItem(const JItemSQL& o);
  virtual void create() = 0;

signals:
  void saveSignal();

protected:
  JDatabase* m_database;
  QTabWidget* m_tab;
  QTabWidget* m_tabDb;
  QHBoxLayout* m_ltButton;

private:
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnSearch;
  QDialog* m_dlgDb;
  virtual void setItem(const JItemSQL& o) = 0;
};

#endif // JVIEW_H
