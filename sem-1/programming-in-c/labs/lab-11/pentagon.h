#ifndef PENTAGON_H
#define PENTAGON_H

#include <stdio.h>

// Structure defining a pentagon
typedef struct pentagon_t {
	long double x[5];
	long double y[5];
} pentagon;
// Pointer to a pentagon structure
typedef pentagon * pentagon_p;

/* Function for reading a pentagon from the current location in the file
 *
 * Format is "x1 y1 x2 y2 x3 y3 x4 y4 x5 y5"
 * If it returns NULL, then something has gone wrong
 * Doesn't check that the read data is an actual pentagon
 * @param f the file from which the pentagon is read
 * @return if reading has failed, returns NULL, otherwise a healthy pentagon
 */
pentagon_p read_pentagon(FILE *f);

/* Prints the pentagon to the specified file
 *
 * if format [(x1, y1), (x2, y2), (x3, y3), (x4, y4), (x5, y5)]
 * @param f the file to print to
 * @param p the pentagon to print
 */
void print_pentagon(FILE *f, pentagon_p p);

/* Function which finds the perimiter of a pentagon
 *
 * This function does not care if the structure is a valid pentagon,
 * so if an incorrect pentagon is passed, the return value is undefined
 * (actually equal to the cumulative sum of the lengths of segments specified by points)
 */
long double pentagon_perimeter(pentagon_p p);

/* Function which finds the area of a pentagon
 *
 * Again doesn't care about the correctness,
 * the result of finding the area of an incorrect pentagon is undefined
 * (actually defined, but some garbage)
 * Finds the area using the geometric meaning of vector cross multiplication
 */
long double pentagon_area(pentagon_p);

#endif //PENTAGON_H
