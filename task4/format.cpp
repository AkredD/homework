#include "format.h"

std::string replace_format(const std::string &fmt, int &wp)
{
	size_t first = fmt.find_first_of('*');
	std::string result = fmt.substr(0, first) + std::to_string(wp) + fmt.substr(first + 1, fmt.length());
	return result;
}

bool flags(char c)
{
	return c == '-' || c == '+' || c == ' ' || c == '#' || c == '0';
}


bool width(char c)
{
	return (c == '*' || isdigit(c));
}


bool precision(char c)
{
	return (c == '.');
}


void specifier(const std::string &fmt, size_t &index)
{
	std::string s;
	s.push_back(fmt[index]);
	if (index + 1 < fmt.length() && fmt[index + 1] == fmt[index])
	{
		s.push_back(fmt[index + 1]);
	}
	if (s == "l" || s == "ll" || s == "h" || s == "hh"
		|| s == "j" || s == "z" || s == "t" || s == "L")
	{
		index += s.length();
	}
}

std::string read_format(const std::string &formatstr, size_t &index, bool &width_arg, bool &precision_arg, Format &f)
{
	size_t len = formatstr.length(), start = index;
	char c;
	for (index; index < len; index++)
	{
		c = formatstr[index];
		if (c != '%')
		{
			continue;
		}
		else if (index != start)
		{
			return formatstr.substr(start, index - start);
		}
		if (index + 1 < len && formatstr[index + 1] == '%')
		{
			index += 2;
			return "%";
		}
		return read_format_with_args(formatstr, index, width_arg, precision_arg, f);
	}
	return formatstr.substr(start, index - start);
}


Format get_format(const std::string &fmt, size_t &index)
{
	char c = fmt[index++];
	Format f;
	if (c == 'a' || c == 'A' || c == 'c' || c == 'd' || c == 'i' || c == 'e' || c == 'E' || c == 'f' ||
		c == 'F' || c == 'g' || c == 'G' || c == 'o' || c == 's' || c == 'x' || c == 'X' || c == 'p' ||
		c == 'n' || c == 'u')
	{
		f = (Format)c;
		return f;
	}
	throw std::invalid_argument("Format: invalid format " + c);
}


// Check has this format exra arg for precision or width
bool has_arg(const std::string &fmt, size_t &index)
{
	if (fmt[index] == '*')
	{
		index++;
		return true;
	}
	while (isdigit(fmt[index]))
	{
		index++;
	}
	return false;
}


std::string read_format_with_args(const std::string &fmt, size_t &index, bool &width_arg, bool &precision_arg, Format &format)
{
	size_t start = index++, len = fmt.length();
	while (index < len && flags(fmt[index]))
	{
		index++;
	}
	if (width(fmt[index]))
	{
		width_arg = has_arg(fmt, index);
	}
	if (precision(fmt[index]))
	{
		precision_arg = has_arg(fmt, ++index);
	}
	specifier(fmt, index);
	format = get_format(fmt, index);
	return fmt.substr(start, index - start);
}


