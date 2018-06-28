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
class QListWidget;

class JDatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabasePicker(const QString& tableName,
                           const QString& text,
                           const QIcon& icon,
                           bool bShowImage,
                           bool bDisplayGroup,
                           bool bMultipicker,
                           QWidget* parent = nullptr);

  qlonglong getId() const;
  QVector<qlonglong> getIds() const;

  const QString m_text;

  JDatabase* getDatabase() const;

public slots:
  void clear();
  void clearAll();
  void setItem(const JItem& jItem);
  void setItem(qlonglong id,
               const QString& name,
               const QByteArray& arImage);

private slots:
  void searchItem();

signals:
  void changedSignal();

private:
  bool m_bMultipicker;
  JDatabaseSelector* m_selector;
  QPushButton* m_btnSearch;
  JLineEdit* m_edText;
  QPushButton* m_btnClear;
  JImageView* m_imageView;
  QListWidget* m_list;
};

#endif // JDATABASEPICKER_H
