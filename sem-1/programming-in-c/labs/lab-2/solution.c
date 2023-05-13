// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 2

#include <stdio.h>
#include <math.h>

typedef long double real;

const real calcZ1(const real m) {
  const real sqrtM = sqrt(m);
  const real firstConst = 3.0L*m + 2.0L;
  return sqrt((firstConst*firstConst) - 24.0L*m) / (3.0L*sqrtM - 2.0L/sqrtM);
}

const real calcZ2(const real m) {
  return -sqrt(m);
}

int main() {
  printf(
    "Required variables: `m`\n"
    "Formulas:\n"
    "  z1 = sqrt((3m + 2)^2 - 24m)/(3*sqrt(m) - 2/sqrt(m))\n"
    "  z2 = -sqrt(m)\n"
    "Enter `m`: ");
  fflush(stdout);

  // считываем переменную типа long double
  // (максимальный размер для переменной для работы с типом real)
  real m;
  scanf("%Lf", &m);

  // высчитываем по первой формуле и по второй,
  // а потом выводим с точностью 18 знаков после запятой
  const real z1 = calcZ1(m),
             z2 = calcZ2(m);

  printf(
    "z1 = %.18Lf\n"
    "z2 = %.18Lf\n",
    z1, z2);
  return 0;
}