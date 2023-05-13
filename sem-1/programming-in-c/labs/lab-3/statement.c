// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 3

#include <stdio.h>

// получаем символ знака, нужно для считывания через scanf не в десятичной с.с.
int64_t getSign() {
  const char sign = fgetc(stdin);
  switch (sign) {
    case '+':
      return 1ll;
    case '-':
      return -1ll;
  }
  // если символ который мы считали на самом деле не знак, то откатимся
  ungetc(sign, stdin);
  return 1ll;
}

// вспомогательная функция, так как много вывода в данной лабе происходит по основанию 8
void outputInBase8(char *partID, int64_t number) {
  // т.к. число может быть отрицательным, то дополнительно покажем это
  if (number < 0) {
    printf("\n[%s] Entered number in base 8: -0o%llo", partID, -number);
    printf("\n[%s.1] Entered number as unsigned in base 8: 0o%llo\n", partID, number);
  } else {
    printf("\n[%s] Entered number in base 8: 0o%llo\n", partID, number);
  }
}

int main() {
  printf("[1] Enter integer in base 10: ");
  fflush(stdout);

  // считываем число + \n, так как дальше нам возможно нужно будет считать символ -
  int64_t number;
  scanf("%lld%*c", &number);

  outputInBase8("2", number);

  // битовые операции происходят только в беззнаковом типе
  // поэтому без разницы какой знак
  outputInBase8("3.1", number);
  printf("[3.2] Entered number shifted left by 1 bit in base 8: 0o%llo\n", (number<<1));

  outputInBase8("4.1", number);
  printf("[4.2] Entered number after bitwise negation in base 8: 0o%llo\n\n", (~number));

  printf("[5.1] Enter second number, in base 8: ");
  fflush(stdout);

  int64_t number2;
  // чтобы ввести число не в 10 с.с. нужно вручную проверить знак
  const int64_t sign = getSign();
  scanf("%llo", &number2);
  number2 *= sign;

  printf("[5.2] XOR of two entered numbers in base 8: 0o%llo\n", number^number2);
  return 0;
}