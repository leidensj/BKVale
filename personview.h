#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QFrame>
#include "person.h"

class QPushButton;
class PersonPageView;
class QTabWidget;

class PersonView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonView(QWidget* parent = 0);
  ~PersonView();
  Person getPerson() const;
  void setImage(int id, const QString& name, const QByteArray& ar);

private slots:
  //void emitSearchImageSignal();
  void emitSaveSignal();
  void updateControls();

public slots:
  void setPerson(const Person& person,
                 const QString& imageName,
                 const QByteArray& arImage);
  void create();

signals:
  //searchImageSignal();
  saveSignal();

private:
  Person m_currentPerson;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  PersonPageView* m_personPage;
};

#endif // PERSONVIEW_H
