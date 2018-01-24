#ifndef JLINEEDIT_H
#define JLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class JLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit JLineEdit(bool toUpper,
                     bool enterAsTab,
                     QWidget* parent = 0);

  void setTextBlockingSignals(const QString& str);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_enterAsTab;

public slots:
  void toUpper();

signals:
  void enterSignal();
};

#endif // JLINEEDIT_H
