#include "format.h"

using namespace std;

string read_format(const string &fmt, size_t &index, bool &widthdth_arg, bool &precisionecision_arg,
	Format &f) {
	size_t len = fmt.length(), start = index;
	char c;
	for (index; index < len; index++) {
		c = fmt[index];
		if (c != '%') {
			continue;
		}
		else if (index != start) {
			return fmt.substr(start, index - start);
		}
		if (index + 1 < len && fmt[index + 1] == '%') {
			index += 2;
			return "%";
		}
		return read_format_widthth_args(fmt, index, widthdth_arg, precisionecision_arg, f);
	}
	return fmt.substr(start, index - start);
}

bool flag(char c) {
	return c == '-' || c == '+' || c == ' ' || c == '#' || c == '0';
}

bool width(char c) {
	return c == '*' || isdigit(c);
}

bool precision(char c) {
	return c == '.';
}

void check_specifier(const string &fmt, size_t &index) {
	string s;
	s.push_back(fmt[index]);
	if (index + 1 < fmt.length() && fmt[index + 1] == fmt[index]) {
		s.push_back(fmt[index + 1]);
	}
	if (s == "l" || s == "ll" || s == "h" || s == "hh"
		|| s == "j" || s == "z" || s == "t" || s == "L") {
		index += s.length();
	}
}

Format get_format(const string &fmt, size_t &index) {
	char c = fmt[index++];
	Format f;
	if (c == 'a' || c == 'A' || c == 'c' || c == 'd' || c == 'i' || c == 'e' || c == 'E' || c == 'f' ||
		c == 'F' || c == 'g' || c == 'G' || c == 'o' || c == 's' || c == 'x' || c == 'X' || c == 'p' ||
		c == 'n' || c == 'u') {
		f = (Format)c;
		return f;
	}
	throw invalid_argument("Format: invalid format " + c);
}
// Check has this format exra arg for precisionecision or widthdth
bool has_arg(const string &fmt, size_t &index) {
	if (fmt[index] == '*') {
		index++;
		return true;
	}
	while (isdigit(fmt[index])) {
		index++;
	}
	return false;
}


string read_format_widthth_args(const string &fmt, size_t &index, bool &widthdth_arg, bool &precisionecision_arg,
	Format &format) {
	size_t start = index++, len = fmt.length();
	while (index < len && flag(fmt[index])) {
		index++;
	}
	if (width(fmt[index])) {
		widthdth_arg = has_arg(fmt, index);
	}
	if (precision(fmt[index])) {
		precisionecision_arg = has_arg(fmt, ++index);
	}
	check_specifier(fmt, index);
	format = get_format(fmt, index);
	return fmt.substr(start, index - start);
}


string replace_format(const string &fmt, int &wp) {
	size_t first = fmt.find_first_of('*');
	string result = fmt.substr(0, first) + to_string(wp) + fmt.substr(first + 1, fmt.length());
	return result;
}
