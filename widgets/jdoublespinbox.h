#ifndef JDOUBLESPINBOX_H
#define JDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>

class JDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:
  JDoubleSpinBox(bool enterAsTab = true,
                 QWidget* parent = nullptr);

public:
  void setValueBlockingSignals(double value);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_enterAsTab;

signals:
  void enterSignal();
};

#endif // JDOUBLESPINBOX_H
