#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include <QFrame>
#include "category.h"

class QPushButton;
class JLineEdit;

class CategoryView : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);
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
  int m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
};

#endif // CATEGORYVIEW_H
