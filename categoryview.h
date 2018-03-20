#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "category.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;

class CategoryView : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  Category getCategory() const;

private slots:
  void emitSaveSignal();
  void updateControls();

public slots:
  void setCategory(const Category& category);
  void create();

signals:
  void saveSignal();

private:
  Category m_currentCategory;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JDatabasePicker* m_imagePicker;
};

#endif // CATEGORYVIEW_H
