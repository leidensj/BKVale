#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QComboBox>
#include "note.h"

class QTableWidgetItem;

namespace Ui {
class NoteWidget;
}

class BKComboBox : public QComboBox
{
  Q_OBJECT

public:
  BKComboBox();

protected:
  void keyPressEvent(QKeyEvent *event);

public slots:
  void toUpper();

signals:
  void advanceSignal();
};

class NoteWidget : public QFrame
{
  Q_OBJECT

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
  QStringList m_descriptions;

private slots:
  void updateTable(int row, int column);
  void changed();
  void toUpper(const QString& text);

public slots:
  void addItem();
  void clear();
  void removeItem();
  void createNew(int number,
                 const QStringList& suppliers,
                 const QStringList& descriptions);
  void setEnabled(bool bEnable);
  void advance();

signals:
  void changedSignal();

};

#endif // BKFRAME_H
