#ifndef JDATABASEPICKER_H
#define JDATABASEPICKER_H

#include <QFrame>
#include <QString>
#include <category.h>
#include <QSqlDatabase>
#include "jitem.h"

class QPushButton;
class JLineEdit;
class JImageView;
class JDatabaseSelector;

class JDatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabasePicker(const QString& text,
                           const QIcon& icon,
                           bool bShowImage = false,
                           bool bRequired = false,
                           bool bDisplayGroup = true,
                           QWidget* parent = 0);

  qlonglong getId() const;
  const QString m_text;

  void setDatabase(QSqlDatabase db,
                   const QString& tableName);
  void setUserFilter(const QString& userFilter);

public slots:
  void clear();
  void setItem(const JItem& jItem);
  void setItem(qlonglong id,
               const QString& name,
               const QByteArray& arImage);

private slots:
  void searchItem();

signals:
  void changedSignal();

private:
  qlonglong m_id;
  JDatabaseSelector* m_selector;
  QPushButton* m_btnSearch;
  JLineEdit* m_edText;
  QPushButton* m_btnClear;
  JImageView* m_imageView;
};

#endif // JDATABASEPICKER_H
