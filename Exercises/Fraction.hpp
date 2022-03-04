///
/// Exemple de classe de fraction (numérateur/dénominateur) avec opérateurs arithmétiques.
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <iostream>
#include <tuple>

using namespace std;


class Fraction {
public:
	Fraction(int numer = 0, int denom = 1);
	Fraction(pair<int, int> p);

	int getNumer() const { return numer_; }
	int getDenom() const { return denom_; }

	double asDouble() const;
	Fraction& increment();

	Fraction operator+() const; // Unaire (+foo, fait rien)
	Fraction operator-() const; // Unaire (-foo, négation)
	Fraction& operator++(); // Préfixe (++foo)
	Fraction operator++(int); // Postfixe (foo++)
	Fraction& operator--();
	Fraction operator--(int);

	Fraction operator+(const Fraction& rhs) const;
	Fraction operator+(int rhs) const;
	Fraction operator-(const Fraction& rhs) const;
	Fraction operator-(int rhs) const;
	Fraction operator*(const Fraction& rhs) const;
	Fraction operator*(int rhs) const;
	Fraction operator/(const Fraction& rhs) const;
	Fraction operator/(int rhs) const;

	bool operator==(const Fraction& rhs) const;
	bool operator!=(const Fraction& rhs) const;
	bool operator<(const Fraction& rhs) const;
	bool operator<=(const Fraction& rhs) const;
	bool operator>(const Fraction& rhs) const;
	bool operator>=(const Fraction& rhs) const;

	Fraction& operator+=(const Fraction& rhs);
	Fraction& operator+=(int rhs);
	Fraction& operator-=(const Fraction& rhs);
	Fraction& operator-=(int rhs);
	Fraction& operator*=(const Fraction& rhs);
	Fraction& operator*=(int rhs);
	Fraction& operator/=(const Fraction& rhs);
	Fraction& operator/=(int rhs);

	friend Fraction operator+(int lhs, const Fraction& rhs);
	friend Fraction operator-(int lhs, const Fraction& rhs);
	friend Fraction operator*(int lhs, const Fraction& rhs);
	friend Fraction operator/(int lhs, const Fraction& rhs);
	friend ostream& operator<<(ostream& lhs, const Fraction& rhs);

private:
	void simplify();

	int numer_;
	int denom_;
};

