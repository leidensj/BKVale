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
class JDatabase;

class JDatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabasePicker(const QString& tableName,
                           const QString& text,
                           const QIcon& icon,
                           bool bShowImage,
                           bool bDisplayGroup,
                           QWidget* parent = 0);

  qlonglong getId() const;
  const QString m_text;

  JDatabase* getDatabase() const;

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
