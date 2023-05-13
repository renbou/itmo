#include <bits/stdc++.h>

#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)

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

template<typename T>
function<void(T &a, T b)>
	setmax = [](T &a, T b) -> void {
		if (unlikely (b > a)) {
			a = b;
		} else {
			b = a;
		}
	};

template<typename T>
function<void(T &a, T b)>
	setmin = [](T &a, T b) -> void {
	if (unlikely (b < a)) {
		a = b;
	} else {
		b = a;
	}
};

int main() {
	FILE *in = fopen("turtle.in", "r ");
	FILE *out = fopen("turtle.out", "w");

	uint16 height, width;
	fscanf(in, "%hu %hu", &height, &width);

	vector< vector<uint16> > map( height,
							      vector<uint16>(width, 0) );
	for (auto &row: map) {
		for (uint16 &elem: row)
			fscanf(in, "%hu", &elem);
	}
	reverse(map.begin(), map.end());

	vector< vector<uint32> > sums( height,
								   vector<uint32>(width, 0) );
	sums[0][0] = map[0][0];

	for (uint16 row = 0; row < height; row++) {
		for (uint16 col = 0; col < width; col++) {
			uint32 	&currentSum = sums[row][col],
					&&currentElem = map[row][col];
			if (col > 0) setmax<uint32>(currentSum, sums[row][col - 1] + currentElem);
			if (row > 0) setmax<uint32>(currentSum, sums[row - 1][col] + currentElem);
		}
	}

	fprintf(out, "%u\n", sums.back().back());
	return 0;
}