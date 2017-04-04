#ifndef ROLLED_H
#define ROLLED_H


// Прокатанная заготовка
class Rolled
{
public:
  Rolled();
  Rolled(const Rolled& other);
  Rolled &operator=(const Rolled& other);

protected:
  void copy(const Rolled& other);


};

#endif // ROLLED_H
