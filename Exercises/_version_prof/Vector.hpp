///
/// Exemple des slides sur la copie d'objet.
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <memory>
#include <utility>
#include <iostream>
#include <span>

#include <cppitertools/itertools.hpp>

using namespace std;
using namespace iter;


template <typename T>
class Vector {
	template <typename U>
	friend class Vector;

public:
	Vector() = default; // Petit truc pour dire au compilateur de générer pour nous le ctor par défaut.
	Vector(const Vector& other); // Par copie
	template <typename U>
	Vector(const Vector<U>& other); // Par copie
	Vector(Vector&& other); // Par déplacement (ou par move)
	Vector(int size, T value = {});
	Vector(span<const T> values);
	~Vector();

	Vector& operator=(const Vector& other);
	template <typename U>
	Vector& operator=(const Vector<U>& other);
	Vector& operator=(Vector&& other);

	T* getData();
	const T* getData() const;
	int getSize() const;

	void resize(int size);
	bool hasSameSize(const Vector& other) const;

	T& operator[](int index);
	const T& operator[](int index) const;

	Vector operator-() const;
	Vector operator+() const;
	Vector operator+(const Vector& rhs) const;
	Vector operator-(const Vector& rhs) const;

	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);

	template <typename>
	friend ostream& operator<<(ostream& lhs, const Vector& rhs);

private:
	unique_ptr<T[]> values_;
	int size_ = 0;
};


template <typename T>
Vector<T>::Vector(const Vector& other) {
	// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation.
	*this = other;
	// On a techniquement une double initialisation, mais on sait que c'est trival dans ce cas et ça simplifie le code vu qu'on a besoin d'être ans un état valide pour appeler l'opérateur d'affectation.
}

template <typename T>
template <typename U>
Vector<T>::Vector(const Vector<U>& other) {
	// On réutilise l'opérateur d'affectation templaté.
	*this = other;
}

template <typename T>
Vector<T>::Vector(Vector&& other) {
	// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation par déplacement (« move »).
	*this = move(other);
}

template <typename T>
Vector<T>::Vector(int size, T value) {
	resize(size);
	for (auto&& i : range(size_))
		values_[i] = value;
}

template <typename T>
Vector<T>::Vector(span<const T> values) {
	// Pour copier d'un tableau donné, on utilise notre resize() puis on copie. L'initialisation par défaut des membres se fait avant.
	resize((int)values.size());
	for (auto&& [i, v] : enumerate(values))
		values_[i] = v;
}

template <typename T>
Vector<T>::~Vector() {
	// Pour désallouer, on réutilise notre resize().
	resize(0);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
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

template <typename T>
template <typename U>
Vector<T>& Vector<T>::operator=(const Vector<U>& other) {
	resize(0);
	resize(other.size_);
	for (int i : range(other.size_))
		values_[i] = T(other.values_[i]);
	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) {
	resize(0);
	// On « vole » les ressources de l'autre objet.
	values_ = move(other.values_);
	size_ = other.size_;
	// On remet à zéro l'autre objet (donc laissé dans un état valide).
	other.size_ = 0;
	// On veut permettre l'affectation en cascade.
	return *this;
}

template <typename T>
T* Vector<T>::getData() {
	return values_.get();
}

template <typename T>
const T* Vector<T>::getData() const {
	return values_.get();
}

template <typename T>
int Vector<T>::getSize() const {
	return size_;
}

template <typename T>
void Vector<T>::resize(int size) {
	auto old = move(values_);
	// Redimensionner à 0 fait juste désallouer le contenu.
	if (size != 0) {
		// On alloue et on copie les anciennes données.
		values_ = make_unique<T[]>(size);
		// Si on redimensionne à plus petit, on copie juste les données qui entre (d'où le min).
		for (int i : range(min(size_, size)))
			values_[i] = move(old[i]);
	}
	size_ = size;
	old.reset();
}

template <typename T>
bool Vector<T>::hasSameSize(const Vector& other) const {
	return size_ == other.size_;
}

template <typename T>
T& Vector<T>::operator[](int index) {
	return values_[index];
}

template <typename T>
const T& Vector<T>::operator[](int index) const {
	return values_[index];
}

template <typename T>
Vector<T> Vector<T>::operator+() const {
	return *this;
}

template <typename T>
Vector<T> Vector<T>::operator-() const {
	Vector result = *this;
	for (auto&& i : range(size_))
		result[i] = -result[i];
	return result;
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector& rhs) const {
	if (not hasSameSize(rhs))
		return {};
	Vector result(size_);
	for (auto&& i : range(size_))
		result[i] = (*this)[i] + rhs[i];
	return result;
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector& rhs) const {
	return *this + -rhs;
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector& rhs) {
	return *this = *this + rhs;
}

template <typename T>
Vector<T>& Vector<T>::operator-=(const Vector& rhs) {
	return *this = *this - rhs;
}


template <typename T>
ostream& operator<<(ostream& lhs, const Vector<T>& rhs) {
	lhs << "[";
	for (auto i : range(rhs.getSize())) {
		lhs << rhs[i];
		if (i != rhs.getSize() - 1)
			lhs << " ";
	}
	lhs << "]";
	return lhs;
}

