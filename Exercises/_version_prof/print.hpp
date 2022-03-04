///
/// Exemples des slides sur les classes/fonctions génériques (exemple 'afficherX()')
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <array>
#include <span>
#include <ranges>

using namespace std;


template <typename T>
void print1(ostream& out, T* values, int nValues) {
	out << "{";
	for (int i = 0; i < nValues; i++) {
		if (i != 0)
			out << ", ";
		out << values[i];
	}
	out << "}";
}

template <typename T, int size>
class Array {
public:
	//int getSize() const { return size; }
	int getSize() const;

	template <typename U>
	Array<U, size> toArray() const;
	//Array<U, size> toArray() const {
	//	Array<U, size> result;
	//	for (int i = 0; i < size; ++i)
	//		result[i] = U(values_[i]);
	//	return result;
	//}

	T& operator[] (int index) { return values_[index]; }
	const T& operator[] (int index) const { return values_[index]; }

private:
	T values_[size] = {};
};

template <typename T, int size>
int Array<T, size>::getSize() const {
	return size;
}

template <typename T, int size>
template <typename U>
Array<U, size> Array<T, size>::toArray() const {
	Array<U, size> result;
	for (int i = 0; i < size; ++i)
		result[i] = U(values_[i]);
	return result;
}

template <typename T>
void print2(ostream& out, span<const T> values) {
	out << "{";
	bool firstIter = true;
	for (auto&& v : values) {
		if (not firstIter)
			out << ", ";
		out << v;
		firstIter = false;
	}
	out << "}";
}

template <typename T, size_t size>
void print2(ostream& out, span<T, size> values) {
	print2(out, span<const T>(values));
}

template <ranges::input_range T>
void print3(ostream& out, const T& values) {
	out << "{";
	bool firstIter = true;
	for (auto&& v : values) {
		if (not firstIter)
			out << ", ";
		out << v;
		firstIter = false;
	}
	out << "}";
}

template <typename T>
using AsSpanConst = span<const ranges::range_value_t<T>>;

template < typename T>
concept ConvertibleToSpanConst = is_convertible_v<T, AsSpanConst<T>>;

template <ConvertibleToSpanConst T>
void print4(ostream& out, const T& values) {
	print2(out, AsSpanConst<T>(values));
}

template <typename T>
requires ranges::input_range<T> and not ConvertibleToSpanConst<T>
void print4(ostream& out, const T& values) {
	print3(out, values);
}
