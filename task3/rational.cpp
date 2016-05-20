#include "rational.h"

int rational::getNum() const {
	return this->numer;
}

int rational::getDenom() const {
	return this->denom;
}

rational::rational(int numer) {
	this->numer = numer;
	this->denom = 1;
}

rational::rational(int numer, int denom) : numer(numer), denom(denom) {
	int gcd = calcGCD(numer, denom);
	this->numer = numer / gcd;
	this->denom = denom / gcd;
}

int rational::calcGCD(int l, int r) const{
	return r ? calcGCD(r, l % r) : l;
}

rational const rational::operator-(rational const &second) const{
	return rational(this->numer * second.denom - this->denom * second.numer,
		this->denom * second.denom);
}

rational const rational::operator +(rational const &second) const{
	return rational(this->numer * second.denom + this->denom * second.numer,
		this->denom * second.denom);
}

rational const rational::operator *(rational const &second) const{
	return rational(this->numer * second.numer,
		this->denom * second.denom);
}

rational const rational::operator /(rational const &second) const{
	return rational(this->numer * second.denom,
		this->denom * second.numer);
}
