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
  bool isValid() const;

private:
  NoteView* m_view;
  JDatabase* m_database;
  QDockWidget* m_dock;
  bool save();
  bool print(QIODevice* printer,
             InterfaceType type,
             const QString& user,
             int id);

public slots:
  void create();
  void saveAndPrint(QIODevice* printer,
                    InterfaceType type,
                    const QString& user);

private slots:
  void emitChangedSignal();
  void showSearch();
  void setNote(int id);
  void removeNote(int id);
  void searchSupplier();

signals:
  changedSignal();
};

#endif // NOTEWIDGET_H
