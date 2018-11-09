#ifndef STOREVIEW_H
#define STOREVIEW_H

#include "store.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;

class StoreView : public JItemView
{
  Q_OBJECT

public:
  explicit StoreView(QWidget* parent = 0);
  const JItem& getItem() const;

public slots:
  void create();

private:
  mutable Store m_currentStore;
  JLineEdit* m_edName;
  JDatabasePicker* m_personPicker;
  JDatabasePicker* m_addressPicker;
  JDatabasePicker* m_phonePicker;
  void setItem(const JItem& o);
};

#endif // STOREVIEW_H
