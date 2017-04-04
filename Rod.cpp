#include "Rod.h"

Rod::Rod()
{
  m_length = 0;
}

Rod::Rod(Length length)
{
  m_length = length;
}

Rod::Rod(const Rod &other)
{
  copy(other);
}

