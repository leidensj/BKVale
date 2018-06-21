#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include <QFrame>
#include "category.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JDatabase;

class CategoryView : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);
  Category getCategory() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

public slots:
  void setCategory(const Category& category);
  void create();

signals:
  void saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JDatabasePicker* m_imagePicker;
  JDatabase* m_database;
};

#endif // CATEGORYVIEW_H
