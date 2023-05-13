// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 6

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

// print one line with index
void printTableHelper (int *row, int n, int colWidth, int index, int skip) {
	// print one empty column in case of header
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

// print table row divider
int printDivider(int n, int colWidth, int skip, char **saveRow) {
	char *row;
	// size of all elements and columns
	int rowSize = colWidth * (n + 1) + (n + 2);
	// if we don't have a row saved then create one and save it
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

	// if we need to skip a column then print
	// spaces in front of it
	char *startPoint = row;
	if (skip) {
		startPoint += (colWidth + 1);
	}
	printf("%*s\n", rowSize, startPoint);
	return 0;
}

// print 2-dimensional matrix with indices
int printTable(int **matrix, int rows, int cols) {
	int *indexes = malloc(sizeof(int) * cols);
	if (indexes == NULL) {
		perror("Can't allocate memory for table index");
		return 1;
	}
	for (int i = 0; i < cols; i++) {
		indexes[i] = i;
	}

	// find the max length of any number in the array
	int maxSize = log10(cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			maxSize = max(maxSize, log10(matrix[i][j]));
		}
	}

	// width of one column
	int colWidth = maxSize + 2;
	char *divider = NULL;

	// print the header, skipping one column, and save the divider for later use
	if(printDivider(cols, colWidth, 1, &divider)) {
		perror("Can't allocate memory for divider, quitting");
		return 1;
	}
	// print table column indices
	printTableHelper(indexes, cols, colWidth, 0, 1);
	printDivider(cols, colWidth, 0, &divider);

	for (int i = 0; i < rows; i++) {
		printTableHelper(matrix[i], cols, colWidth, i, 0);
		printDivider(cols, colWidth, 0, &divider);
	}

	free(divider);
	free(indexes);
}

void printArray(int *array, int size) {
	int *matrix[1] = {array};
	printTable(matrix, 1, size);
}

int main() {
	printf("[1.1] Filling the array with numbers...\n");
	int array[4];
	int *ptr = array;
	ptr[0] = 50;
	ptr[1] = 40;
	ptr[2] = 30;
	ptr[3] = 20;
	printf("[1.2] Array filled by pointer:\n");
	printArray(ptr, 4);

	printf("[2.1] Allocating memory for array:\n");
	int *dynamicArray = calloc(4, sizeof(int));
	if (dynamicArray == NULL) {
		perror("[^] Unable to allocate memory for dynamic array");
		return 1;
	}
	printf("[2.2] Memory allocated for array at %p\n", dynamicArray);

	*(dynamicArray + 0) = 50;
	*(dynamicArray + 1) = 40;
	*(dynamicArray + 2) = 30;
	*(dynamicArray + 3) = 20;

	printf("[2.3] Filled dynamic array:\n");
	printArray(dynamicArray, 4);

	free(dynamicArray);
	return 0;
}
