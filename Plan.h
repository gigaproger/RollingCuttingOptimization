#ifndef PLAN_H
#define PLAN_H

#include "PlanBlock.h"

// раскройный план
class Plan
{
public:
  Plan();

  Plan(const Plan &other);

  Plan &operator=(const Plan& other);

  // очистка плана - приведение его к пустому состоянию
  void clear();

  // возвращает количество резов холодными ножницами
  Count coldCutterActionCount() const;

  // возвращает общую длину плана
  Length totalLength() const;

  inline Count blockCount() const;

  inline bool itemCountInBlockAt(int blockIndex, Count &count) const;

  inline bool blockTypeAt(int blockIndex, PlanBlock::Type &type) const;

  // проверка целостности плана
  bool checkIntegrity();

  // возвращает true, если план пуст
  inline bool isEmpty() const;


  // возвращает данные о длине пункта блока по индексам
  inline bool itemLengthAt(int blockIndex,
                           int itemIndex,
                           Length &length) const;


  // возвращает данные о пункте блока по индексам
  inline bool itemAt(int blockIndex,
                     int itemIndex,
                     Length &length,
                     PlanBlock::Type &type) const;


  // максимальное кол-во пунктов в блоке
  Count maxItemCountInBlock() const;


  // построение однородного плана
  bool makeUniform(Length wholeLength,
                   Length itemLength,
                   Count maxItemCountInBlock,
                   Length maxInterimLength);


  // присоединение к себе другого плана
  void join(const Plan &other, bool withDelimiter);

protected:
  void copy(const Plan &other);

  bool makeIterimVector();

protected:
  // блоки конечных заготовок
  PlanBlockVector m_blocks;

  // блоки промежуточных заготовок
  // каждый блок - это то что помещается на рольганг
  // и режется холодными ножницами совместно
  PlanBlockVector  m_interims;
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Count Plan::blockCount() const
{
  return m_blocks.size();
}

bool Plan::itemCountInBlockAt(int blockIndex, Count &count) const
{
  if ( (blockIndex < 0) || (blockIndex >= m_blocks.size()) )
  {
    return false;
  }

  count = m_blocks[blockIndex].itemCount();
  return true;
}

 bool Plan::blockTypeAt(int blockIndex, PlanBlock::Type &type) const
{
  if ( (blockIndex < 0) || (blockIndex >= m_blocks.size()) )
  {
    return false;
  }

  type = m_blocks[blockIndex].type();
  return true;
}

bool Plan::isEmpty() const
{
  return m_blocks.empty();
}

bool Plan::itemLengthAt(int blockIndex, int itemIndex, Length &length) const
{
  Length len;

  if ( (blockIndex < 0) || (blockIndex >= m_blocks.size()) )
  {
    return false;
  }

  const PlanBlock &block = m_blocks.at(blockIndex);

  if ( block.isDelimiter() )
  {
    length = 0;
  }
  else
  {
    len = block.lengthAt(itemIndex);

    if ( len == INVALID_LENGTH )
      return false;

    length = len;
  }

  return true;
}

bool Plan::itemAt(int blockIndex, int itemIndex, Length &length, PlanBlock::Type &type) const
{
  Length len;

  if ( (blockIndex < 0) || (blockIndex >= m_blocks.size()) )
  {
    return false;
  }

  const PlanBlock &block = m_blocks.at(blockIndex);

  type = block.type();


  if ( block.isDelimiter() )
  {
    length = 0;
  }
  else
  {
    len = block.lengthAt(itemIndex);

    if ( len == INVALID_LENGTH )
      return false;

    length = len;
  }

  return true;
}


#endif // PLAN_H
