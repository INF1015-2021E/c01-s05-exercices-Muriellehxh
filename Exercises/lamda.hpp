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
// exemple ou fonction RETOURNE une lambda
function<T(T)> generateLine(T slope, T offset) {
// TYPE DE RETOUR: function<RETURN_TYPE ( PARAMETER_TYPES ) >
// in this case, both are the same type, cause both are T types

	return [=](T x) mutable { return slope * x + offset; };
	// mutable permet de modifier slope a chaque differents appels de lambda
} 

// [=] : tous les objets definis (T slope, T offset), vont etre contenu dans lambda


//a chaque fois quon appelle fonction, on retourne prochain element du tableau  
function<int()> getNextFn(const vector<int>& v) {
	int i = 0;
	return [i, &v]() mutable { return v[i++]; }; // on change i a chaque appel

	// capture de i, &v pour que foo (main.cpp) puisse l'utiliser dans next1
}

