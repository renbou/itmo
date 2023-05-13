// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 4

#include <stdio.h>
#include <stdint.h>
#define BITS_IN_64 63

typedef int64_t int64;
typedef uint64_t uint64;
typedef uint8_t ubyte;
typedef ubyte bool;

int main() {
  // without if's? O_o

  const int64 left = 20;
  const int64 right = 30;
  const int64 rangeR = right - left + 1;

  printf(
    "[1] Check for range [%lld;%lld]\n"
    "[1.1] Enter number to check: ",
    left, right);
  fflush(stdout);

  int64 number;
  scanf("%lld", &number);

  // если эта переменная отрицательная, то значит, что число < левая_граница, не подходит
  const int64 checkIfLessThanLeft = number - left;
  // проверка на отрицательность через последний бит, т.к. в доп. коде он равен 1 если число < 0
  const bool firstCheck = ( (uint64)(checkIfLessThanLeft & (1ull << BITS_IN_64)) >> BITS_IN_64 );
  // если это число положительное, то число > правая_граница, не подходит
  const int64 checkIfNotMoreThanRight = checkIfLessThanLeft - rangeR;
  const bool secondCheck = ( (uint64)(checkIfNotMoreThanRight & (1ull << BITS_IN_64)) >> BITS_IN_64 );

  /* Варианты:
   * - 2 правды только если число меньше левой границы (оба числа отрицательны)
   * - 1 только когда число лежит в пределах границ (во второй раз получаем отриц. число)
   * - 0 когда число лежит правее правой границы
  */

  char * resultMap[3] =
    {"The number %lld is higher than the right boundary of range [%lld;%lld]\n\n",
     "The number %lld is in the range [%lld;%lld]\n\n",
     "The number %lld is less than the left boundary of range [%lld;%lld]\n\n"};

  printf("[1.2] Result of comparison:\n");
  printf(resultMap[firstCheck + secondCheck], number, left, right);

  const ubyte bit = 17;

  // переменная bit размером 8 бит, поэтому выводим не с помощью %u
  printf("[2.1] Enter number in which to check bit #%hhu: ", bit);
  fflush(stdout);

  scanf("%lld", &number);

  // просто проверяем если нужный бит выставлен
  if ((uint64)number & (1ull << (bit - 1)))
    printf("[2.2] bit #%hhu is set\n", bit);
  else
    printf("[2.2] bit #%hhu is not set\n", bit);
  return 0;
}