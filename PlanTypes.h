#ifndef PLANSCALARS_H
#define PLANSCALARS_H

#include <vector>

// Длина в сантиметрах
typedef int Length;

const Length INVALID_LENGTH = -1;

// Ширина в мм
typedef double Width;

// масса в килограммах
typedef int Weight;

// количество, шт
typedef size_t Count;

// индекс
typedef size_t Index;

typedef std::vector<Length> LengthVector;


// минимально допустимая коммерческая длина, см
const Length MIN_PRODUCT_LEN = 200;

#endif // PLANSCALARS_H

