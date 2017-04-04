#include "Layer.h"

Layer::Layer()
{
  clear();
}

Layer::Layer(const Layer &other)
{
  copy(other);
}

void Layer::addRod(Length length)
{
  m_rods.push_back( Rod(length) );
}

void Layer::addSlice(Length length)
{
  m_slices.push_back( Slice(length) );
}

void Layer::addLengthToRodAt(Index index, Length length)
{
  if ( index >= m_rods.size() )
    return;

  m_rods[index].setLength( m_rods[index].length() + length );

}

Length Layer::rodLengthAt(Index index) const
{
  if ( index >= m_rods.size())
  {
    return INVALID_LENGTH;
  }

  return m_rods[index].length();
}

Length Layer::sliceLengthAt(Index index) const
{
  if ( index >= m_slices.size())
  {
    return INVALID_LENGTH;
  }

  return m_slices[index].length();
}

bool Layer::sliceItems(Index sliceIndex, LengthVector &items) const
{
  Length minRodLen;
  Length cutSumLen;
  Length sliceLen;

  size_t i;

  if (sliceIndex >= sliceCount() )
    return false;

  minRodLen = minRodLength();

  items.clear();

  if ( sliceIndex == cutCount() )
  {
    cutSumLen = cutSumLengthAt( cutCount() - 1 );
  }
  else
  {
    cutSumLen = cutSumLengthAt( sliceIndex );
  }

  if ( cutSumLen == INVALID_LENGTH )
  {
    return false;
  }

  sliceLen = m_slices[sliceIndex].length();

  for (i = 0; i < rodCount(); ++i )
  {
    Length itemLength;

    if ( m_rods[i].length() <= cutSumLen )
    {
      itemLength = INVALID_LENGTH;
    }
    else if ( (m_rods[i].length() > cutSumLen) &&
              (m_rods[i].length() < cutSumLen + sliceLen ) )
    {
      itemLength = m_rods[i].length() - cutSumLen;
    }
    else
    {
      itemLength =  sliceLen;
    }

    items.push_back( itemLength );
  }

  return true;
}

Length Layer::sliceItemTotalLength(Index sliceIndex) const
{
  LengthVector items;

  if ( !sliceItems(sliceIndex, items) )
    return 0;

  Length ret = 0;

  for (int i = 0; i < items.size(); ++i)
  {
    ret += items[i];
  }

  return ret;
}

Length Layer::totalRodLength() const
{
  Length ret = 0;

  for (size_t i = 0; i < m_rods.size(); ++i)
  {
    ret += m_rods[i].length();
  }

  return ret;
}

Length Layer::totalSliceLength() const
{
  Length ret = 0;

  for (size_t i = 0; i < m_slices.size(); ++i)
  {
    ret += m_slices[i].length();
  }

  return ret;
}

Length Layer::cutSumLengthAt(Index orderNumber) const
{
  Length ret;

  if ( cutCount() == 0 )
  {
    return INVALID_LENGTH;
  }

  if ( orderNumber >= cutCount() )
  {
    return INVALID_LENGTH;
  }

  ret = 0;

  for ( size_t i = 0; i <= orderNumber ; ++i )
  {
    ret += m_slices[i].length();
  }

  return ret;
}

Length Layer::lastFullCutSumLength() const
{
  Length ret;

  Length minRodLen;

  if ( cutCount() == 0 )
  {
    return INVALID_LENGTH;
  }

  minRodLen = minRodLength();

  if ( minRodLen == INVALID_LENGTH )
  {
    return INVALID_LENGTH;
  }

  ret = 0;

  for (Index i = 0; i < cutCount(); ++i )
  {
    if ( ret + m_slices[i].length() >= minRodLen )
      break;

    ret += m_slices[i].length();
  }

  return ret;
}

bool Layer::lastFullCutOrderNumber(Index &orderNumber) const
{
  Length lenAccum;
  Length minRodLen;
  Index i;

  if ( cutCount() == 0 )
  {
    return false;
  }

  minRodLen = minRodLength();

  if ( minRodLen == INVALID_LENGTH )
  {
    return false;
  }

  lenAccum = 0;

  for (i = 0; i < sliceCount(); ++i )
  {
    if ( lenAccum + m_slices[i].length() >= minRodLen )
      break;

    lenAccum += m_slices[i].length();
  }

  orderNumber = (i == sliceCount() ? i - 1 : i);

  return true;
}

Length Layer::lastCutSumLength() const
{
  return cutSumLengthAt( cutCount() - 1 );
}

Length Layer::minRodLength() const
{
  Length ret;

  if (m_rods.size() == 0)
  {
    ret = INVALID_LENGTH;
  }
  else
  {
    ret = m_rods[0].length();

    for (size_t i = 1; i < m_rods.size(); ++i)
    {
      if (ret > m_rods[i].length() )
      {
        ret = m_rods[i].length();
      }
    }
  }

  return ret;
}

Length Layer::maxRodLength() const
{
  Length ret;

  if (m_rods.size() == 0)
  {
    ret = INVALID_LENGTH;
  }
  else
  {
    ret = m_rods[0].length();

    for (size_t i = 1; i < m_rods.size(); ++i)
    {
      if (ret < m_rods[i].length() )
      {
        ret = m_rods[i].length();
      }
    }
  }

  return ret;
}

bool Layer::check()
{

  if ( (cutCount() > 0) && (rodCount() == 0) )
  {
    return false;
  }

  if (maxRodLength() != totalSliceLength() )
  {
    return false;
  }

/*  if ( lastCutSumLength() >= maxRodLength() )
  {
    return false;
  }
*/

  return true;
}

