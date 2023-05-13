#include "pi.h"

#include "random_gen.h"

double calculate_pi(unsigned long runs) {
	unsigned long successful = 0;
	for (unsigned long i = 0; i < runs; i++) {
		double x = get_random_number();
		double y = get_random_number();
		if (x*x + y*y <= 1.0l) {
			successful++;
		}
	}

	return runs != 0 ? (static_cast<double>(successful) / runs) * 4 : 0;
}

