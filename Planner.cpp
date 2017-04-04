#include <QTableWidgetItem>
#include <QElapsedTimer>
#include "Planner.h"


Planner::Planner()
{
  m_productLengthMin = 0;

  m_productLengthMax = 0;

  m_maxRodCountInLayer = 0;

  m_coolerLength = 0;

  m_rodLengthMinLimit = 0;

  m_discardToSamples = 0;

  m_frontDiscard = 0;

  m_rearDiscard = 0;

  m_temperatureCoeff = 0.013;

  m_optimizingPriority = MinRest;
}

QString Planner::error() const
{
  return m_error;
}

void Planner::clearRolledLengths()
{
  m_rolledLengths.clear();
}

void Planner::addRolledLength(Length length)
{
  m_rolledLengths.push_back(length);
}

bool Planner::simplePlan(Count layerCount, Count rodCount, Length rodLength, Count sliceCount, Length sliceLength)
{
  m_layers.clear();

  Index i, j;
  Length sumLengh;

  for (i = 0; i < layerCount; ++i)
  {
    Layer layer;

    for (j = 0; j < rodCount; ++j)
    {
      layer.addRod( rodLength );
    }

    sumLengh = 0;

    for (j = 0; j < sliceCount; ++j)
    {
      if ( sumLengh + sliceLength < rodLength )
      {
        layer.addSlice( sliceLength );
        sumLengh += sliceLength;
      }
      else
      {
        break;
      }
    }

    layer.addSlice( rodLength - sumLengh  );

    m_layers.push_back(layer);
  }

  return true;
}


bool Planner::plan2()
{
  QElapsedTimer timer;

  timer.start();

  m_error.clear();

  // проверяем исходные данные
  if ( !validateSourceData() )
    return false;

  // сбрасываем вектор оценок
  m_estimates.clear();

  Length productLen; // коммерческая длина

  Length rodForProdLen; // длина части штанги, идущая в продукцию

  Length rodLen; // длина всей штанги

  EasyEstimate estimate; // оценка плана

  m_totalPlanCount = 0;
  m_goodPlanCount = 0;


  // проходим по диапазону допустимой коммерческой длины с шагом 1 см
  for ( productLen = m_productLengthMin;
        productLen <= m_productLengthMax;
        ++productLen )
  {
    // ищем минимально допустимую длину штанги, кратную коммерческой длине
    rodForProdLen = 0;

    while( rodForProdLen < m_rodLengthMinLimit )
    {
      rodForProdLen += productLen;
    }

    // проходим по всем допустимым кратностям коммерческой длины
    for (; rodForProdLen < m_coolerLength; rodForProdLen += productLen)
    {
      rodLen = rodForProdLen + m_frontDiscard + m_rearDiscard + m_discardToSamples;

      // проверяем прокатанные заготовки на возможность разреза на штангу размера rodLen
      if ( !validateRolledsForRodLen(rodLen) )
      {
        continue;
      }

      // генерируем план
      generatePlan( rodLen, productLen, rodForProdLen / productLen );

      m_totalPlanCount++;

      // оцениваем план, если план допустим, то
      // записываем оценку в вектор
      if ( estimatePlan( estimate, rodLen, productLen  ) )
      {
        m_goodPlanCount++;
        m_estimates.push_back( estimate );
      }
    }

  }

  if ( m_estimates.empty() )
  {
    m_error = "Не удалось сформировать ни одного допустимого плана.";
    return false;
  }

  // выбор лучшего плана
  Index bestEstimateIndex = findBestPlan();

  // еще раз отстраиваем лучший план
  rodLen = m_estimates[bestEstimateIndex].rodLength();

  productLen = m_estimates[bestEstimateIndex].productLength();

  generatePlan(rodLen, productLen, (rodLen - m_frontDiscard - m_rearDiscard - m_discardToSamples) / productLen );

  m_bestPlanEstimate = m_estimates[bestEstimateIndex];

  m_estimates.clear();

  m_timeElapsed = timer.elapsed();

  return true;
}

