#include "EasyEstimate.h"

EasyEstimate::EasyEstimate()
{
  m_restLength = 0;
  m_productLength = 0;
  m_rodLength = 0;
}

EasyEstimate::EasyEstimate(Length restLength, Length productLength, Length rodLength)
{
  m_restLength = restLength;
  m_productLength = productLength;
  m_rodLength = rodLength;
}

EasyEstimate::EasyEstimate(const EasyEstimate &other)
{
  copy(other);
}


void EasyEstimate::copy(const EasyEstimate &other)
{
  m_restLength    = other.m_restLength;
  m_productLength = other.m_productLength;
  m_rodLength     = other.m_rodLength;
}

