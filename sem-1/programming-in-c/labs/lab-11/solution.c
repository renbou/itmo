// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 11
#include <pentagon.h>

int main() {
	printf("[1.1] Enter the coords of the pentagon in format \'x0 y0 x1 y1...\'\n: ");
	fflush(stdout);
	pentagon_p a = read_pentagon(stdin);
	printf("[1.2] Read pentagon:\n");
	print_pentagon(stdout, a);
	printf("\n");
	printf("[1.3] The perimeter of the pentagon = %Lf\n", pentagon_perimeter(a));
	printf("[1.4] The area of the pentagon = %Lf\n", pentagon_area(a));
}