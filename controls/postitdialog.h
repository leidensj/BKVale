#ifndef POSTITDIALOG_H
#define POSTITDIALOG_H

#include <QDialog>

class JLineEdit;
class QPlainTextEdit;
class QPushButton;
class QCheckBox;

class PostItDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PostItDialog(QWidget* parent = nullptr);

private slots:
  void print();
  void create();
  void updateSize(Qt::CheckState ck);

private:
  JLineEdit* m_title;
  JLineEdit* m_subtitle;
  QPlainTextEdit* m_text;
  QPushButton* m_btnPrint;
  QPushButton* m_btnNew;
  QCheckBox* m_cbTitleSize;
  QCheckBox* m_cbSubtitleSize;
  QCheckBox* m_cbTextSize;
};

#endif // POSTITDIALOG_H
