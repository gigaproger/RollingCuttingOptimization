#ifndef SLICE_H
#define SLICE_H

#include "PlanTypes.h"

class Slice
{
public:
  Slice();
  Slice(Length length);
  Slice(const Slice &other);
  inline Slice& operator=(const Slice &other);

  inline void setLength(Length length);
  inline Length length() const;

protected:
  inline void copy(const Slice &other);

protected:
  Length m_length;
};

typedef std::vector<Slice> SliceVector;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Slice::copy(const Slice &other)
{
  m_length = other.m_length;
}

Slice &Slice::operator=(const Slice &other)
{
  copy(other);
  return *this;
}

void Slice::setLength(Length length)
{
  m_length = length;
}

Length Slice::length() const
{
  return m_length;
}



#endif // SLICE_H
