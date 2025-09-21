#ifndef CHARMOMETER_H
#define CHARMOMETER_H

#include <QDialog>

class QPushButton;

class Charmometer : public QDialog
{
  Q_OBJECT

public:
  explicit Charmometer(QWidget* parent = nullptr);

private:
  QPushButton* m_btnPrint;
  QPushButton* m_btnNew;
};

#endif // CHARMOMETER_H
