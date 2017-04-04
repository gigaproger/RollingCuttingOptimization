#include "LenAccum.h"

LenAccum::LenAccum()
{
  m_length = 0;
  m_isFinished = false;
}

LenAccum::LenAccum(Length startLength)
{
  m_length = startLength;
  m_isFinished = false;
}

LenAccum::LenAccum(const LenAccum &other)
{
  copy(other);
}

