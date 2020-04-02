#ifndef JFILTER_H
#define JFILTER_H

#include <QString>
#include <QWidget>

class QPushButton;
class QHBoxLayout;
class QFrame;

class JFilter : public QWidget
{
  Q_OBJECT

public:
  explicit JFilter(QWidget* parent = nullptr);
  virtual QString getFilter() const = 0;

public slots:
  virtual void clear() = 0;

private slots:
  void emitFilterChangedSignal();

protected:
  QHBoxLayout* m_ltButton;
  QPushButton* m_btnClear;
  QFrame* m_fr;

signals:
  void filterChangedSignal(const QString& filter);
};

#endif // JFILTER_H
