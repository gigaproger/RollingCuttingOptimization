#include "Slice.h"


Slice::Slice()
{
  m_length = 0;
}

Slice::Slice(Length length)
{
  m_length = length;
}

Slice::Slice(const Slice &other)
{
  copy(other);
}

