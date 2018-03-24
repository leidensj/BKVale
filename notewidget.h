#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "note.h"
#include "settings.h"

class QDockWidget;
class NoteView;
class JDatabase;
class QIODevice;

class NoteWidget : public QFrame
{
  Q_OBJECT

public:
  explicit NoteWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  Note getNote() const;

private:
  NoteView* m_view;
  JDatabase* m_database;
  QDockWidget* m_dock;

  bool save();
  bool print(QIODevice* printer,
             InterfaceType type,
             const QString& userName,
             int id);

public slots:
  void create();
  void saveAndPrint(QIODevice* printer,
                    InterfaceType type);

private slots:
  void emitChangedSignal();
  void showSearch();
  void setNote(qlonglong id);
  void setNote(const JItem& jItem);
  void checkRemovedNote(qlonglong id);
  void searchProduct(int row);

signals:
  changedSignal();
};

#endif // NOTEWIDGET_H
