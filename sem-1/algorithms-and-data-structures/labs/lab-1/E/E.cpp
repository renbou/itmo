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
	FILE *in = fopen("sortland.in", "r ");
	FILE *out = fopen("sortland.out", "w");

	uint16 n;
	fscanf(in, "%hu", &n);

	vector< pair<uint32, uint16> > numbers(n);

	{
		double temp;
		for (uint32 i = 0; i < n; i++) {
			fscanf(in, "%lf", &temp);
			numbers[i] = {static_cast<uint32>(round(temp * 100)), i + 1};
		}
	}

	for (uint32 i = 0; i < n; i++) {
		for (uint32 j = i + 1; j < n; j++) {
			if (unlikely (numbers[j].first < numbers[i].first))
				swap(numbers[i], numbers[j]);
		}
	}

	fprintf(out, "%u %u %u\n", numbers[0].second, numbers[n / 2].second, numbers[n - 1].second);
	return 0;
}