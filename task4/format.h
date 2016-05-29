#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <stdexcept>
#include <iostream>


template<typename ...Args>
std::string format(const std::string &formatString, const Args &... args);


enum Format
{
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

Format get_format(const std::string &form, size_t &i);
bool has_arg(const std::string &form, size_t &i);
std::string reeadFA(const std::string &form, size_t &i, bool &width_arg, bool &precision_arg, Format &format);
std::string replace_format(const std::string &form, int &wp);
bool flags(char);
bool width(char);
bool precision(char);
void specifier(const std::string &form, size_t &i);



std::string read_format(const std::string &form, size_t &i, bool &width_arg, bool &precision_arg, Format &f);

// Replaces '*' in format to value of width or precision
std::string replace_format(const std::string &form, int &wp);

// Returns formatted argument (for str)
template<typename T>
std::string get_format_string(T &arg, Format &format, std::string &format_str, typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, const std::string>::value>::type * = 0)
{
	char *arg_c = const_cast<char *>(arg.c_str());
	if (!check_type(format, arg_c))
	{
		throw std::invalid_argument("Format : invalid type of  argument");
	}
	std::string result((size_t)snprintf(NULL, 0, const_cast<char *>(format_str.c_str()), arg_c), '\0');
	snprintf(const_cast<char *>(result.c_str()), result.length() + 1, const_cast<char *>(format_str.c_str()),
		arg_c);
	return result;
}

// Check type of string
template<typename T>
bool check_type(Format format, T variable, typename std::enable_if<(std::is_convertible<T, std::string>::value), std::string>::type * = 0)
{
	if (format == p || format == s)
	{
		return true;
	}
	else
	{
		throw std::invalid_argument("string expected");
	}
}

// Returns formatted arguments for other types
template<typename T>
std::string get_format_string(T &arg, Format &format, std::string &format_str, typename std::enable_if<!std::is_same<T, std::string>::value && !std::is_same<T, const std::string>::value>::type * = 0)
{
	if (!check_type(format, arg))
	{
		throw std::invalid_argument("Format : invalid type of  argument");
	}
	std::string result((size_t)snprintf(NULL, 0, const_cast<char *>(format_str.c_str()), arg), '\0');
	snprintf(const_cast<char *>(result.c_str()), result.length() + 1, const_cast<char *>(format_str.c_str()), arg);
	return result;
}

//Check type pointers
template<typename T>
bool check_type(Format format, T variable, typename std::enable_if<(!(std::is_convertible<T, std::string>::value) && (std::is_pointer<T>::value)), std::string>::type * = 0)
{
	return format == p;
}

//Check numbers or char type
template<typename T>
bool check_type(Format format, T variable, typename std::enable_if<!(std::is_convertible<T, std::string>::value) && !(std::is_pointer<T>::value), std::string>::type * = 0)
{
	if (!std::is_convertible<T, int>::value)
	{
		throw std::invalid_argument("Invalid argument");
	}
	if (format == d || format == i || format == u || format == o || format == x || format == X || format == f ||
		format == F || format == e || format == E || format == g || format == G || format == a || format == A ||
		format == c)
	{
		return true;
	}
	throw std::invalid_argument("Invalid argument");
}


//Converts arg to int, used for reading width and precision
template<typename T>
int read_int(T &arg, typename std::enable_if<std::is_integral<T>::value>::type * = 0)
{
	return (int)arg;
}

template<typename T>
int read_int(T &arg, typename std::enable_if<!std::is_integral<T>::value>::type * = 0)
{
	throw std::invalid_argument(std::string("Invalid argument, expected integral type"));
}


// Read width, precision and argument and returns formatted argument
template<typename T1, typename T2, typename T3, typename ...Args>
std::string format_3_args(std::string &formatParams, Format &format, T1 &width, T2 &precision, T3 &arg, Args &... rest)
{
	int width_val = read_int(width);
	formatParams = replace_format(formatParams, width_val);
	int precision_val = read_int(precision);
	formatParams = replace_format(formatParams, precision_val);
	return get_format_string(arg, format, formatParams);
};


template<typename ...Args>
std::string format_3_args(std::string &formatS, Format &f, Args &... rest)
{
	throw std::out_of_range("Not enough arguments");
}

// Read width or precision. Returns formatted argument
template<typename T1, typename T2, typename ...Args>
std::string format_2_args(std::string &formatS, Format &format, T1 &ws, T2 &arg, Args &... rest)
{
	int val = read_int(ws);
	formatS = replace_format(formatS, val);
	return get_format_string(arg, format, formatS);
};

template<typename ...Args>
std::string format_2_args(std::string &formatS, Format &f, Args &... rest)
{
	throw std::out_of_range("Not enough arguments");
}

// Read argument and, formatted argument
template<typename T, typename ...Args>
std::string format_arg(std::string &formatS, Format &format, T &arg, Args &... rest)
{
	return get_format_string(arg, format, formatS);
};

template<typename ...Args>
std::string format_arg(std::string &formatS, Format &f, Args &... rest)
{
	throw std::out_of_range("Not enough arguments");
}


// Processes rest of format (without arguments) and throws out_of_range if format declared more
template<typename ...Args>
std::string sprint(const std::string &form, int &count_of_args, size_t &i, Args &... rest)
{
	if (i < form.length())
	{
		std::string formatS, result = "";
		bool width_arg = false, precision_arg = false;
		Format format = def;
		do
		{
			formatS = read_format(form, i, width_arg, precision_arg, format);
			if (formatS[0] != '%')
			{
				result += formatS;
			}
			else if (formatS == "%")
			{
				result += formatS;
			}
			else
			{
				throw std::out_of_range("Not enough arguments in Format");
			}
		} while (i < form.length());
		return result;
	}
	return "";
}

// Read format, read the required number of arguments and formatted it
template<typename T, typename ...Args>
std::string sprint(const std::string &form, int &countE, size_t &i, T &head, Args &... rest)
{
	if (countE > 0)
	{
		return sprint(form, --countE, i, rest...);
	}
	size_t len = form.length();
	std::string formatS;
	while (i < len)
	{
		bool width_arg = false, precision_arg = false;
		Format format = def;
		std::string temp;
		do
		{
			formatS = read_format(form, i, width_arg, precision_arg, format);
			temp += formatS;
		} while (formatS == "%" || formatS[0] != '%');

		formatS = temp;
		countE = width_arg + precision_arg;
		std::string result;
		result = (countE == 0 ? format_arg(formatS, format, head, rest...) :
			(countE == 1 ? format_2_args(formatS, format, head, rest...) :
				format_3_args(formatS, format, head, rest...)));
		return result + sprint(form, countE, i, rest...);
	}
	throw std::invalid_argument("Extra arguments");
}


/**
* Returns a formatted string using the inputted format string and arguments.
* Throws  std::invalid_argument, if a format string contains an illegal syntax or illegal arguments in formatting
* Throws std::out_of_range, if the arguments' list contains too few or too many arguments
*/
template<typename ...Args>
std::string format(const std::string &formatString, const Args &... args)
{
	int count_of_args = 0;
	size_t i = 0;
	return sprint(formatString, count_of_args, i, args...);
}
#endif