#ifndef JSPINBOX_H
#define JSPINBOX_H

#include <QSpinBox>
#include <QKeyEvent>

class JSpinBox : public QSpinBox
{
  Q_OBJECT
public:
  JSpinBox(bool enterAsTab,
           QWidget* parent = nullptr);

public:
  void setValueBlockingSignals(int value);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_enterAsTab;

signals:
  void enterSignal();
};

#endif // JSPINBOX_H
