#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class DatabasePicker;
class QDateEdit;
class QDialogButtonBox;
class QCheckBox;
class JSpinBox;

class TimeCardGenerator : public QObject
{
  Q_OBJECT

public:
  TimeCardGenerator(const QString& fileName, const QString& html, bool bOpen);

public slots:
  void generate();

signals:
  void finished();

private:
  QString m_fileName;
  QString m_html;
  bool m_bOpen;
};

class TimeCardDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCardDialog(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();
  void updateControls();

private:
  DatabasePicker* m_storePicker;
  QDateEdit* m_date;
  QDialogButtonBox* m_buttons;
  JSpinBox* m_spnExtraPages;
  QCheckBox* m_cbOpenFile;
};

#endif // TIMECARD_H
