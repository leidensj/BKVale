#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include "items/category.h"
#include "widgets/jitemview.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;

class CategoryView : public JItemView
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  JDatabasePicker* m_imagePicker;
};

#endif // CATEGORYVIEW_H
