#include "Plan.h"
#include "LenAccum.h"

Plan::Plan()
{

}

Plan::Plan(const Plan &other)
{
  copy(other);
}

Plan &Plan::operator=(const Plan &other)
{
  copy(other);
  return *this;
}

void Plan::clear()
{
  m_blocks.clear();
  m_interims.clear();
}

Count Plan::coldCutterActionCount() const
{
  Count ret = -1;
  Count continuousPackCount = 0;

  for (int i = 0; i < m_blocks.size(); ++i)
  {
    if ( m_blocks[i].isDelimiter() )
    {
      if (continuousPackCount > 1)
      {
        ret += continuousPackCount - 1;
      }

      continuousPackCount = 0;
    }

    continuousPackCount++;
  }

  return ret;
}

Length Plan::totalLength() const
{
  Length ret = 0;

  for (int i = 0; i < m_blocks.size(); ++i)
  {
    ret += m_blocks[i].totalLength();
  }

  return ret;
}

Count Plan::maxItemCountInBlock() const
{
  Count ret = 0;

  for (int i = 0; i < m_blocks.size(); ++i)
  {
    if ( m_blocks[i].itemCount() > ret )
      ret = m_blocks[i].itemCount();
  }

  return ret;
}


bool Plan::makeUniform(Length wholeLength, Length itemLength, Count maxItemCountInBlock, Length maxInterimLength)
{
  PlanBlock delimiter(PlanBlock::Delimiter, 0, 0);
  Count  blockInPackCount;
  Count  fullPackCount;
  Length fullPackTotalLength;
  Length fullBlockLength;
  Length rest;
  int i;
  int j;

  clear();

  if ( itemLength == 0 )
    return false;

  if (itemLength > maxInterimLength)
    return false;

  if ( wholeLength == 0)
    return true;

  if (maxItemCountInBlock <= 0)
    return true;

  // считаем количество блоков в полном пакете
  blockInPackCount = maxInterimLength / itemLength;

  // считаем суммарную длину полностью заполненного пакета
  fullPackTotalLength = maxItemCountInBlock * blockInPackCount * itemLength;

  // считаем количество полностью заполненных пакетов
  fullPackCount = wholeLength / fullPackTotalLength;


  // добавляем в план полностью заполненные пакеты
  {
    PlanBlock block(PlanBlock::Product, maxItemCountInBlock, itemLength);

    for (i = 0; i < fullPackCount; ++i)
    {
      for(j = 0; j < blockInPackCount; ++j)
      {
        m_blocks.push_back(block);
      }

      m_blocks.push_back(delimiter);
    }
  }

  // остаток укладываем поблочно
  rest = wholeLength - fullPackCount * fullPackTotalLength;
  fullBlockLength = maxItemCountInBlock * itemLength;

  PlanBlock fullBlock(PlanBlock::Product, maxItemCountInBlock, itemLength);

  while ( rest > 0 )
  {
    if ( rest >= fullBlockLength )
    {
      m_blocks.push_back( fullBlock );
      rest -= fullBlockLength;
    }
    else
    {

      Count itemCount = rest / itemLength;
      PlanBlock block(PlanBlock::Product, itemCount, itemLength);

      if ( !block.addItem(rest % itemLength) )
      {
        clear();
        return false;
      }

      m_blocks.push_back(block);

      rest = 0;
    }
  }

  // удаляем крайний блок-разделитель если он есть
  if ( m_blocks.size() > 0 )
  {
    if ( m_blocks.back().isDelimiter() )
    {
      m_blocks.pop_back();
    }
  }

  // обновляем вектор промежуточных заготовок
  makeIterimVector();

  return true;
}

void Plan::join(const Plan &other, bool withDelimiter)
{
  if (other.isEmpty() )
    return;

  if ( withDelimiter )
  {
    m_blocks.push_back(PlanBlock(PlanBlock::Delimiter, 0, 0));
  }

  for (int i = 0; i < other.m_blocks.size(); ++i)
  {
    m_blocks.push_back( other.m_blocks[i] );
  }
}


void Plan::copy(const Plan &other)
{
  m_blocks = other.m_blocks;
  m_interims = other.m_interims;
}

bool Plan::makeIterimVector()
{
  LenAccumVector accums;
  PlanBlockVector::const_iterator bi;
  int i, j, delta;
  Length length;

  // TODO - а может сделать проверку целостности по ходу?

  // TODO - если медленно работает -
  // можно оптимизировать, отказавшись от accums

  m_interims.clear();

  if ( m_blocks.empty() )
    return true;

  for (bi = m_blocks.cbegin(); bi != m_blocks.cend(); ++bi)
  {
    const PlanBlock& block = *bi;

    if ( block.type() == PlanBlock::Delimiter )
    {
      PlanBlock interimPlanBlock(PlanBlock::Delimiter, 0, 0);
      PlanBlock delimiter(PlanBlock::Delimiter, 0, 0);

      // сливаем аккумуляторы в вектор промежуточных заготовок
      for (i = 0; i < accums.size(); ++i)
      {
        interimPlanBlock.addItem( accums[i].length() );
      }

      m_interims.push_back( interimPlanBlock );
      m_interims.push_back( delimiter );

      // сбрасываем аккумуляторный пакет
      accums.clear();
    }
    else
    {
      // добавляем недостающие аккумуляторы
      if ( block.itemCount() > accums.size() )
      {
        delta = block.itemCount() - accums.size();

        for (i = 0; i < delta; ++i)
        {
          accums.push_back( LenAccum() );
        }
      }

      // аккумулируем длины
      for (i = 0; i < block.itemCount(); ++i)
      {
        LenAccum &accum = accums[i];

        length = block.lengthAt(i);

        // разрыв заготовки в пределах пакета
        if ( accum.finished() && ( length > 0 ) )
        {
          return false;
        }
        // заготовка закончилась
        else if ( !accum.finished() && (accum.length() > 0) && (length == 0) )
        {
          accum.setFinished();
        }
        // заготовка продолжается - добавляем длину
        else if ( !accum.finished() && ( length > 0 ) )
        {
          accum.addLength( length );
        }
      }
    }
  }

  return true;
}

