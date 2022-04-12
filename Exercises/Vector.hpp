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

// on met classe template T
template <typename T>
class Vector {
public: 
	Vector() = default; // Petit truc pour dire au compilateur de générer pour nous le ctor par défaut.
	Vector(const Vector& other); // Par copie
	Vector(Vector&& other); // Par déplacement (ou par move)
	Vector(int size, const T& value = {}); // avant = int value ={}
	Vector(span<const int> values);
	~Vector(); // pas besoin de mettre destructeur, pcq on a ptr intelligents

	// pas besoin de mettre Vector<T>&, pcq on est a l'INTERIEUR DE DEF DE CLASSE
	// type implicite 
	Vector& operator=(const Vector& other); 
	Vector& operator=(Vector&& other);

	T* getData(); // before int*
	const T* getData() const;  // before int*
	int getSize() const;

	void resize(int size);
	bool hasSameSize(const Vector& other) const;

	int& operator[](int index);
	const int& operator[](int index) const;

	Vector operator-() const;
	Vector operator+() const;
	Vector operator+(const Vector& rhs) const; // addition de 2 vecteurs de meme type Vector
	Vector operator-(const Vector& rhs) const;

	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);

	friend ostream& operator<<(ostream& lhs, const Vector& rhs);

private:
	unique_ptr<int[]> values_;
	int size_ = 0;
};


template <typename T> // a mettre devant methode constructrices
Vector::Vector(const Vector& other)
{
// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation.
	*this = other;
	// On a techniquement une double initialisation, mais on sait que c'est trival dans ce cas et ça simplifie le code vu qu'on a besoin d'être ans un état valide pour appeler l'opérateur d'affectation.
}

template <typename T>
Vector::Vector(Vector&& other)
{
// On laisse l'initialisation par défaut puis on réutilise l'opérateur d'affectation par déplacement (« move »).
	*this = move(other);
}

template <typename T>
Vector::Vector(int size, int value)
{
	resize(size);
	for(auto&& i : range(size_))
		values_[i] = value;
}

Vector::Vector(span<const int> values)
{
// Pour copier d'un tableau donné, on utilise notre resize() puis on copie. L'initialisation par défaut des membres se fait avant.
	resize((int) values.size());
	for(auto&& [i, v] : enumerate(values))
		values_[i] = v;
}


// PAS DE DESTRUCTEUR

//Vector::~Vector()
//{
//// Pour désallouer, on réutilise notre resize().
//	resize(0);
//}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) // avant Vector &

// Vector, c un template, Vector<int>, c un type concret
{
// On vérifie qu'on ne copie pas dans nous-même.
	if(&other == this)
		return *this;
	// Pourrait optimiser en ne réallouant pas si on a l'espace suffisant.
	resize(0);
	resize(other.size_);
	for(int i : range(other.size_))
		values_[i] = other.values_[i];
	// On veut permettre l'affectation en cascade.
	return *this;
}

Vector& Vector::operator=(Vector&& other)
{
	resize(0);
	// On « vole » les ressources de l'autre objet.
	values_ = move(other.values_);
	size_ = other.size_;
	// On remet à zéro l'autre objet (donc laissé dans un état valide).
	other.size_ = 0;
	// On veut permettre l'affectation en cascade.
	return *this;
}

int* Vector::getData()
{
	return values_.get();
}

const int* Vector::getData() const
{
	return values_.get();
}

int Vector::getSize() const
{
	return size_;
}

void Vector::resize(int size)
{
	auto old = move(values_);
	// Redimensionner à 0 fait juste désallouer le contenu.
	if(size != 0)
	{
// On alloue et on copie les anciennes données.
		values_ = make_unique<int[]>(size);
		// Si on redimensionne à plus petit, on copie juste les données qui entre (d'où le min).
		for(int i : range(min(size_, size)))
			values_[i] = old[i];
	}
	size_ = size;
	old.reset();
}

bool Vector::hasSameSize(const Vector& other) const
{
	return size_ == other.size_;
}


template <typename T>
T& Vector<T>::operator[](int index)
{
	return values_[index];
}

// faire template T aussi
const int& Vector<int>::operator[](int index) const
{
	return values_[index];
}


Vector<int> Vector<int>::operator+() const
{
	return *this;
}

Vector Vector::operator-() const
{
	Vector result = *this;
	for(auto&& i : range(size_))
		result[i] = -result[i];
	return result;
}

Vector Vector::operator+(const Vector& rhs) const
{
	if(not hasSameSize(rhs))
		return {};
	Vector result(size_);
	for(auto&& i : range(size_))
		result[i] = (*this)[i] + rhs[i];
	return result;
}

Vector Vector::operator-(const Vector& rhs) const
{
	return *this + -rhs;
}

Vector& Vector::operator+=(const Vector& rhs)
{
	return *this = *this + rhs;
}

Vector& Vector::operator-=(const Vector& rhs)
{
	return *this = *this - rhs;
}


ostream& operator<<(ostream& lhs, const Vector& rhs)
{
	lhs << "[";
	for(auto i : range(rhs.getSize()))
	{
		lhs << rhs[i];
		if(i != rhs.getSize() - 1)
			lhs << " ";
	}
	lhs << "]";
	return lhs;
}
s