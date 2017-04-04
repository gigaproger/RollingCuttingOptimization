#ifndef ROD_H
#define ROD_H

#include "PlanTypes.h"

// Штанга
class Rod
{
public:
  Rod();
  Rod(Length length);
  Rod(const Rod &other);

  inline Rod& operator=(const Rod &other);

  inline void setLength(Length length);
  inline Length length() const;

protected:
  inline void copy(const Rod &other);

protected:
  Length m_length;
};

typedef std::vector<Rod> RodVector;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Rod::copy(const Rod &other)
{
  m_length = other.m_length;
}

Rod &Rod::operator=(const Rod &other)
{
  copy(other);
  return *this;
}

void Rod::setLength(Length length)
{
  m_length = length;
}

Length Rod::length() const
{
  return m_length;
}



#endif // ROD_H
