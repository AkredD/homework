//
// Created by AkredD on 27.05.2016.
//

#ifndef TASK4_FORMAT_H
#define TASK4_FORMAT_H

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

// Processes rest of format (without arguments) and throws out_of_range if format declared some
template<typename ...Args>
string sprint(const string &fmt, int &count_of_args, size_t &index, Args &... rest) {
	if (index < fmt.length()) {
		string formatSpec, result = "";
		bool width_arg = false, precision_arg = false;
		Format format = def;
		do {
			formatSpec = read_format(fmt, index, width_arg, precision_arg, format);
			if (formatSpec[0] != '%') {
				result += formatSpec;
			}
			else if (formatSpec == "%") {
				result += formatSpec;
			}
			else {
				throw out_of_range("Format : not enough arguments");
			}
		} while (index < fmt.length());
		return result;
	}
	return "";
}

template<typename ...Args>
string format(const string &formatString, const Args &... args);

// Read format, read the required number of arguments and formatted it
template<typename T, typename ...Args>
string sprint(const string &fmt, int &count_of_extraArgs, size_t &index, T &head, Args &... rest) {
	// If we use some args (for precision, width) we need skip it
	if (count_of_extraArgs > 0) {
		return sprint(fmt, --count_of_extraArgs, index, rest...);
	}
	size_t len = fmt.length();
	string formatSpec;
	while (index < len) {
		bool width_arg = false, precision_arg = false;
		Format format = def;
		string temp;
		do {
			formatSpec = read_format(fmt, index, width_arg, precision_arg, format);
			temp += formatSpec;
		} while (formatSpec == "%" || formatSpec[0] != '%');

		formatSpec = temp;
		count_of_extraArgs = width_arg + precision_arg;
		string result;
		result = count_of_extraArgs == 0 ? format_arg(formatSpec, format, head, rest...) :
			(count_of_extraArgs == 1 ? format_2_args(formatSpec, format, head, rest...) :
				format_3_args(formatSpec, format, head, rest...));
		return result + sprint(fmt, count_of_extraArgs, index, rest...);
	}
	throw invalid_argument("Empty format and extra args");
}

enum Format {
	def = '\0',

	d = 'd',
	i = 'i',
	u = 'u',
	o = 'o',
	x = 'x',
	X = 'X',
	f = 'f',
	F = 'F',
	e = 'e',
	E = 'E',
	g = 'g',
	G = 'G',
	a = 'a',
	A = 'A',
	c = 'c',
	s = 's',
	p = 'p',
	n = 'n',
};

// Returns formatted argument For string arg need conversion to char * type
template<typename T>
string get_format_string(T &arg, Format &format, string &format_str,
	typename enable_if<is_same<T, string>::value ||
	is_same<T, const string>::value>::type * = 0) {
	char *arg_c = const_cast<char *>(arg.c_str());
	if (!check_type(format, arg_c)) {
		throw invalid_argument("Format : invalid type of  argument");
	}
	string result((size_t)snprintf(NULL, 0, const_cast<char *>(format_str.c_str()), arg_c), '\0');
	snprintf(const_cast<char *>(result.c_str()), result.length() + 1, const_cast<char *>(format_str.c_str()),
		arg_c);
	return result;
}

string read_format(const string &fmt, size_t &index, bool &width_arg, bool &precision_arg,
	Format &f);
// Replace '*' in format on the the corresponding value of width or precision
string replace_format(const string &fmt, int &wp);

// Check string type
template<typename T>
bool check_type(Format format, T variable,
	typename enable_if<(is_convertible<T, string>::value), string>::type * = 0) {
	if (format == p || format == s) {
		return true;
	}
	else {
		throw invalid_argument("Invalid argument: string expected");
	}
}

//Check type pointers type
template<typename T>
bool check_type(Format format, T variable,
	typename enable_if<(!(is_convertible<T, string>::value) &&
		(is_pointer<T>::value)), string>::type * = 0) {
	return format == p;
}

//Check numbers or char type
template<typename T>
bool check_type(Format format, T variable, typename enable_if<
	!(is_convertible<T, string>::value) && !(is_pointer<T>::value), string>::type * = 0) {
	if (!is_convertible<T, int>::value) {
		throw invalid_argument("Invalid argument");
	}
	if (format == d || format == i || format == u || format == o || format == x || format == X || format == f ||
		format == F || format == e || format == E || format == g || format == G || format == a || format == A ||
		format == c) {
		return true;
	}
	throw invalid_argument("Invalid argument");
}



// Returns formatted arguments for other types
template<typename T>
string get_format_string(T &arg, Format &format, string &format_str, typename enable_if<
	!is_same<T, string>::value && !is_same<T, const string>::value>::type * = 0) {
	// This check doesn't work
	if (!check_type(format, arg)) {
		throw invalid_argument("Format : invalid type of  argument");
	}
	string result((size_t)snprintf(NULL, 0, const_cast<char *>(format_str.c_str()), arg), '\0');
	snprintf(const_cast<char *>(result.c_str()), result.length() + 1, const_cast<char *>(format_str.c_str()), arg);
	return result;
}


// Convert arg to int. Need for read width and precision
template<typename T>
int read_int(T &arg, typename enable_if<is_integral<T>::value>::type * = 0) {
	return (int)arg;
}

template<typename T>
int read_int(T &arg, typename enable_if<!is_integral<T>::value>::type * = 0) {
	throw invalid_argument(string("Invalid argument, expected integral type"));
}

// Read width, precision and argument and returns formatted argument
template<typename T1, typename T2, typename T3, typename ...Args>
string format_3_args(string &formatSpec, Format &format, T1 &width, T2 &precision, T3 &arg,
	Args &... rest) {
	int width_val = read_int(width);
	formatSpec = replace_format(formatSpec, width_val);
	int precision_val = read_int(precision);
	formatSpec = replace_format(formatSpec, precision_val);
	return get_format_string(arg, format, formatSpec);
};

template<typename ...Args>
string format_3_args(string &formatSpec, Format &f, Args &... rest) {
	throw out_of_range("Not enough arguments");
}

// Read width or precision and returns formatted argument
template<typename T1, typename T2, typename ...Args>
string format_2_args(string &formatSpec, Format &format, T1 &ws, T2 &arg, Args &... rest) {
	int val = read_int(ws);
	formatSpec = replace_format(formatSpec, val);
	return get_format_string(arg, format, formatSpec);
};

template<typename ...Args>
string format_2_args(string &formatSpec, Format &f, Args &... rest) {
	throw out_of_range("Not enough arguments");
}

// Read argument and returns formatted argument
template<typename T, typename ...Args>
string format_arg(string &formatSpec, Format &format, T &arg, Args &... rest) {
	return get_format_string(arg, format, formatSpec);
};

template<typename ...Args>
string format_arg(string &formatSpec, Format &f, Args &... rest) {
	throw out_of_range("Not enough arguments");
}

/**
* Returns a formatted string using the inputted format string and arguments.
* Throws  invalid_argument, if a format string contains an illegal syntax or illegal arguments in formatting
* Throws out_of_range, if the arguments' list contains too few or too many arguments
*/
template<typename ...Args>
string format(const string &formatString, const Args &... args) {
	int count_of_args = 0;
	size_t index = 0;
	return sprint(formatString, count_of_args, index, args...);
}


#endif //TASK4_FORMAT_H
