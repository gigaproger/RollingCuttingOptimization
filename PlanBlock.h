#ifndef PLANBLOCK_H
#define PLANBLOCK_H

#include "PlanTypes.h"

// Блок раскройного плана
class PlanBlock
{
public:

  // типы блоков
  enum Type
  {
    Product,      // блок конечной продукции
    Sample,       // образцы
    FrontDiscard, // обрезь переднего края
    RearDiscard,  // обрезь заднего края
    Delimiter     // блок-разделитель
  };

public:
  PlanBlock();

  PlanBlock(Type type, Count count, Length length);

  PlanBlock(const PlanBlock& other);

  PlanBlock &operator=(const PlanBlock& other);

  // количество элементов в блоке
  inline Count itemCount() const;

  // общая длина блока
  Length totalLength() const;

  // возвращает тип блока
  inline Type type() const;

  // возвращает true, если блок является разделителем
  inline bool isDelimiter() const;

  // возвращает длину элемента блока по индексу элемента
  inline Length lengthAt(int index) const;

  // добавление пункта
  bool addItem(Length length);

protected:
  // копирование в себя другого блока
  void copy(const PlanBlock& other);

protected:

  // тип блока
  Type m_type;

  // элементы блока
  LengthVector m_items;

};

typedef std::vector<PlanBlock> PlanBlockVector;

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Count PlanBlock::itemCount() const
{
  return m_items.size();
}

PlanBlock::Type PlanBlock::type() const
{
  return m_type;
}

bool PlanBlock::isDelimiter() const
{
  return (m_type == Delimiter);
}

Length PlanBlock::lengthAt(int index) const
{
  if ( index < 0 || index >= m_items.size() )
  {
    return INVALID_LENGTH;
  }

  return m_items[index];
}



#endif // PLANBLOCK_H
