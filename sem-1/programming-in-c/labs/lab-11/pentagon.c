#include <pentagon.h>
#include <stdlib.h>
#include <math.h>

pentagon_p read_pentagon(FILE *f) {
	pentagon_p result = calloc(1, sizeof(pentagon));
	if (result == NULL)
		return NULL;
	for (size_t i = 0; i < 5; i++) {
		int ret = fscanf(f, "%Lf %Lf", &result->x[i], &result->y[i]);
		if (ret != 2) {
			free(result);
			return NULL;
		}
	}
	return result;
}

void print_pentagon(FILE * f, pentagon_p p) {
	if (p == NULL || f == NULL)
		return;
	fputc('[', f);
	for (int i = 0; i < 5; i++) {
		fprintf(f, "(%Lf, %Lf)", p->x[i], p->y[i]);
		if (i < 4)
			printf(", ");
	}
	fputc(']', f);
};

long double pentagon_perimeter(pentagon_p p) {
	if (p == NULL)
		return 0;

	// initialize with the length of segment from last to first point
	long double xd, yd;
	xd = p->x[4] - p->x[0];
	yd = p->y[4] - p->y[0];
	long double result = sqrtl(xd * xd + yd * yd);
	for (size_t i = 1; i < 5; i++) {
		xd = p->x[i - 1] - p->x[i];
		yd = p->y[i - 1] - p->y[i];
		result += sqrtl(xd * xd + yd * yd);
	}

	return result;
}

long double pentagon_area(pentagon_p p) {
	if (p == NULL)
		return 0;

	long double ax = p->x[0], ay = p->y[0];
	long double result = 0;
	for (size_t i = 0; i < 3; i++) {
		// current points, for easier interpretation
		// a, b, c are the points of the current triangle, we iterate 3 times,
		// which corresponds to the pentagon being divided into 3 triangles
		long double bx = p->x[i + 1], by = p->y[i + 1];
		long double cx = p->x[i + 2], cy = p->y[i + 2];

		// coordinates of vectors ab and ac
		long double abx = bx - ax, aby = by - ay;
		long double acx = cx - ax, acy = cy - ay;

		// the magnitude of the cross product is equal to the area of a parallelogram, so we divide it by 2
		long double cur_triangle_area = (abx * acy - aby * acx) / 2.0L;
		result += cur_triangle_area;
	}

	return result;
}