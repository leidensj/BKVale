#ifndef JPLAINTEXTEDIT_H
#define JPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class JPlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

public:
  JPlainTextEdit(bool enterAsTab = true,
                 QWidget* parent = nullptr);

public:
  void setTextBlockingSignals(const QString& str);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_enterAsTab;

signals:
  void enterSignal();
};

#endif // JPLAINTEXTEDIT_H
