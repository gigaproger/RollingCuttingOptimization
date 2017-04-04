#ifndef RODGENERATEINFO_H
#define RODGENERATEINFO_H

#include "PlanTypes.h"

// TODO - убрать
class RodGenerateInfo
{
public:
  RodGenerateInfo();

protected:
  Length       m_rodLength;
  LengthVector m_rolledRests;
  Index        m_finishRolledIndex;
};

typedef std::vector<RodGenerateInfo> RodGenerateInfoVector;

#endif // RODGENERATEINFO_H
