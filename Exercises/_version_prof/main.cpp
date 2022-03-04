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
	Array<double, 3> foo;
	foo[1] = 2.2; foo[2] = 4.4;
	const auto& cfoo = foo;
	print1(cout, &cfoo[0], foo.getSize());
	cout << "\n";
	Array bar = foo.toArray<int>();
	print1(cout, &bar[0], bar.getSize());
	cout << "\n";
}

void runPrintExample() {
	int a[] = {4, 1, 3};
	int b[] = {5, 6};
	string c[] = {"b", "a"};
	using F = Fraction;
	Fraction d[] = {F(1, 2), F(2, 3), F(5, 2)};
	//vector<int> e[] = {{}, {}};
	print1(cout, a, 3); print1(cout, b, 2); print1(cout, c, 2); print1(cout, d, 2);
	cout << "\n";

	array<int, 2> arr = {3, 2};
	vector<int> vec = {3, 2};
	deque<int> deq = {1, 2, 3};
	print2(cout, span<const int>(arr)); cout << "\n";
	print2(cout, span<int>(vec)); cout << "\n";
	//print2(cout, span(arr)); cout << "\n";
	//print2(cout, span(vec)); cout << "\n";
	print3(cout, arr); cout << "\n";
	print3(cout, vec); cout << "\n";
	print3(cout, deq); cout << "\n";
	//print3(cout, 42); cout << "\n"; // Erreur dans le for (pas capable d'itérer sur 42).
	print4(cout, vec); cout << "\n";
	//print4(cout, deq); cout << "\n";
}

void runVectorExample() {
	{
		array<int, 3> values1 = {1, 2, 3};
		array<int, 3> values2 = {10, 20, 30};
		Vector<int> foo = span<const int>(values1);
		Vector<int> bar = span<const int>(values2);
		foo += bar + bar;
		foo -= bar;
		cout << foo << "\n";
	}
	{
		using F = Fraction;
		array<Fraction, 3> values1 = {F(1, 1), F(1, 2), F(1, 3)};
		array<Fraction, 3> values2 = {1, 2, 3};
		array<int, 3> values3 = {10, 20, 30};
		Vector<Fraction> foo(values1);
		Vector<Fraction> bar(values2);
		Vector<int> qux(values3);

		auto fn = [=, &spam = foo] (int i) {
			cout << spam[i] << " " << bar[i] << "\n";
		};
		fn(1);
		foo[1] = F(1, 3);
		bar[1] = F(1, 3);
		fn(1);

		cout << foo << "\n";
		foo += Vector<Fraction>(qux);
		cout << foo << "\n";
		foo -= bar;
		cout << foo << "\n";
	}
}

void runLambdaExample() {
	class Spam {
	public:
		void print() {
			val = 9000;
			string foo = "hello";
			function<void()> fn = [=] () mutable {
				cout << foo << "\n";
				foo = "wurld";
			};
			val = 69;
			fn();
			foo = "fren";
			val = 42;
			fn();
		}

		int val = 0;
	};

	Spam spam = {};
	spam.print();

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

	auto&& [line, curve] = generateCurves<double>(10, 2, 1);
	for (auto&& x : {-1, 0, 1, 5, 10})
		cout << line(x) << " ";
	cout << "\n";
}


int main() {
	//runArrayExample();
	//cout << "\n\n\n";
	//runPrintExample();
	//cout << "\n\n\n";
	//runVectorExample();
	//cout << "\n\n\n";
	//runLambdaExample();
}

