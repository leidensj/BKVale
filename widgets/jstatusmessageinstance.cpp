#include "jstatusmessageinstance.h"

QLabel* JStatusMessageInstance::m_label = nullptr;

QLabel* JStatusMessageInstance::getInstance()
{
  if (m_label == nullptr)
    m_label = new QLabel;
  return m_label;
}

JStatusMessageInstance::JStatusMessageInstance()
{

}
