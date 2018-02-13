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
  void setImage(int imageId, const QString& imageName);

private slots:
  void emitSaveSignal();
  void emitSearchImageSignal();
  void updateControls();

public slots:
  void setCategory(const Category& category,
                   const QString& imageName);
  void create();

signals:
  void saveSignal();
  void searchImageSignal();

private:
  int m_currentId;
  int m_currentImageId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  QPushButton* m_btnSearchImage;
  JLineEdit* m_edImageName;
  QPushButton* m_btnClearImage;
};

#endif // CATEGORYVIEW_H
