///
/// Ce projet utilise quelques librairies disponibles sur Vcpkg et permet entre autre d'en tester le bon fonctionnement.
///


#include <cstddef>
#include <cstdint>

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <span>
#include <utility>

#include <cppitertools/range.hpp>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/zip.hpp>

#include "Fraction.hpp"
#include "Vector.hpp"
#include "print.hpp"
#include "lamda.hpp"

using namespace std;
using namespace iter;


void runArrayExample() {
	Array<double, 3> foo; // meme chose que std::array<double, 3> foo
	foo[1] = 2.2; foo[2] = 4.4;
	const auto& cfoo = foo;
	print1(cout, &cfoo[0], foo.getSize()); // on fait pas de cpp pour template (template is defined in hpp)
	// template est "called" en mettant print1<type>(...)
	cout << "\n";
	Array bar = foo.toArray<int>(); // = convertit en tableau de int
	// on lui dit "TON TYPENAME U EST int"
	// meme chose que auto bar = ...
	print1(cout, &bar[0], bar.getSize());
	cout << "\n";
}


// partie CONTRAINTES
void runPrintExample() {
	int a[] = {4, 1, 3}; int b[] = {5, 6}; string c[] = {"b", "a"};
	print1(cout, a, 3); print1(cout, b, 2); print1(cout, c, 2); // type de print1 ( ex: print1<int> ) est IMPLICITE
	// on doit preciser type dans qq cas
	
	cout << "\n";

	array<int, 2> arr = {3, 2};
	vector<int> vec = {3, 2};
	deque<int> deq = {1, 2, 3};

	// PAS AFFICHABLE
	struct Spam { int m };
	vector<Spam> spam = { {1}, {2}, {3}};
	print3(cout, spam); // elements pas affichable, car aucun rhs avec type spam
	// erreur plus facile a lire


	print2(cout, span<const int>(arr)); cout << "\n";
	print2(cout, span<const int> (vec)); cout << "\n";
	//print2(cout, span(arr)); cout << "\n";
	//print2(cout, span(vec)); cout << "\n";
	print3(cout, arr); cout << "\n";
	print3(cout, vec); cout << "\n";
	print3(cout, deq); cout << "\n";
	//print3(cout, 42); cout << "\n"; // Erreur dans le for (pas capable d'itérer sur 42).
}

void runVectorExample() {
	array<int, 3> values1 = {1, 2, 3};
	array<int, 3> values2 = {10, 20, 30};
	Vector foo(values1);
	Vector bar(values2);
	foo += bar + bar;
	foo -= bar;
	cout << foo << "\n";
}

// CHAPITRE 10: LAMBDA
void runLambdaExample() {
	vector<int> foo = {1, 2, 3, 4, 5};
	vector<int> bar = {10, 20, 30, 40, 50};
	auto next1 = getNextFn(foo);
	auto next2 = getNextFn(bar);
	for (auto i : range(foo.size()))
		cout << next1() << " ";
	cout << "\n";
	for (auto i : range(bar.size()))
		cout << next2() << " ";
	cout << "\n";



	// implementation de ligne 23 de lambda.hpp
	auto f = generateLine<double>(5, 10);
	f(10); // 10 *5 +10 = 60
	// we define what x is in:
	// 
	// return [=](T x) mutable { return slope * x + offset; }

	cout << f(10);


	auto line = generateLine(10, 1);
	for (auto&& x : {-1, 0, 1, 5, 10})
		cout << line(x) << " ";
	cout << "\n";
}


int main() {
	runArrayExample();
	//cout << "\n\n\n";
	//runPrintExample();
	//cout << "\n\n\n";
	//runVectorExample();
	//cout << "\n\n\n";
	//runLambdaExample();
}