QString Planner::summary() const
{
  QString ret;
  QString line;

  if ( !m_error.isEmpty() )
    return QString();

  line.sprintf("Обработано вариантов:\t\t%d\n", m_totalPlanCount);
  ret += line;

  line.sprintf("Допустимых вариантов:\t\t%d\n", m_goodPlanCount);
  ret += line;

  if ( m_timeElapsed == 0 )
  {
    line = "Время работы, мс:\t\t< 1\n";
  }
  else
  {
    line.sprintf("Время работы, мс:\t\t%lld\n", m_timeElapsed );
  }

  ret += line;

  line.sprintf("Коммерческая длина, м:\t\t%.2f\n", (double)m_bestPlanEstimate.productLength() / 100.0);
  ret += line;

  line.sprintf("Длина проката, м:\t\t%.2f\n", (double)totalRolledLength() / 100.0);
  ret += line;

  line.sprintf("Длина обрези, м:\t\t%.2f\n", (double)m_bestPlanEstimate.restLength()  / 100.0);
  ret += line;

  return ret;
}

void Planner::outLayerList(QTableWidget *table)
{
  Index i;
  QString text;
  QTableWidgetItem *cell;

  if ( table == 0 )
  {
    return;
  }

  table->clear();

  table->setRowCount(m_layers.size());

  table->setColumnCount(1);

  for (i = 0; i < m_layers.size(); ++i)
  {
    text.sprintf("%d", m_layers[i].totalRodLength() );

    cell = new QTableWidgetItem(text);

    table->setItem(i, 0, cell);
  }

  QStringList labels;
  labels << "Суммарная длина, см";
  table->setHorizontalHeaderLabels(labels);
  table->resizeColumnsToContents();
}

void Planner::outLayer(QTableWidget *table, Index layerIndex)
{
  Index i;
  Index j;
  QString text;
  QTableWidgetItem *cell;
  LengthVector items;
  Length len;
  QColor color;

  if ( table == 0 )
  {
    return;
  }

  if (layerIndex >= m_layers.size() )
  {
    return;
  }

  table->clear();

  Layer &layer = m_layers.at( layerIndex );

  table->setRowCount( layer.rodCount() );

  table->setColumnCount( layer.sliceCount() + 1);

  // выводим штанги в первую колонку
  for ( i = 0; i < layer.rodCount(); ++i )
  {
    text.sprintf("%d", layer.rodLengthAt(i) );

    cell = new QTableWidgetItem( text );

    cell->setBackgroundColor( QColor(192, 192, 255) );

    table->setItem( i, 0, cell );
  }

  for ( i = 0; i < layer.sliceCount(); ++i )
  {
    if ( !layer.sliceItems(i, items) )
    {
      break;
    }

    for ( j = 0; j < items.size(); ++j )
    {
      len = items[j];


      if ( len == INVALID_LENGTH )
      {
        text.clear();
        color = Qt::gray;
      }
      else
      {
        text.sprintf("%d", len );
        color = Qt::white;
      }

      cell = new QTableWidgetItem( text );

      cell->setBackgroundColor( color );

      table->setItem( j, i + 1, cell );
    }
  }

  QStringList labels;

  labels << "Штанга, см";

  for (i = 0; i < layer.sliceCount(); ++i)
  {
    text.sprintf("Срез %d", i + 1);
    labels << text;
  }

  table->setHorizontalHeaderLabels(labels);
  table->resizeColumnsToContents();
}

LayerVector* Planner::layers()
{
  return &m_layers;
}

