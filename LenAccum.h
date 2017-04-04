#ifndef LENACCUM_H
#define LENACCUM_H

#include "PlanTypes.h"

// Накопитель длины
class LenAccum
{
public:
  LenAccum();
  LenAccum(Length startLength);
  LenAccum(const LenAccum &other);
  LenAccum &operator=(const LenAccum &other);

  // возвращает длину
  inline Length length() const;

  // добавление длины
  inline void addLength(Length length);

  // установка флага завершения накопления длины
  inline void setFinished();

  // возвращает true, если накопление длины завершено
  inline bool finished() const;

protected:
  inline void copy(const LenAccum &other);

protected:
  // накопленная длина
  Length m_length;

  // если true, то накопление длины закончено
  bool   m_isFinished;
};

typedef std::vector<LenAccum> LenAccumVector;

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


Length LenAccum::length() const
{
  return m_length;
}

void LenAccum::addLength(Length length)
{
  m_length += length;
}

void LenAccum::setFinished()
{
  m_isFinished = true;
}

bool LenAccum::finished() const
{
  return m_isFinished;
}

void LenAccum::copy(const LenAccum &other)
{
  m_length = other.m_length;
  m_isFinished = other.m_isFinished;
}

#endif // LENACCUM_H
