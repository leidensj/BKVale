#ifndef JVIEW_H
#define JVIEW_H

#include <QWidget>
#include "items/jitemsql.h"
#include <QPushButton>
#include "controls/databaseviewer.h"
#include <QTabWidget>

class QPushButton;
class QTabWidget;
class QDialog;
class QHBoxLayout;

class JItemView : public QWidget
{
  Q_OBJECT

public:
  explicit JItemView(const QString& tableName, QWidget* parent = nullptr);
  ~JItemView();
  Id getId() const;

public slots:
  virtual void getItem(JItemSQL& o) const = 0;
  virtual void clear();
  virtual void setItem();
  virtual bool save(Id& id);

protected slots:
  virtual void setItem(const JItemSQL& o) = 0;
  virtual void itemsRemoved(const QVector<Id>& ids);
  virtual void save();
  void setFocusWidgetOnCreate(QWidget* w);
  void addViewButton(const QString& tableName);
  void viewButtonClicked();

signals:
  void saveSignal();
  void itemSelectedSignal();

protected:
  Id m_id;
  DatabaseViewer* m_viewer;
  QTabWidget* m_tab;
  QTabWidget* m_tabDb;
  QHBoxLayout* m_ltButton;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnSearch;
  QPushButton* m_btnMore;

private:
  QDialog* m_dlgDb;
  QWidget* m_wFocus;
};

#endif // JVIEW_H
