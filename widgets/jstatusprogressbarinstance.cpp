#include "jstatusprogressbarinstance.h"

QProgressBar* JStatusProgressBarInstance::m_progressBar = nullptr;

QProgressBar* JStatusProgressBarInstance::getInstance()
{
  if (m_progressBar == nullptr)
    m_progressBar = new QProgressBar;
  return m_progressBar;
}

JStatusProgressBarInstance::JStatusProgressBarInstance()
{

}
