#ifndef JADDREMOVEBUTTONS_H
#define JADDREMOVEBUTTONS_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;

class JAddRemoveButtons : public QWidget
{
  Q_OBJECT

public:
  explicit JAddRemoveButtons(QWidget* parent = nullptr);
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QHBoxLayout* m_lt;

public slots:
  void enableRemoveButton(bool bEnable);
};

#endif // JADDREMOVEBUTTONS_H
