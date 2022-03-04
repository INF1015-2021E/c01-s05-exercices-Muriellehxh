///
/// Exemple des slides sur la copie d'objet.
///


#include <cstddef>
#include <cstdint>

#include <memory>
#include <utility>
#include <iostream>
#include <span>

#include <cppitertools/itertools.hpp>

#include "Vector.hpp"

using namespace std;
using namespace iter;


Vector::Vector(const Vector& other) {
	// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation.
	*this = other;
	// On a techniquement une double initialisation, mais on sait que c'est trival dans ce cas et ça simplifie le code vu qu'on a besoin d'être ans un état valide pour appeler l'opérateur d'affectation.
}

Vector::Vector(Vector&& other) {
	// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation par déplacement (« move »).
	*this = move(other);
}

Vector::Vector(int size, int value) {
	resize(size);
	for (auto&& i : range(size_))
		values_[i] = value;
}

Vector::Vector(span<const int> values) {
	// Pour copier d'un tableau donné, on utilise notre resize() puis on copie. L'initialisation par défaut des membres se fait avant.
	resize((int)values.size());
	for (auto&& [i, v] : enumerate(values))
		values_[i] = v;
}

Vector::~Vector() {
	// Pour désallouer, on réutilise notre resize().
	resize(0);
}

Vector& Vector::operator=(const Vector& other) {
	// On vérifie qu'on ne copie pas dans nous-même.
	if (&other == this)
		return *this;
	// Pourrait optimiser en ne réallouant pas si on a l'espace suffisant.
	resize(0);
	resize(other.size_);
	for (int i : range(other.size_))
		values_[i] = other.values_[i];
	// On veut permettre l'affectation en cascade.
	return *this;
}

Vector& Vector::operator=(Vector&& other) {
	resize(0);
	// On « vole » les ressources de l'autre objet.
	values_ = move(other.values_);
	size_ = other.size_;
	// On remet à zéro l'autre objet (donc laissé dans un état valide).
	other.size_ = 0;
	// On veut permettre l'affectation en cascade.
	return *this;
}

int* Vector::getData() {
	return values_.get();
}

const int* Vector::getData() const {
	return values_.get();
}

int Vector::getSize() const {
	return size_;
}

void Vector::resize(int size) {
	auto old = move(values_);
	// Redimensionner à 0 fait juste désallouer le contenu.
	if (size != 0) {
		// On alloue et on copie les anciennes données.
		values_ = make_unique<int[]>(size);
		// Si on redimensionne à plus petit, on copie juste les données qui entre (d'où le min).
		for (int i : range(min(size_, size)))
			values_[i] = old[i];
	}
	size_ = size;
	old.reset();
}

bool Vector::hasSameSize(const Vector& other) const {
	return size_ == other.size_;
}

int& Vector::operator[](int index) {
	return values_[index];
}

const int& Vector::operator[](int index) const {
	return values_[index];
}

Vector Vector::operator+() const {
	return *this;
}

Vector Vector::operator-() const {
	Vector result = *this;
	for (auto&& i : range(size_))
		result[i] = -result[i];
	return result;
}

Vector Vector::operator+(const Vector& rhs) const {
	if (not hasSameSize(rhs))
		return {};
	Vector result(size_);
	for (auto&& i : range(size_))
		result[i] = (*this)[i] + rhs[i];
	return result;
}

Vector Vector::operator-(const Vector& rhs) const {
	return *this + -rhs;
}

Vector& Vector::operator+=(const Vector& rhs) {
	return *this = *this + rhs;
}

Vector& Vector::operator-=(const Vector& rhs) {
	return *this = *this - rhs;
}


ostream& operator<<(ostream& lhs, const Vector& rhs) {
	lhs << "[";
	for (auto i : range(rhs.getSize())) {
		lhs << rhs[i];
		if (i != rhs.getSize() - 1)
			lhs << " ";
	}
	lhs << "]";
	return lhs;
}
