#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QFrame>
#include "note.h"
#include <QSerialPort>
#include <QSqlDatabase>

class QDockWidget;
class NoteView;
class NoteDatabase;

class NoteWidget : public QFrame
{
  Q_OBJECT

public:
  explicit NoteWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  bool isValid() const;


private:
  NoteView* m_view;
  NoteDatabase* m_database;
  QDockWidget* m_dock;
  bool save();
  bool print(QSerialPort& printer, int id);

public slots:
  void create();
  void saveAndPrint(QSerialPort& printer);

private slots:
  void emitChangedSignal();
  void showSearch();
  void setNote(int id);
  void removeNote(int id);

signals:
  changedSignal();
};

#endif // NOTEWIDGET_H
