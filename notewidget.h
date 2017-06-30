#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QComboBox>
#include <QTableWidget>
#include "note.h"
#include "notedatabasewidget.h"

#define MAX_ITEMS 100

class QTableWidgetItem;

namespace Ui {
class NoteWidget;
}

class NoteComboBox : public QComboBox
{
  Q_OBJECT

public:

  enum Behavior
  {
    Supplier,
    TableUnity,
    TableDescription
  };

  NoteComboBox(Behavior behavior);
  const Behavior m_behavior;

protected:
  void keyPressEvent(QKeyEvent *event);

public slots:
  void toUpper();

signals:
  void supplierEnteredSignal();
};

class NoteTableWidget : public QTableWidget
{
  Q_OBJECT

protected:
  void keyPressEvent(QKeyEvent *event);

public:
  NoteTableWidget();
  QString text(int row, int column) const ;
  void setText(int row, int column, const QString& str);
  QString serializeItems() const;
};

class NoteWidget : public QFrame
{
  Q_OBJECT

public:
  explicit NoteWidget(QWidget *parent = 0);
  ~NoteWidget();
  bool isItemSelected() const;
  Note getNote() const;
  QStringList getItemDescriptions() const;
  bool isValid() const;
  bool isHistoryMode() const;
  void setHistoryDatabase(const QSqlDatabase& sqldb);

private:
  Ui::NoteWidget *ui;
  NoteComboBox m_supplier;
  NoteTableWidget m_table;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column) const;
  QStringList m_descriptions;
  NoteDatabaseWidget m_noteDatabaseWidget;

private slots:
  void updateTable(int row, int column);
  void changed();

public slots:
  void addItem();
  void removeItem();
  void clear();
  void create(int number,
              const QStringList& suppliers,
              const QStringList& descriptions);
  void setEnabled(bool bEnable);
  void supplierEntered();
  void showNoteDatabase();
  void setNote(const Note& note);

signals:
  void changedSignal();
};

#endif // BKFRAME_H
