#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include <QFrame>
#include "category.h"

class QPushButton;
class JLineEdit;
class JPicker;

class CategoryView : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryView(QWidget* parent = 0);
  Category getCategory() const;
  void setImage(int id, const QString& name, const QByteArray& ar);

private slots:
  void emitSaveSignal();
  void emitSearchImageSignal();
  void updateControls();

public slots:
  void setCategory(const Category& category);
  void create();

signals:
  void saveSignal();
  void searchImageSignal();

private:
  Category m_currentCategory;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JPicker* m_imagePicker;
};

#endif // CATEGORYVIEW_H
