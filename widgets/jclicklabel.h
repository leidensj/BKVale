#ifndef JCLICKLABEL_H
#define JCLICKLABEL_H

#include <QLabel>
#include <QWidget>

class JClickLabel : public QLabel
{
  Q_OBJECT

public:
  explicit JClickLabel(QWidget* parent = nullptr);

signals:
  void clicked();

protected:
  void mousePressEvent(QMouseEvent* event);

};

#endif // JCLICKLABEL_H
