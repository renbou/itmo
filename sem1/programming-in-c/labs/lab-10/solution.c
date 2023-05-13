// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 10

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define max(a, b) \
  (a > b ? a : b)

typedef uint64_t uint64;
typedef uint32_t uint32;

// accumulator for tail-recursion optimization
// with Ofast gets optimized to an inline loop instead of a function call
uint32 _sum_digits(uint32 a, uint32 accum) {
	if (a == 0)
		return accum;
	return _sum_digits(a / 10, accum + a % 10);
}

uint32 sum_digits(uint32 a) {
	return _sum_digits(a, 0u);
}

typedef struct point_t {
	double x;
	double y;
} point;

double** calc_distances(point* points, uint32 n) {
	// allocate enough memory for all the distances
	double **result = calloc(n, sizeof(double *));
	if (result == NULL)
		return NULL;
	for (uint32 i = 0; i < n; i++) {
		result[i] = calloc(n-1, sizeof(double));
		if (result[i] == NULL) {
			for (uint32 j = 0; j < i; j++)
				free(result[i]);
			free(result);
			return NULL;
		}
	}

	// calculate distance from each point to other points
	// when we get to j=i we will fill i'th with 0, but on the next iteration
	// we will fill it with the correct distance to the next point
	for (uint32 i = 0; i < n; i++) {
		double *current = result[i];
		for (uint32 j = 0; j < n; j++) {
			double xd = points[i].x - points[j].x;
			double xy = points[i].y - points[j].y;
			double dist = sqrt(xd*xd + xy*xy);
			current[j > i ? j - 1 : j] = dist;
		}
	}
	return result;
}

int main() {
	printf(
		"[1.1] Enter the number of points\n"
  		": ");
	fflush(stdout);
	uint32 n;
	scanf("%u", &n);
	point *points = calloc(n, sizeof(point));
	if (points == NULL) {
		printf("ERROR can't allocate memory for %u points", n);
		return 1;
	}
	for (uint32 i = 0; i < n; i++) {
		printf("[1.2] Point %u coords (x y): ", i);
		fflush(stdout);
		scanf("%lf %lf", &points[i].x, &points[i].y);
	}

	double **distances = calc_distances(points, n);
	printf("[1.3] Distances:\n");
	for (uint32 i = 0; i < n; i++) {
		printf("From point %u: ", i);
		double *current = distances[i];
		for (uint32 j = 0; j < n; j++) {
			if (j == i)
				continue;
			printf("%lf ", current[j > i ? j - 1 : j]);
		}
		free(current);
		printf("\n");
	}
	free(distances);

	printf(
		"[2.1] Enter the number which digits you want to be summed:\n"
  		": ");
	fflush(stdout);
	scanf("%u", &n);
	printf("[2.2] Sum of the digits of %u is equal to %u", n, sum_digits(n));
	return 0;
}