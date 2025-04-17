#ifndef COINVIEW_H
#define COINVIEW_H

#include "items/coin.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;
class JDoubleSpinBox;

class CoinView : public JItemView
{
  Q_OBJECT

public:
  explicit CoinView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  JDoubleSpinBox* m_sbTax;
  DatabasePicker* m_imagePicker;
};

#endif // COINVIEW_H
