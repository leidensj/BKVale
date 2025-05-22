#ifndef POSTITDIALOG_H
#define POSTITDIALOG_H

#include <QDialog>

class JLineEdit;
class QPlainTextEdit;
class QPushButton;

class PostItDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PostItDialog(QWidget* parent = nullptr);

private slots:
  void print();
  void create();

private:
  JLineEdit* m_title;
  JLineEdit* m_subtitle;
  QPlainTextEdit* m_text;
  QPushButton* m_btnPrint;
  QPushButton* m_btnNew;
};

#endif // POSTITDIALOG_H
