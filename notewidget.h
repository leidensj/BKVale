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

class SupplierComboBox : public QComboBox
{
  Q_OBJECT

public:
  SupplierComboBox();

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
  QStringList getItemDescriptions() const;
  bool isValid() const;
  void setHistoryDatabase(QSqlDatabase db);
  bool save(QString& error);
  Note getNote() const;

private:
  Ui::NoteWidget *ui;
  SupplierComboBox m_supplier;
  NoteTableWidget m_table;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column) const;
  NoteDatabaseWidget m_noteDatabaseWidget;
  int currentNoteID;

private slots:
  void updateTable(int row, int column);
  void emitChangedSignal();
  void noteRemoved(int id);

public slots:
  void addItem();
  void removeItem();
  void clear();
  void create();
  void supplierEntered();
  void showNoteDatabase();
  void setNote(const Note& note);
  void enableControls();

signals:
  void changedSignal();
};

#endif // BKFRAME_H
