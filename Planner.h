#ifndef PLANNER_H
#define PLANNER_H

#include <QTableWidget>

#include "EasyEstimate.h"

#include "Layer.h"

// Планировщик
// TODO - сделать вывод в таблицу через Friend-класс
class Planner
{
public:
  // приоритет оптимизации
  enum OptimizingPriority
  {
    MaxProductivity, // максимальная производительность
    MinRest          // минимальный остаток
  };

public:
  Planner();

  // возвращает ошибку в ходе последнего планирования
  // если ошибок не было - пустая строка
  QString error() const;

  // очистка массива прокатанных заготовок
  void clearRolledLengths();

  // добавить прокатанную заготовку
  void addRolledLength(Length length);

  // планирование
  bool plan2();

  // выдачи краткой текстовой справки о результатах планирования
  QString summary() const;

  // однородное планирование без проверки
  bool simplePlan(Count  layerCount,
                  Count  rodCount,
                  Length rodLength,
                  Count  sliceCount,
                  Length sliceLength );

  inline void setProductLengthMin(Length len) { m_productLengthMin = len; }

  inline void setProductLengthMax(Length len) { m_productLengthMax = len; }

  inline void setMaxRodCountInLayer(Count count) {m_maxRodCountInLayer = count; }

  inline void setCoolerLength(Length len) { m_coolerLength = len; }

  inline void setRodLengthMinLimit(Length len) { m_rodLengthMinLimit = len; }

  inline void setDiscardToSamples( Length len ) { m_discardToSamples = len; }

  inline void setFrontDiscard( Length len ) { m_frontDiscard = len; }

  inline void setRearDiscard( Length len ) { m_rearDiscard = len; }

  inline void setOptimizingPriority(OptimizingPriority priority);

  inline unsigned totalPlanCount() const;

  inline unsigned goodPlanCount() const;

  // вывод списка слоев
  void outLayerList(QTableWidget *table);

  // вывод в таблицу конкретного слоя
  void outLayer(QTableWidget *table, Index layerIndex);

  // TODO - может избавиться от этого метода?
  LayerVector *layers();

protected:

  // проверка исходных данных для планирования
  // возврашает false если есть ошибки
  // текст ошибки в
  bool validateSourceData();

  // сгенерировать план
  void generatePlan(Length rodLen, Length productLen, Count fullSliceCount);

  // завершить слой
  void finishLayer(Layer& layer, Length fullSliceLength, Count fullSliceCount);

  // проверка прокатанных заготовок на возможность разбиения на штанги длины rodLen
  bool validateRolledsForRodLen(Length rodLen);

  // оценить текущий план
  // возвращает false если план некорректен
  bool estimatePlan(EasyEstimate& estimate, Length rodLength, Length productLength);

  // ищет по вектору оценок лучший план
  Index findBestPlan();

  // суммарная длина прокатанных заготовок
  Length totalRolledLength() const;


protected:

  // ошибка
  QString m_error;

////////////////////////////////////////
/// ИСХОДНЫЕ ДАННЫЕ ДЛЯ ПЛАНИРОВАНИЯ ///
////////////////////////////////////////

  // длины прокатанных заготовок
  LengthVector m_rolledLengths;

  // минимальная коммерческая длина
  Length m_productLengthMin;

  // максимальная коммерческая длина
  Length m_productLengthMax;

  // максимальное количество штанг в слое
  Count  m_maxRodCountInLayer;

  // длина холодильника
  Length m_coolerLength;

  // минимальная длина штанги
  Length m_rodLengthMinLimit;

  // длина отреза на образцы
  Length m_discardToSamples;

  // длина фронтального отреза
  Length m_frontDiscard;

  // длина кормового отреза
  Length m_rearDiscard;

  // коеффициент уширения
  double m_temperatureCoeff;

  // приоритет оптимизации
  OptimizingPriority m_optimizingPriority;

////////////////////////////////////
/// ДАННЫЕ ПРОЦЕССА ПЛАНИРОВАНИЯ ///
////////////////////////////////////

  // слои - по сути - один план
  LayerVector m_layers;

  // вектор оценок планов
  EasyEstimateVector m_estimates;

  // оценка лучшего плана
  EasyEstimate m_bestPlanEstimate;

  // общее количество планов, рассмотренных в ходе планирования
  unsigned int m_totalPlanCount;

  // количество допустимых планов
  unsigned int m_goodPlanCount;

  qint64 m_timeElapsed;
};

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


void Planner::setOptimizingPriority(Planner::OptimizingPriority priority)
{
  m_optimizingPriority = priority;
}


unsigned Planner::totalPlanCount() const
{
  return m_totalPlanCount;
}

unsigned Planner::goodPlanCount() const
{
  return m_goodPlanCount;
}



#endif // PLANNER_H
