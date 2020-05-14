#ifndef JSTATUSMESSAGEINSTANCE_H
#define JSTATUSMESSAGEINSTANCE_H

#include <QLabel>

class JStatusMessageInstance
{
public:
  static QLabel* getInstance();
private:
  static QLabel* m_label;
  JStatusMessageInstance();
};

#endif // JSTATUSMESSAGEINSTANCE_H
