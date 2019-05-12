#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include "items/category.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;

class CategoryView : public JItemView
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();

private:
  mutable Category m_ref;
  JLineEdit* m_edName;
  JDatabasePicker* m_imagePicker;
  void setItem(const JItemSQL& o);
};

#endif // CATEGORYVIEW_H
