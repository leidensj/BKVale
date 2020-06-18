#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include <QTextDocument>

class JLineEdit;
class QPushButton;

class FindWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FindWidget(QWidget* parent = nullptr);

private:
  JLineEdit* m_edFind;
  QPushButton* m_btnDown;
  QPushButton* m_btnUp;
  QPushButton* m_btnCaseSensitive;

private slots:
  void emitFindSignal();

signals:
  void findSignal(const QString &exp, QTextDocument::FindFlags options);
};

#endif // FINDWIDGET_H
