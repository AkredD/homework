#ifndef TASK6_LAZY_STRING_H
#define TASK6_LAZY_STRING_H

#include <string>
#include <istream>
#include <ostream>
#include <memory>

using namespace std;

class lazy_string {
	
	struct char_ref {
		friend class lazy_string;
		operator char() const;
		char_ref &operator=(char);
	private:
		char_ref(lazy_string *, size_t);
		const size_t index;
		lazy_string *const ls;
	};

public:
	operator string();
	lazy_string();
	lazy_string(const std::string &str);
	lazy_string substr(size_t beg, size_t len);
	size_t sizes() const;
	size_t length() const;
	//char_ref at(size_t beg);
	char at(size_t beg) const;
    //char_ref operator[](size_t beg);
	char operator[](size_t pos) const;
	friend istream &operator >>(istream &is, lazy_string &ls);
	friend ostream &operator <<(ostream &os, lazy_string &ls);

private:
	size_t start, size;
	shared_ptr<string> str;
	lazy_string(size_t start, size_t sz, shared_ptr<string> str);
};

#endif