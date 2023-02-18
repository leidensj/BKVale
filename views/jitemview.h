#ifndef JVIEW_H
#define JVIEW_H

#include <QWidget>
#include "items/jitemsql.h"
#include <QPushButton>
#include "controls/databaseviewer.h"
#include <QTabWidget>
#include <QDialog>

class QPushButton;
class QTabWidget;
class QDialog;
class QHBoxLayout;
class QToolButton;

class JItemView : public QWidget
{
  Q_OBJECT

public:
  explicit JItemView(const QString& tableName, QWidget* parent = nullptr);
  ~JItemView();
  Id getId() const;
  QString getTableName() const;

public slots:
  virtual void getItem(JItemSQL& o) const = 0;
  virtual void clear();
  virtual void setItem();
  virtual void setItem(const Id& id);
  virtual void setCopyItem();
  virtual void save();

protected slots:
  virtual void setItem(const JItemSQL& o) = 0;
  virtual void itemsRemoved(const Ids& ids);
  void setFocusWidgetOnClear(QWidget* w);
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
  QPushButton* m_btnClear;
  QPushButton* m_btnSave;
  QPushButton* m_btnSearch;
  QPushButton* m_btnPrint;
  QToolButton* m_btnMore;

private:
  QDialog* m_dlgDb;
  QWidget* m_wFocus;
};

class JItemViewDialog : public QDialog
{
  Q_OBJECT

public:
  explicit JItemViewDialog(JItemView* view, QWidget* parent = nullptr);
};

#endif // JVIEW_H
