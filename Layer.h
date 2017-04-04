#ifndef LAYER_H
#define LAYER_H

#include "Rod.h"
#include "Slice.h"

// TODO защиты от выхода за границу масссива
// TODO функция количества перестроений упора НХР
class Layer
{
public:
  Layer();
  Layer(const Layer &other);
  inline Layer& operator=(const Layer &other);

  // очистка слоя
  inline void clear();

  // добавить заготовку
  void addRod(Length length);

  // добавить рез
  void addSlice(Length length);

  // добавить длину штанге
  void addLengthToRodAt(Index index, Length length);

  Length rodLengthAt(Index index) const;

  Length sliceLengthAt(Index index) const;

  // выдача элементов среза
  bool sliceItems(Index sliceIndex, LengthVector &items) const;

  // суммарная длина элементов
  Length sliceItemTotalLength(Index sliceIndex) const;

  // суммарная длина штанг
  Length totalRodLength() const;

  // суммарная длина отрезов - по длине слоя
  // должна равняться длине максимальной заготовки
  Length totalSliceLength() const;

  // суммарная длина от начала слоя,
  // длина на которой делается рез с порядковым номером orderNumber
  // нумерация резов начинается с 0
  Length cutSumLengthAt(Index orderNumber) const;

  // суммарная длина от начала слоя
  // на которой делается последний полный срез,
  // то есть срез всех штанг, входящих в данный слой
  Length lastFullCutSumLength() const;

  // порядковый номер последнего полного среза
  bool lastFullCutOrderNumber(Index &orderNumber) const;

  Length lastCutSumLength() const;

  // количество штанг
  inline Count  rodCount() const;

  // количество отрезов
  inline Count  sliceCount() const;

  // количество резов
  inline Count  cutCount() const;

  // минимальная длина штанги
  Length minRodLength() const;

  // максимальная длина штанги
  Length maxRodLength() const;

  // проверка корректности слоя
  bool check();

protected:
  inline void copy(const Layer &other);

protected:
  RodVector   m_rods;
  SliceVector m_slices;
};

typedef std::vector<Layer> LayerVector;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Layer::copy(const Layer &other)
{
  m_rods = other.m_rods;
  m_slices = other.m_slices;
}

Layer &Layer::operator=(const Layer &other)
{
  copy(other);
  return *this;
}

void Layer::clear()
{
  m_rods.clear();
  m_slices.clear();
}

Count Layer::rodCount() const
{
  return m_rods.size();
}

Count Layer::sliceCount() const
{
  return m_slices.size();
}

Count Layer::cutCount() const
{
  if (m_slices.size() < 2)
    return 0;
  else
    return m_slices.size() - 1;
}


#endif // LAYER_H
