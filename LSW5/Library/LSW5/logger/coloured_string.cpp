#include "coloured_string.h"

namespace LSW {
	namespace v5 {

		coloured_string::coloured_string(coloured_string & oth) {
			*this = oth;
		}
		coloured_string::coloured_string(std::string & oth) {
			*this = oth;
		}

		coloured_string::coloured_string(const char& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const int& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const float& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const double& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const unsigned& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const long& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const long long& o)
		{
			*this = o;
		}

		coloured_string::coloured_string(const size_t& o)
		{
			*this = o;
		}

		char_c coloured_string::pop() {
			if (str.size() > 0) {
				char_c cpy = str.front();
				str.erase(str.begin());
				return cpy;
			}
			else return char_c();
		}
		size_t coloured_string::size() const {
			return str.size();
		}

		coloured_string& coloured_string::operator+=(coloured_string & oth) {
			str.clear();
			while (oth.size()) {
				str += oth.pop();
			}
			return *this;
		}
		coloured_string& coloured_string::operator+=(const std::string& rstr) {
			str.clear();
			C curr_color = C::WHITE, last_added_color = C::WHITE;
			bool ignore_once = false;
			bool percentage_c = false;

			for (auto& i : rstr) {
				if (i == '&' && !ignore_once) {
					percentage_c = true;
					continue;
				}

				if (i == '\\' && !ignore_once) {
					ignore_once = true;
					continue;
				}
				else ignore_once = false;

				if (percentage_c) { // &color (0-9,a-f)
					percentage_c = false;
					char k = toupper(i);
					if (k >= '0' && k <= '9') {
						curr_color = static_cast<C>(k - '0');
					}
					else if (k >= 'A' && k <= 'F') {
						curr_color = static_cast<C>(k - 'A' + 10);
					}
					else {
						str.push_back({ '&', curr_color }); // wasn't &color, so + &
						str.push_back({ i, curr_color });
					}
					continue;
				}
				str.push_back({ i, curr_color });
				last_added_color = curr_color;
			}

			return *this;
		}

		coloured_string& coloured_string::operator+=(const char& a)
		{
			char arr[2] = { a, '\0' };
			return this->operator+=(std::string(arr));
		}

		coloured_string& coloured_string::operator+=(const int& a)
		{
			return this->operator+=(std::to_string(a));
		}

		coloured_string& coloured_string::operator+=(const float& a)
		{
			char temp[32];
			if (fabs(a) < 1e15) sprintf_s(temp, "%.3f", a);
			else sprintf_s(temp, a > 0 ? "+++" : "---");
			return this->operator+=(std::string(temp));
		}

		coloured_string& coloured_string::operator+=(const double& a)
		{
			char temp[32];
			if (fabs(a) < 1e15) sprintf_s(temp, "%.5lf", a);
			else sprintf_s(temp, a > 0 ? "+++" : "---");
			return this->operator+=(std::string(temp));
		}

		coloured_string& coloured_string::operator+=(const unsigned& a)
		{
			return this->operator+=(std::to_string(a));
		}

		coloured_string& coloured_string::operator+=(const long& a)
		{
			return this->operator+=(std::to_string(a));
		}

		coloured_string& coloured_string::operator+=(const long long& a)
		{
			return this->operator+=(std::to_string(a));
		}


		coloured_string& coloured_string::operator+=(const size_t& a)
		{
			return this->operator+=(std::to_string(a));
		}

		coloured_string& coloured_string::operator=(coloured_string& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const std::string& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const char& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const int& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const float& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const double& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const unsigned& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const long& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const long long& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		coloured_string& coloured_string::operator=(const size_t& a)
		{
			str.clear();
			return this->operator+=(a);
		}

		std::basic_string<char_c>::const_iterator coloured_string::begin() const {
			return str.begin();
		}
		std::basic_string<char_c>::const_iterator coloured_string::end() const {
			return str.end();
		}

		char_c& coloured_string::operator[](size_t p) {
			return str[p];
		}
		void coloured_string::push_back(char_c&& c)
		{
			str += c;
		}
		void coloured_string::clear()
		{
			str.clear();
		}
	}
}