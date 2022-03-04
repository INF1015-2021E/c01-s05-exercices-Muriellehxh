///
/// 
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <functional>
#include <vector>
#include <deque>
#include <span>

#include <cppitertools/itertools.hpp>

using namespace std;
using namespace iter;


template <typename T>
pair<function<T(T)>, function<T(T)>> generateCurves(T a, T b, T c) {
	return {
		[=] (T x) { return a * x + b; },
		[=] (T x) { return a * x*x + b * x + c; }
	};
}

function<int()> getNextFn(const vector<int>& v) {
	int i = 0;
	return [i, &v]() mutable { return v[i++]; };
}

