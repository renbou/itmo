// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 9

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define max(a, b) \
  (a > b ? a : b)

typedef uint64_t uint64;
typedef uint32_t uint32;

// trial prime testing
int is_prime(uint32 num) {
	uint64 until = round(sqrt(num));
	for (uint64 i = 2; i <= until; i++) {
		if (num % i == 0)
			return 0;
	}
	return 1;
}

int main() {
	uint32 minutes;
	long double subscription_fee, extra_minute_cost;
	printf(
		"[1.1] Enter the total length of all calls in minutes\n"
  		": ");
	fflush(stdout);
	scanf("%u", &minutes);

	printf(
		"[1.2] Enter the default subscription fee\n"
  		": ");
	fflush(stdout);
	scanf("%Lf", &subscription_fee);

	printf(
		"[1.3] Enter the cost of extra minutes\n"
		": ");
	fflush(stdout);
	scanf("%Lf", &extra_minute_cost);

	// check if we have valid data entered

	long double default_minute_cost = subscription_fee / 499.0L;
	if (default_minute_cost > extra_minute_cost || fabs(default_minute_cost - extra_minute_cost) < 1e-9) {
		printf("[1.4] Invalid costs entered! Default minute cost is >= than the cost of extra minutes!\n");
		exit(1);
	}

	// max(minutes, 499)-499 is because if minutes < 499, then we get a huge number when we subtract 499
	long double result_cost = subscription_fee + max(max(minutes, 499)-499, 0) * extra_minute_cost;
	printf("[1.4] Total cost is %Lf\n", result_cost);

	printf(
		"[2.1] Enter n >= 2 until which to print prime numbers\n"
  		": ");
	fflush(stdout);
	uint32 n;
	scanf("%u", &n);

	printf("[2.2] Prime numbers from 2 to n:\n\t");
	fflush(stdout);
	for (uint32 i = 2; i <= n; i++) {
		if (is_prime(i)) {
			printf("%u ", i);
		}
		if (i % 100 == 0) {
			printf("\n\t");
		}
	}
	printf("\n");
	return 0;
}