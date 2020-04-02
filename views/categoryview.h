#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include "items/category.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;

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
  DatabasePicker* m_imagePicker;
};

#endif // CATEGORYVIEW_H
