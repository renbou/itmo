// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 7

#include <stdio.h>
#include <stdint.h>
#include <math.h>

enum bakery {CAKE, MUFFIN, CANDY, ICECREAM};

typedef long double real;

typedef struct {
	real x1, y1;
	real x2, y2;
} segment;

// length of segment is sqrt((x2-x1)^2 + (y2-y1)^2)
real segment_length(segment *s) {
	real first = (s->x2 - s->x1), second = (s->y2 - s->y1);
	return sqrt(first*first + second*second);
}

typedef union {
	struct {
		unsigned int
			dsl_on:1,
			ppp_on:1,
			link_on:1,
			filler: 29;
	};
	uint32_t flags;
} adsl_status;

adsl_status adsl_modem_make_status(unsigned int dsl_on, unsigned int ppp_on, unsigned int link_on) {
	adsl_status st = {{dsl_on, ppp_on, link_on, 0}};
	return st;
}

int main() {
	printf("[1.1] Enum bakery \"CANDY\" value = %u\n", CANDY);

	segment a = {1.0L, 2.5L, 10.0L, 5.0L};
	printf(
		"[2.1] Segment a coords:\n"
  		"\tx1 = %Lf\n"
		"\ty1 = %Lf\n"
  		"\tx2 = %Lf\n"
		"\ty2 = %Lf\n", a.x1, a.y1, a.x2, a.y2);
	printf("[2.2] Segment a length = %Lf\n", segment_length(&a));

	printf(
		"[3.1] ADSL modem flags:\n"
  		"\tDSL off, PPP off, Link off: %u\n"
		"\tDSL off, PPP off, Link on: %u\n"
		"\tDSL off, PPP on, Link off: %u\n"
		"\tDSL off, PPP on, Link on: %u\n"
		"\tDSL on, PPP off, Link off: %u\n"
		"\tDSL on, PPP off, Link on: %u\n"
		"\tDSL on, PPP on, Link off: %u\n"
		"\tDSL on, PPP on, Link on: %u\n",
		(uint32_t)(adsl_modem_make_status(0, 0, 0).flags),
		(uint32_t)(adsl_modem_make_status(0, 0, 1).flags),
		(uint32_t)(adsl_modem_make_status(0, 1, 0).flags),
		(uint32_t)(adsl_modem_make_status(0, 1, 1).flags),
		(uint32_t)(adsl_modem_make_status(1, 0, 0).flags),
		(uint32_t)(adsl_modem_make_status(1, 0, 1).flags),
		(uint32_t)(adsl_modem_make_status(1, 1, 0).flags),
		(uint32_t)(adsl_modem_make_status(1, 1, 1).flags));

	uint32_t value;
	printf("[3.2] Enter value (in hex) to set in bit field\n\t: ");
	fflush(stdout);
	scanf("%x", &value);

	adsl_status status;
	status.flags = value;
	printf(
		"[3.3] Corresponding values:\n"
  		"DSL %s\n"
		"PPP %s\n"
  		"Link %s\n",
  		status.dsl_on ? "on" : "off",
  		status.ppp_on ? "on" : "off",
  		status.link_on ? "on" : "off");

	return 0;
}
