#ifndef PHONETABLEWIDGET_H
#define PHONETABLEWIDGET_H

#include "jitemtable.h"
#include "phone.h"

class PhoneTableWidget : public JItemTable
{
  Q_OBJECT

  enum class Column
  {
    Phone
  };

public:
  explicit PhoneTableWidget(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable Phone m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // STOREEMPLOYEETABLEWIDGET_H
