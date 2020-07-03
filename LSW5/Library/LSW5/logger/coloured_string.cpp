#include "coloured_string.h"

namespace LSW {
	namespace v5 {

		coloured_string::coloured_string(coloured_string & oth) {
			*this = oth;
		}
		coloured_string::coloured_string(const coloured_string & oth) {
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

		const size_t coloured_string::find(const char c) const
		{
			for (size_t p = 0; p < str.size(); p++) {
				if (str[p].ch == c) return p;
			}
			return static_cast<size_t>(-1);
		}

		const size_t coloured_string::find(const char_c c) const
		{
			for (size_t p = 0; p < str.size(); p++) {
				if (str[p] == c) return p;
			}
			return static_cast<size_t>(-1);
		}

		const size_t coloured_string::rfind(const char c) const
		{
			if (str.size()) {
				for (size_t p = str.size() - 1; p > 0; p--) {
					if (str[p].ch == c) return p;
				}
				if (str[0].ch == c) return 0;
			}
			return static_cast<size_t>(-1);
		}

		const size_t coloured_string::rfind(const char_c c) const
		{
			if (str.size()) {
				for (size_t p = str.size() - 1; p > 0; p--) {
					if (str[p] == c) return p;
				}
				if (str[0] == c) return 0;
			}
			return static_cast<size_t>(-1);
		}

		const size_t coloured_string::find(const std::string& s) const
		{
			if (const size_t s_len = s.length(); s_len) {
				
				if (s_len > str.size()) return static_cast<int>(-1); // no way to find				

				for (size_t p = 0; p < (str.size() - s_len) + 1; p++) { // if both have same size, run once
					if (str[p].ch == s[0]) {
						bool bad = false;
						for (size_t start = 1; start < s_len && (start + p) < str.size() && !bad; start++) { // just to be sure, if someday I change the code...
							bad |= str[p + start].ch != s[start];
						}
						if (!bad) return p;
					}
				}
			}
			return static_cast<size_t>(-1);
		}

		const size_t coloured_string::find(const coloured_string& s) const
		{
			if (const size_t s_len = s.size(); s_len) {

				if (s_len > str.size()) return static_cast<int>(-1); // no way to find				

				for (size_t p = 0; p < (str.size() - s_len) + 1; p++) { // if both have same size, run once
					if (str[p] == s[0]) {
						bool bad = false;
						for (size_t start = 1; start < s_len && (start + p) < str.size() && !bad; start++) { // just to be sure, if someday I change the code...
							bad |= str[p + start] != s[start];
						}
						if (!bad) return p;
					}
				}
			}
			return static_cast<size_t>(-1);
		}

		coloured_string coloured_string::substr(const size_t start, const size_t len)
		{
			coloured_string cpy;
			for (size_t p = 0; (p + start) < str.size() && p < len; p++) cpy += str[p + start];
			return cpy;
		}

		std::string coloured_string::s_str()
		{
			std::string cpy;
			for (auto& i : str) cpy += i.ch;
			return cpy;
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

		coloured_string& coloured_string::operator+=(const coloured_string & oth) {
			//str.clear();
			for(auto& i : oth) 
				str += i;
			
			return *this;
		}
		coloured_string& coloured_string::operator+=(const std::string& rstr) {
			//str.clear();
			if (!rstr.size()) return *this;
			C curr_color = C::WHITE, last_added_color = C::WHITE;
			bool ignore_once = false;
			bool percentage_c = false;
			bool has_backslash_to_add = false;

			for (auto& i : rstr) {
				has_backslash_to_add = false;
				
				if (i == '&' && !ignore_once) {
					percentage_c = true;
					continue;
				}

				if (i == '\\' && !ignore_once) {
					ignore_once = true;
					continue;
				}
				else {
					if (ignore_once && (i != '&' && i != '\\')) has_backslash_to_add = true;
					ignore_once = false;
				}

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

				if (has_backslash_to_add) str.push_back({ '\\', curr_color });
				str += { i, curr_color };
				last_added_color = curr_color;
			}

			//auto& cpyy = str.at(str.size() - 1);

			return *this;
		}

		coloured_string& coloured_string::operator+=(const char& a)
		{
			char arr[2] = { a, '\0' };
			return this->operator+=(std::string(arr));
		}
		coloured_string& coloured_string::operator+=(const char_c& a)
		{
			str.push_back(a);
			return *this;
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

		coloured_string& coloured_string::operator=(const coloured_string& a)
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

		coloured_string& coloured_string::operator=(const char_c& a)
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

		char_c& coloured_string::operator[](const size_t p) {
			return str[p];
		}
		const char_c& coloured_string::operator[](const size_t p) const{
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
		char_c* coloured_string::data()
		{
			return str.data();
		}
		const char_c* coloured_string::data() const
		{
			return str.data();
		}
	}
}