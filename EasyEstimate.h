#ifndef EASYESTIMATE_H
#define EASYESTIMATE_H

#include "PlanTypes.h"

// Простая оценка плана
class EasyEstimate
{
public:

  EasyEstimate();

  EasyEstimate(Length restLength, Length productLength, Length rodLength);

  EasyEstimate(const EasyEstimate& other);

  inline EasyEstimate& operator=(const EasyEstimate& other);

  inline Length restLength() const;

  inline Length productLength() const;

  inline Length rodLength() const;

protected:
  void copy(const EasyEstimate& other);

protected:
  // коммерческая длина
  Length m_productLength;

  // длина штанги
  Length m_rodLength;

  // суммарный размер остатка
  Length m_restLength;
};

typedef std::vector<EasyEstimate> EasyEstimateVector;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EasyEstimate &EasyEstimate::operator=(const EasyEstimate &other)
{
  copy(other);
  return *this;
}

Length EasyEstimate::restLength() const
{
  return m_restLength;
}

Length EasyEstimate::productLength() const
{
  return m_productLength;
}

Length EasyEstimate::rodLength() const
{
  return m_rodLength;
}


#endif // EASYESTIMATE_H
