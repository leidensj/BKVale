#ifndef JPICKER_H
#define JPICKER_H

#include <QFrame>
#include <QString>
#include <category.h>

class QPushButton;
class JLineEdit;
class JImageView;

class JPicker : public QFrame
{
  Q_OBJECT

public:
  explicit JPicker(const QString& itemText,
                   bool bShowImage = false,
                   bool bRequired = false,
                   bool bDisplayGroup = true,
                   QWidget* parent = 0);
  const QString m_itemText;
  void setId(qlonglong id);
  void setText(const QString& text);
  void setImage(const QByteArray& ar);
  qlonglong getId() const;
  QString getText() const;
  QByteArray getImage() const;

signals:
  void searchSignal();

public slots:
  void clear();

private slots:
  void emitSearchSignal();

private:
  qlonglong m_id;
  QPushButton* m_btnSearch;
  JLineEdit* m_edText;
  QPushButton* m_btnClear;
  JImageView* m_imageView;
};

#endif // JDATABASEPICKER_H
