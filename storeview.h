#ifndef STOREVIEW_H
#define STOREVIEW_H

#include "store.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JPlainTextEdit;
class QListWidget;

class StoreView : public JItemView
{
  Q_OBJECT

public:
  explicit StoreView(QWidget* parent = 0);
  const JItemSQL& getItem() const;

public slots:
  void create();
  void updateControls();

private:
  mutable Store m_ref;
  JPlainTextEdit* m_teDescription;
  JDatabasePicker* m_formPicker;
  JDatabasePicker* m_addressPicker;
  JDatabasePicker* m_phonePicker;
  QListWidget* m_list;
  void setItem(const JItemSQL& o);
};

#endif // STOREVIEW_H
