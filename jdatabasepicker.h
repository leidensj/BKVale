#ifndef JDATABASEPICKER_H
#define JDATABASEPICKER_H

#include <QFrame>
#include <QString>
#include <category.h>
#include <QVector>
#include <QSqlDatabase>
#include "jitem.h"

class QPushButton;
class JLineEdit;
class JImageView;
class JDatabaseSelector;
class JDatabase;

class JDatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabasePicker(const QString& tableName,
                           const QString& text,
                           const QIcon& icon,
                           bool bMultiPicker = false,
                           QWidget* parent = nullptr);

  Id getId() const;
  const QVector<Id>& getIds() const;

  const QString m_text;

  void setPlaceholderText(bool bSet);
  JDatabase* getDatabase() const;

public slots:
  void clear();
  void setItems(const QVector<JItem*>& items);
  bool setItem(const JItem& jItem);
  bool setItem(Id id,
               const QString& name,
               const QByteArray& arImage);

private slots:
  void searchItem();

signals:
  void changedSignal();

private:
  bool m_bMultiPicker;
  JDatabaseSelector* m_selector;
  JLineEdit* m_edText;
  JImageView* m_imageView;
  QVector<Id> m_ids;
  QVector<QString> m_names;
};

#endif // JDATABASEPICKER_H
