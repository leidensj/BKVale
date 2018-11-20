#ifndef JTIMEEDIT_H
#define JTIMEEDIT_H

#include <QTimeEdit>

class JTimeEdit : public QTimeEdit
{
  Q_OBJECT

public:
  enum class Flags
  {
    EnterAsTab = 1 << 1,
    ArrowsAsTab = 1 << 2,
  };

  explicit JTimeEdit(int flags = (int)Flags::EnterAsTab |(int)Flags::ArrowsAsTab,
                     QWidget* parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  int m_flags;

signals:
  void enterSignal();
  void keyDownSignal();
  void keyUpSignal();
};

#endif // JTIMEEDIT_H
