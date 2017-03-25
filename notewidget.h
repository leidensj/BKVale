#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include "note.h"

class QTableWidgetItem;

namespace Ui {
class NoteWidget;
}

class NoteWidget : public QFrame
{
  Q_OBJECT

protected:
    bool eventFilter(QObject* obj, QEvent* event);

public:
  explicit NoteWidget(QWidget *parent = 0);
  ~NoteWidget();
  bool isValidSelection() const;
  Note getNote() const;
  void setNote(const Note& note);
  QStringList getItemDescriptions() const;
  bool isValid() const;
  bool isDirty() const;
  bool isHistoryMode() const;

private:
  Ui::NoteWidget *ui;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column) const;
  QString text(int row, int column) const ;
  void setText(int row, int column, const QString& str);
  QString serializeItems() const;
  bool m_bDirty;
  bool m_bHistoryMode;

private slots:
  void updateTable(int row, int column);
  void changed();
  void upperSupplier(const QString& text);
  void upperDescription(QTableWidgetItem* item);

public slots:
  void addItem();
  void clear();
  void removeItem();
  void createNew(int number,
                 const QStringList& suppliers);
  void setEnabled(bool bEnable);

signals:
  void changedSignal();

};

#endif // BKFRAME_H
