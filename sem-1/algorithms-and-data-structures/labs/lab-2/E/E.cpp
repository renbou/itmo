#include <bits/stdc++.h>
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,sse4,popcnt,abm,mmx,avx,tune=native")

using std::swap;
using std::cout;
using std::cin;

int partition(std::vector<int> &array, int left, int right, int k) {
	int pivot = array[right], firstPivot = left;
	// place elems less than pivot on the left side
	for (int j = left; j < right; j++) {
		if (array[j] < pivot) {
			swap(array[j], array[firstPivot]);
			firstPivot++;
		}
	}
	int secondPivot = firstPivot;
	// elems equal to pivot in the center
	for (int j = secondPivot; j < right; j++) {
		if (array[j] == pivot) {
			swap(array[j], array[secondPivot]);
			secondPivot++;
		}
	}
	swap(array[secondPivot], array[right]);
	// return the left if the desired element is before equal elems
	if (k < firstPivot)
		return firstPivot;
	// if the desired element is in the equal ones, then we found the answer
	else if (k <= secondPivot)
		return k;
	// otherwise the desired element is in the right, gotta continue
	return secondPivot;
}

int quickSort(std::vector<int> &array, int n, int k) {
	int left = 0, right = n - 1, elem, size, pivot, j, i, mid;
	while(true) {
		if (right == left) {
			return array[left];
		}
		// pick median out of 5 elements
		size = right - left;
		elem = rand() % size;
		swap(array[left + elem], array[right]);

		mid = partition(array, left, right, k);

		if (mid == k) {
			return array[mid];
		} else if (k < mid) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);

	freopen("kth.in", "r", stdin);
	freopen("kth.out", "w", stdout);

	int n, k, A, B, C;
	cin >> n >> k >> A >> B >> C;
	std::vector<int> arr(n);
	cin >> arr[0] >> arr[1];
	k--;
	for (int i = 2; i < n; i++) {
		arr[i] = arr[i-2]*A + arr[i-1]*B + C;
	}
	cout << quickSort(arr, n, k) << std::endl;
	return 0;
}
