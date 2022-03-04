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
function<T(T)> generateLine(T slope, T offset) {
	return [=](T x) { return slope * x + offset; };
}

function<int()> getNextFn(const vector<int>& v) {
	int i = 0;
	return [i, &v]() mutable { return v[i++]; };
}

