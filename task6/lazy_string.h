#ifndef TASK6_LAZY_STRING_H
#define TASK6_LAZY_STRING_H

#include <string>
#include <istream>
#include <ostream>
#include <memory>

using namespace std;

// Class lazy string for string implements copy-on-write
class lazy_string {
	
	struct char_ref {
		friend class lazy_string;
		operator char() const;
		 // Implements write on lazy_string
		char_ref &operator=(char);
	private:
		char_ref(lazy_string *, size_t);
		const size_t index;
		lazy_string *const ls;
	};

public:
	operator string();
	
	/*
        * Constructs empty lazy_string
        */
	lazy_string();
	
	/*
        * Constructs new lazy_string from received string
        *First argument is string
        */
	lazy_string(const std::string &str);
        /*
        * Returns the substring including a left border, and not including the right
        * If first argument is incorrect throw sout_of_range
        * If second argument is incorrect, returns substring from first argument to the end of line
        * First argument is index of first character of substring
        * Second argument is length of required substring
        * return lazy_string containing characters  [start, start + len]
        * return substring containing characters [start, str.len()] if start + len > str.len()
        * return empty lazy string if start == length of string
        * throws std::out_of_range
        */
	lazy_string substr(size_t beg, size_t len);
	
	/*
        * Returns the number of characters in the string.
        * return string's length (size).
        */
	size_t sizes() const;
	
	/*
        * Returns the number of characters in the string.
        * return string's length (size).
        */
	size_t length() const;
	
	/*
        * The function automatically checks whether position is the valid position of a character in the string
        * (i.e., whether position is less than the string length), throwing an out_of_range exception if it is not.
        * param First arguments is index of the character to return
        * return reference to the character at position pos in the string.
        */
	char_ref at(size_t beg);
	char at(size_t beg) const;
	
	/*
        * Returns a character at the specified index
        * does not check correctness of index
        * param First arguments is index of the character to return
        * return a character at the specified index
        */
        char_ref operator[](size_t beg);
	char operator[](size_t pos) const;
	
	 /*
         * Extracts a lazy_string from the input stream input, storing the sequence in lazy,
         * which is overwritten (the previous value of ls is replaced).
         * param First arguments is stream.
         * param Second arguments is lazy_string.
         * return the same as parameter input.
         */
	friend istream &operator >>(istream &is, lazy_string &ls);
	
	/**
        * Inserts the sequence of characters that conforms value of lazy into output.
        * param First arguments is output stream.
        * param Second arguments is lazy_string.
        * return the same as parameter output.
        */
	friend ostream &operator <<(ostream &os, lazy_string &ls);

private:
	size_t start, size;
	shared_ptr<string> str;
	lazy_string(size_t start, size_t sz, shared_ptr<string> str);
};

#endif
