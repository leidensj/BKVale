#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include <QTextEdit>

class JLineEdit;
class QPushButton;

class FindWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FindWidget(QTextEdit* doc, QWidget* parent = nullptr);

private slots:
  void find();
  void updateControls();

private:
  QTextEdit* m_doc;
  JLineEdit* m_edFind;
  QPushButton* m_btnDown;
  QPushButton* m_btnUp;
  QPushButton* m_btnCaseSensitive;
};

#endif // FINDWIDGET_H
