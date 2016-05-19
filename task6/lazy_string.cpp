#include <iostream>
#include "lazy_string.h"

using namespace std;

lazy_string::operator string() {
	return str->substr(start, size);
};

lazy_string::lazy_string() {
	this->size = 0;
	this->start = 0;
	this->str = make_shared<string>("");
}

lazy_string::lazy_string(const string &strin) {
	this->start = 0;
	this->size = strin.length();
	this->str = make_shared<string>(strin);
}

lazy_string::lazy_string(size_t start, size_t size, shared_ptr<string> str) {
	this->start = start;
	this->size = size;
	this->str = str;
}

lazy_string lazy_string::substr(size_t beg = 0, size_t len = 0) {
	if (beg + len > size) {
		return lazy_string(beg, len, this->str);
	}
	else {
		return lazy_string(beg, beg + len, this->str);
	}
}

size_t lazy_string::sizes() const{
	return size;
}

size_t lazy_string::length() const {
	return size;
}

char lazy_string::at(size_t index) const {
	if (index > size - 1) throw "null pointer exception";
	return (*str)[start + index];
}

char lazy_string::operator[](size_t index) const{
	return (*str)[start + index];
}


istream &operator>>(istream &is, lazy_string &ls) {
	auto strL = make_shared<string>();
	is >> *strL;
	ls.str = strL;
	ls.start = 0;
	ls.size = strL->length();
	return is;
}

ostream &operator<<(ostream &output, lazy_string &ls) {
	for (size_t i = 0; i < ls.length(); i++) output << ls[i];
	return output;
}

lazy_string::char_ref::char_ref(lazy_string *ls, size_t index) : ls(ls), index(index) { }

lazy_string::char_ref &lazy_string::char_ref::operator=(char c) {
	if (ls->str.use_count() > 1) {
		ls->str = make_shared<string>(ls->str->substr(ls->start, ls->size));	
		ls->start = 0;
	}
	(*ls->str)[ls->start + index] = c;
	return *this;
}

lazy_string::char_ref::operator char() const {
	return (*ls->str)[ls->start + index];
}