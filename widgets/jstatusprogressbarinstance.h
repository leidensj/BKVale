#ifndef JSTATUSPROGRESSBARINSTANCE_H
#define JSTATUSPROGRESSBARINSTANCE_H


#include <QProgressBar>

class JStatusProgressBarInstance
{
public:
  static QProgressBar* getInstance();

private:
  static QProgressBar* m_progressBar;
  JStatusProgressBarInstance();
};

#endif // JSTATUSPROGRESSBARINSTANCE_H
