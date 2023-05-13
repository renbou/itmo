#include <bits/stdc++.h>

using namespace std;

typedef uint64_t uint64;
typedef int64_t int64;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint8_t ubyte;
typedef int8_t byte;
typedef unsigned char uchar;

int main() {
	FILE *in = fopen("aplusbb.in", "r");
	FILE *out = fopen("aplusbb.out", "w");
	int64 a, b;
	fscanf(in, "%lld %lld", &a, &b);
	fprintf(out, "%lld\n", (a+b*b));
	return 0;
}