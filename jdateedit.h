#ifndef JDATEEDIT_H
#define JDATEEDIT_H

#include <QDateEdit>
#include <QKeyEvent>

class JDateEdit : public QDateEdit
{
  Q_OBJECT

public:
  explicit JDateEdit(QWidget* parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event);

signals:
  void enterSignal();
  void keyDownSignal();
  void keyUpSignal();
  void deleteSignal();
};

#endif // JDATEEDIT_H
