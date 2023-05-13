// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17

#include <stdio.h>

void handleReal() {
  double number;
  printf("Enter the number: ");
  fflush(stdout);
  scanf("%lf", &number);
  printf("Entered real number: %lf\n", number);
}

void handleChar() {
  char character;
  printf("Enter the character: ");
  fflush(stdout);
  scanf("%c", &character);
  printf("Entered character: %c\n", character);
}

void handleInvalid() {
  printf("Invalid choice entered.\n");
}

int main() {
  printf(
    "What do you want to enter?\n"
    "  r - real number\n"
    "  c - character\n"
    ": ");
  // нужно очистить буффер файла stdout, чтобы всё точно вывелось на экран
  fflush(stdout);

  char choice;
  // считываем сам выбор + символ \n, который записываем никуда
  scanf("%c%*c", &choice);

  // в зависимости от выбора вызываем ту или иную функцию для обработки данных
  switch (choice) {
    case 'r': {
      handleReal();
      break;
    }
    case 'c': {
      handleChar();
      break;
    }
    default: {
      handleInvalid();
      break;
    }
  }
	return 0;
}