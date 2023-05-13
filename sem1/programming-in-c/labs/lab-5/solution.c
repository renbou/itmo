// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 5

#include <stdio.h>
#include <stdlib.h>

#define max(a, b) b > a ? b : a

int log10(int a) {
  int k = 0;
  while (a) {
    a /= 10;
    k++;
  }
  return k;
}

// вывести 1 строку с индексом
void printTableHelper (int *row, int n, int colWidth, int index, int skip) {
  // выведем один пустой столбец, нужно в случае заголовка
  if (skip) {
    char tmp = '\0';
    printf("%*s", colWidth + 1, &tmp);
  } else {
    printf("| %*d ", colWidth - 2, index);
  }
  for (int i = 0; i < n; i++) {
    printf("| %*d ", colWidth - 2, row[i]);
  }
  printf("|\n");
}

// функция для вывода разделителя таблицы
int printDivider(int n, int colWidth, int skip, char **saveRow) {
  char *row;
  // для каждого элемента + индекс + разделители
  int rowSize = colWidth * (n + 1) + (n + 2);
  // если у нас не сохранена уже созданная строка,
  // тогда нужно ее создать и сохранить при такой возможности
  if (saveRow == NULL || *saveRow == NULL) {
    row = calloc(rowSize + 1, sizeof(char));
    if (row == NULL) {
      return 1;
    }
    if (saveRow != NULL) {
      *saveRow = row;
    }

    row[0] = '+';
    for (int i = 0; i < n + 1; i++) {
      for (int j = 0; j < colWidth; j++) {
        row[1 + i * (colWidth + 1) + j] = '-';
      }
      row[1 + i * (colWidth + 1) + colWidth] = '+';
    }
  } else {
    row = *saveRow;
  }

  // если нам нужно сделать отступ, как например для индексов по столбцам
  // то начнем печатать со второго столбца, и сделаем отступ в 1 столбец
  // для этого выставим размер строки в максимум, если надо будет то оно
  // дополнится пробелами
  char *startPoint = row;
  if (skip) {
    startPoint += (colWidth + 1);
  }
  printf("%*s\n", rowSize, startPoint);
  return 0;
}

// вывести 2-мерный массив как таблицу значений
int printTable(int **matrix, int rows, int cols) {
  int *indexes = malloc(sizeof(int) * cols);
  if (indexes == NULL) {
    perror("Can't allocate memory for table index");
    return 1;
  }
  for (int i = 0; i < cols; i++) {
    indexes[i] = i;
  }

  // найдем максимальную длину числа в массиве
  // изначально выставим как длину макс. индекса столбца
  int maxSize = log10(cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      maxSize = max(maxSize, log10(matrix[i][j]));
    }
  }

  // ширина одной ячейки
  int colWidth = maxSize + 2;
  char *divider = NULL;

  // вывести заголовок, пропускаем 1 столбец т.к. выводим только индексы столбцов
  // сохраним разделитель в divider чтобы в дальнейшем его просто выводить
  if(printDivider(cols, colWidth, 1, &divider)) {
    perror("Can't allocate memory for divider, quitting");
    return 1;
  }
  // выведем индексы столбцов
  printTableHelper(indexes, cols, colWidth, 0, 1);
  printDivider(cols, colWidth, 0, &divider);

  for (int i = 0; i < rows; i++) {
    printTableHelper(matrix[i], cols, colWidth, i, 0);
    printDivider(cols, colWidth, 0, &divider);
  }
}

void printArray(int array[], int size) {
  int *matrix[1] = {array};
  printTable(matrix, 1, size);
}

#define printMatrix printTable

// функция которая перемножет матрицы nxm и mxk
// и сохраняет в result матрицу nxk
void multiplyMatrices(int **matrix1, int **matrix2, int **result, int n, int m, int k) {
  for (int i = 0; i < n; i++) {
    for (int l = 0; l < k; l++) {
      result[i][l] = 0;
      for (int j = 0; j < m; j++) {
        result[i][l] += matrix1[i][j] * matrix2[j][l];
      }
    }
  }
}

int main() {
  printf("[1.1] Initializing array with numbers from task...\n");

  int numbers[7] = {90, 76, 54, 23, 56, 12, 48};

  printf("[1.2] Entered numbers:\n");
  printArray(numbers, 7);

  // проинициализируем матрицы как динамические массивы
  // так как printMatrix и multMatrix принимают матрицы именно в таком виде
  // и еще одну матрицу для результата перемножения
  int matrix1[2][2] = {{1, 2}, {3, 4}},
      matrix2[2][2] = {{1, 0}, {0, 1}},
      matrix3[2][2] = {{0, 0}, {0, 0}};
  int *matrix1_[2] = {matrix1[0], matrix1[1]},
      *matrix2_[2] = {matrix2[0], matrix2[1]},
      *matrix3_[2] = {matrix3[0], matrix3[1]};

  printf("[2.1] Matrices from task:\n");
  printf("[2.1.1] Matrix 1:\n");
  printMatrix(matrix1_, 2, 2);
  printf("[2.1.2] Matrix 2:\n");
  printMatrix(matrix2_, 2, 2);

  multiplyMatrices(matrix1_, matrix2_, matrix3_, 2, 2, 2);
  printf("[2.2] Result of matrix multiplication:\n");
  printMatrix(matrix3_, 2, 2);

  return 0;
}