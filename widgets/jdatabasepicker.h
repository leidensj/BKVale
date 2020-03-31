#ifndef JDATABASEPICKER_H
#define JDATABASEPICKER_H

#include <QFrame>
#include <QString>
#include <QVector>
#include <QSqlDatabase>
#include "items/jitemsql.h"

class QPushButton;
class JLineEdit;
class JImageView;
class JDatabaseSelector;
class JDatabase;
class JClickLabel;

class JDatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabasePicker(const QString& tableName, bool bMultiPicker = false, QWidget* parent = nullptr);

  Id getId() const;
  const QVector<Id>& getIds() const;
  void setPlaceholderText(bool bSet);
  JDatabase* getDatabase() const;
  QString getText() const;

public slots:
  void clear();
  void setItems(const QVector<JItemSQL*>& v);
  bool setItem(const JItemSQL& o);
  bool setItem(Id id,
               const QString& name,
               const QByteArray& arImage);

private slots:
  void searchItem();
  void showImage();

signals:
  void changedSignal();

private:
  bool m_bMultiPicker;
  JDatabaseSelector* m_selector;
  JLineEdit* m_edText;
  JClickLabel* m_lblImage;
  QVector<Id> m_ids;
  QVector<QString> m_names;
};

#endif // JDATABASEPICKER_H