void Planner::generatePlan(Length rodLen, Length productLen, Count fullSliceCount)
{
  m_layers.clear();

  int oi; // счетчик прокатанных
  int ri; // счетчик штанг

  Layer  layer; // формируемый слой

  Length lenRest; // остаток от длины прокатанной заготовки

  Index  firstRodIndex; // индекс первой штанги в текущем слое,
                        // которая нарезана из текущей прокатанной заготовки

  // проходим по всем прокатанным заготовкам
  for (oi = 0; oi < m_rolledLengths.size(); ++oi)
  {
    lenRest       = m_rolledLengths[oi];
    firstRodIndex = layer.rodCount();

    // добавляем полные заготовки
    while ( lenRest >= rodLen )
    {
      // если слой наполнился
      if ( layer.rodCount() == m_maxRodCountInLayer )
      {
        // заканчиваем формирование слоя
        finishLayer(layer, productLen, fullSliceCount);

        // добавляем его к результату
        m_layers.push_back(layer);

        // начинаем новый слой
        layer.clear();
        firstRodIndex = 0;
      }

      // добавляем к слою заготовку
      layer.addRod(rodLen);

      // остаток прокатанной заготовки уменьшаем
      lenRest -= rodLen;
    }

    // распределяем остаток по заготовкам
    if ( layer.rodCount() > 0 )
    {
      // на сколько штанг распределяется остаток
      int rodCountForRestDistrib = layer.rodCount() - firstRodIndex;

      for ( ri = firstRodIndex; ri < layer.rodCount(); ++ri )
      {
        layer.addLengthToRodAt(ri,  lenRest / rodCountForRestDistrib );
      }

      // самую малость кладем в последнюю заготовку
      layer.addLengthToRodAt( layer.rodCount() - 1, lenRest % rodCountForRestDistrib );
    }
  }

  // заканчиваем формирование слоя
  finishLayer(layer, productLen, fullSliceCount);

  // добавляем его к результату
  m_layers.push_back(layer);


}

void Planner::finishLayer(Layer &layer, Length fullSliceLength, Count fullSliceCount)
{
  if ( m_frontDiscard > 0)
  {
    layer.addSlice(m_frontDiscard);
  }

  if ( m_discardToSamples > 0)
  {
    layer.addSlice(m_discardToSamples);
  }

  for (Count i = 0; i < fullSliceCount; ++i )
  {
    layer.addSlice(fullSliceLength);
  }

  Length maxRodLen = layer.maxRodLength();

  // последний слой
  layer.addSlice(maxRodLen - layer.totalSliceLength());
}

bool Planner::estimatePlan(EasyEstimate &estimate, Length rodLength, Length productLength)
{
  Index li;
  Index ri;
  Length totalRest = 0;

  // план из 0 слоев недопустим
  if ( m_layers.empty() )
    return false;

  for ( li = 0; li < m_layers.size(); ++li )
  {
    const Layer& layer = m_layers.at(li);

    if ( layer.maxRodLength() > m_coolerLength )
    {
      return false;
    }

    // TODO - проверить правильность
    totalRest += layer.rodCount() * m_frontDiscard + layer.sliceItemTotalLength( layer.sliceCount() - 1);
  }

  estimate = EasyEstimate(totalRest, productLength, rodLength);

  return true;
}

Index Planner::findBestPlan()
{
  Index ret = 0;
  Length minRest = m_estimates[0].restLength();

  for (Index i = 1; i < m_estimates.size(); ++i)
  {
    if ( minRest > m_estimates[i].restLength() )
    {
      minRest = m_estimates[i].restLength();
      ret = i;
    }
  }

  return ret;
}



Length Planner::totalRolledLength() const
{
  // TODO - убрать этот код в ОБЩУЮ функцию
  Length ret = 0;

  for (Index i = 0; i < m_rolledLengths.size(); ++i)
  {
    ret += m_rolledLengths[i];
  }

  return ret;
}
bool Planner::validateSourceData()
{
  Index i;

  if ( m_rolledLengths.empty() )
  {
    m_error = "Нет заготовок для разреза";
    return false;
  }

  if ( m_productLengthMin < MIN_PRODUCT_LEN)
  {
    m_error = "Недопустимая коммерческая длина" + QString::number(m_productLengthMin);
    return false;
  }

  if ( m_productLengthMin > m_productLengthMax )
  {
    m_error = "Максимальная коммерческая длина меньше минимальной коммерческой длины";
    return false;
  }

  // проверяем прокатанные заготовки
  for (i = 0; i < m_rolledLengths.size(); ++i)
  {
    if ( m_rolledLengths[i] < m_rodLengthMinLimit )
    {
      m_error.sprintf("Заготовка, порядковый номер в партии %d, имеет длину %d см меньше минимально допустимой %d см", i + 1, m_rolledLengths[i], m_rodLengthMinLimit );
      return false;
    }
  }

  return true;
}

bool Planner::validateRolledsForRodLen(Length rodLen)
{
  for (Index i = 0; i < m_rolledLengths.size(); ++i)
  {
    if ( m_rolledLengths[i] < rodLen )
    {
      return false;
    }
  }

  return true;
}

