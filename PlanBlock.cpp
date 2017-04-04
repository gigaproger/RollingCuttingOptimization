#include "PlanBlock.h"

PlanBlock::PlanBlock()
{
  m_type = Product;
}

PlanBlock::PlanBlock(Type type, Count count, Length length)
{
  m_type = type;

  for (int i = 0; i < count; ++i)
  {
    m_items.push_back(length);
  }
}

PlanBlock::PlanBlock(const PlanBlock &other)
{
  copy(other);
}

PlanBlock &PlanBlock::operator=(const PlanBlock &other)
{
  copy(other);
  return *this;
}

Length PlanBlock::totalLength() const
{
  Length ret = 0;

  LengthVector::const_iterator i;

  for (i = m_items.cbegin(); i != m_items.cend(); ++i)
  {
    ret += *i;
  }

  return ret;
}


bool PlanBlock::addItem(Length length)
{
  m_items.push_back(length);
  return true;
}

void PlanBlock::copy(const PlanBlock &other)
{
  m_type  = other.m_type;
  m_items = other.m_items;
}

