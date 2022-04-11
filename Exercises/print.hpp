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
		out << values[i]; // par ducktypping, on se fout de type T,
		// MAIS, faut que classe ait fonction affichage 
	}
	out << "}";
}


template <typename T, int size> // size est constante dans classe 
class Array {
public:
	int getSize() const { return size; }

	template <typename U> // methode generique a l'interieur de classe generique
	// on fait conversion de type de tableau typename T en typename U

	
	Array<U, size> toArray() const {
		Array<U, size> result;
		for (int i = 0; i < size; ++i)
			result[i] = U(values_[i]); // result[i] = a la conversion en U
		return result;
	}

	T& operator[] (int index) { return values_[index]; }
	const T& operator[] (int index) const { return values_[index]; }

private:
	T values_[size] = {};
};


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

	// on peut surcharger meme template
}


// template tres generique
template <range::input_range T> // permet d'avoir contraintes sur type 
// ACCELERE PROCESSUS DE DEBOGAGE
template <typename T>
void print3(ostream& out, const T& values) {
	out << "{";
	bool firstIter = true;
	for (auto&& v : values) { // ducktypping = condition de template 
	// on peut avoir un type de template T qui permet d'iterer sur values
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
	print3(out, AsSpanConst<T>(values));
}

template <typename T>
requires ranges::input_range<T> and not ConvertibleToSpanConst<T>
void print4(ostream& out, const T& values) {
	print3(out, values);
}
