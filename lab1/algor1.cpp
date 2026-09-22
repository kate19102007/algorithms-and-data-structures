#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;


void selectionSort(vector<int>& a, long long& cmp, long long& swaps)
{
	int n = a.size();
	for (int i = 0; i < n - 1; i++)
	{
		int minIdx = i;
		for (int j = i + 1; j < n; j++)
		{
			cmp++;
			if (a[j] < a[minIdx])
				minIdx = j;
		}
		if (minIdx != i)
		{
			swap(a[i], a[minIdx]);
			swaps++;
		}
	}
}


const int MIN_RUN = 32;


int calcMinRun(int n) {
	int r = 0;
	while (n >= MIN_RUN) {
		r |= (n & 1);
		n >>= 1;
	}
	return n + r;
}

void insertionSort(vector<int>& arr, int left, int right, long long& cmp, long long& swaps) {
	for (int i = left + 1; i <= right; i++) {
		int key = arr[i];
		int j = i - 1;
		while (j >= left) {
			cmp++;
			if (arr[j] > key) {
				arr[j + 1] = arr[j];
				swaps++;
				j--;
			}
			else {
				break;
			}
		}
		arr[j + 1] = key;
	}
}

void merge(vector<int>& arr, int l, int m, int r, long long& cmp, long long& swaps) {
	vector<int> left(arr.begin() + l, arr.begin() + m + 1);
	vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

	int i = 0, j = 0, k = l;
	while (i < left.size() && j < right.size()) {
		cmp++;
		if (left[i] <= right[j]) {
			arr[k++] = left[i++];
		}
		else {
			arr[k++] = right[j++];
		}
		swaps++;
	}
	while (i < left.size()) {
		arr[k++] = left[i++];
		swaps++;
	}
	while (j < right.size()) {
		arr[k++] = right[j++];
		swaps++;
	}
}


int findRun(vector<int>& arr, int start, int n, long long& cmp, long long& swaps) {
	int end = start + 1;
	if (end == n) return end;

	cmp++;
	if (arr[end] < arr[start]) {
		
		while (end < n) {
			cmp++;
			if (arr[end] < arr[end - 1]) end++;
			else break;
		}
		reverse(arr.begin() + start, arr.begin() + end);
		swaps += (end - start) / 2; 
	}
	else {
		
		while (end < n) {
			cmp++;
			if (arr[end] >= arr[end - 1]) end++;
			else break;
		}
	}
	return end;
}


void timSort(vector<int>& arr, long long& cmp, long long& swaps) {
	int n = arr.size();
	int minRun = calcMinRun(n);
	vector<pair<int, int>> runs;

	int i = 0;
	while (i < n) {
		int runEnd = findRun(arr, i, n, cmp, swaps);
		int runLen = runEnd - i;

		
		if (runLen < minRun) {
			int end = min(i + minRun, n);
			insertionSort(arr, i, end - 1, cmp, swaps);
			runEnd = end;
		}
		runs.push_back({ i, runEnd });
		i = runEnd;

		
		while (runs.size() > 1) {
			int l1 = runs[runs.size() - 2].first;
			int r1 = runs[runs.size() - 2].second;
			int l2 = runs[runs.size() - 1].first;
			int r2 = runs[runs.size() - 1].second;

			int len1 = r1 - l1;
			int len2 = r2 - l2;

			if (len1 <= len2) {
				merge(arr, l1, r1 - 1, r2 - 1, cmp, swaps);
				runs.pop_back();
				runs[runs.size() - 1] = { l1, r2 };
			}
			else {
				break;
			}
		}
	}

	
	while (runs.size() > 1) {
		int l1 = runs[runs.size() - 2].first;
		int r1 = runs[runs.size() - 2].second;
		int l2 = runs[runs.size() - 1].first;
		int r2 = runs[runs.size() - 1].second;

		merge(arr, l1, r1 - 1, r2 - 1, cmp, swaps);
		runs.pop_back();
		runs[runs.size() - 1] = { l1, r2 };
	}
}

int partition(vector<int>& a, int low, int high,
	long long& cmp, long long& swaps)
{
	int pivot = a[high];
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
		cmp++;
		if (a[j] < pivot)
		{
			i++;
			swap(a[i], a[j]);
			swaps++;
		}
	}
	swap(a[i + 1], a[high]);
	swaps++;
	return i + 1;
}


void quickSort(vector<int>& a, int low, int high,
	long long& cmp, long long& swaps)
{
	while (low < high)
	{
		int pi = partition(a, low, high, cmp, swaps);

		if (pi - low < high - pi)
		{
			quickSort(a, low, pi - 1, cmp, swaps);
			low = pi + 1;
		}
		else
		{
			quickSort(a, pi + 1, high, cmp, swaps);
			high = pi - 1;
		}
	}
}



vector<int> makeArray(int n, int type)
{
	vector<int> a(n);
	if (type == 0)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dist(1, 100000); 
		for (int i = 0; i < n; i++)
			a[i] = dist(gen);
	}
	if (type == 1)
	{
		for (int i = 0; i < n; i++)
			a[i] = i;
	}
	if (type == 2)
	{
		for (int i = 0; i < n; i++)
			a[i] = n - i;
	}
	return a;
}

int main()
{
	setlocale(LC_ALL, "Ukrainian");
	int sizes[] = { 100, 1000, 10000 };
	for (int n : sizes)
	{
		for (int type = 0; type < 3; type++)
		{
			vector<int> original = makeArray(n, type);
			cout << "\n=========================================\n";
			if (type == 0)
				cout << "Random array, N =  " << n << endl;
			if (type == 1)
				cout << "Sorted array, N = " << n << endl;
			if (type == 2)
				cout << "Reverse array, N = " << n << endl;
			cout << "=========================================\n";

			// Вибором
			{
				vector<int> a = original;
				long long cmp = 0, swaps = 0;
				auto start = chrono::high_resolution_clock::now();
				selectionSort(a, cmp, swaps);
				auto finish = chrono::high_resolution_clock::now();
				double time = chrono::duration<double, milli>(finish - start).count();
				cout << "Selection sort : "
					<< time << " ms,  "
					<< cmp << " comparisons, "
					<< swaps << " permutations\n";
			}
			// Timsort 
			{
				vector<int> a = original;
				long long cmp = 0, swaps = 0;
				auto start = chrono::high_resolution_clock::now();
				timSort(a, cmp, swaps);
				auto finish = chrono::high_resolution_clock::now();
				double time = chrono::duration<double, milli>(finish - start).count();
				cout << "Timsort : "
					<< time << " ms, "
					<< cmp << " comparisons, "
					<< swaps << " permutations\n";
			}
			// Швидке
			{
				vector<int> a = original;
				long long cmp = 0, swaps = 0;
				auto start = chrono::high_resolution_clock::now();
				quickSort(a, 0, n - 1, cmp, swaps);
				auto finish = chrono::high_resolution_clock::now();
				double time = chrono::duration<double, milli>(finish - start).count();
				cout << "Quick sort : "
					<< time << " ms, "
					<< cmp << " comparisons, "
					<< swaps << " permutations\n";
			}
		}
	}
	return 0;
}
