#include "cstring.h"

namespace LSW {
	namespace v5 {
		namespace Tools {
			Cstring::Cstring(const Cstring& c)
			{
				*this = c;
			}
			Cstring::Cstring(Cstring&& m) noexcept
			{
				*this = std::move(m);
			}
			const size_t Cstring::find(const char c) const
			{
				for (size_t p = 0; p < str.size(); p++) {
					if (str[p].ch == c) return p;
				}
				return static_cast<size_t>(-1);
			}

			const size_t Cstring::find(const char_c c) const
			{
				for (size_t p = 0; p < str.size(); p++) {
					if (str[p] == c) return p;
				}
				return static_cast<size_t>(-1);
			}

			const size_t Cstring::rfind(const char c) const
			{
				if (str.size()) {
					for (size_t p = str.size() - 1; p > 0; p--) {
						if (str[p].ch == c) return p;
					}
					if (str[0].ch == c) return 0;
				}
				return static_cast<size_t>(-1);
			}

			const size_t Cstring::rfind(const char_c c) const
			{
				if (str.size()) {
					for (size_t p = str.size() - 1; p > 0; p--) {
						if (str[p] == c) return p;
					}
					if (str[0] == c) return 0;
				}
				return static_cast<size_t>(-1);
			}

			const size_t Cstring::find(const std::string& s) const
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

			const size_t Cstring::find(const Cstring& s) const
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

			Cstring Cstring::substr(const size_t start, const size_t len) const
			{
				Cstring cpy;

				for (size_t p = 0; (p + start) < str.size() && p < len; p++) cpy += str[p + start];

				if (start + len + 1 < str.size()) cpy.last_added_color = str[start + len + 1].cr;
				else cpy.last_added_color = last_added_color;

				return cpy;
			}

			std::string Cstring::s_str() const
			{
				std::string cpy;
				for (auto& i : str) cpy += i.ch;
				return cpy;
			}

			char_c Cstring::pop() {
				if (str.size() > 0) {
					char_c cpy = str.back();
					str.erase(str.end()-1);
					if (str.length() > 0) last_added_color = str.back().cr;
					return cpy;
				}
				else return char_c();
			}
			size_t Cstring::size() const {
				return str.size();
			}

			Cstring& Cstring::append(const Cstring& oth) {
				//str.clear();
				for (auto& i : oth) str += i;
				last_added_color = oth.last_added_color;

				return *this;
			}
			Cstring& Cstring::append(const std::string& rstr) {
				//str.clear();
				if (!rstr.size()) return *this;
				cstring::C curr_color = last_added_color;
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
							curr_color = static_cast<cstring::C>(k - '0');
							last_added_color = curr_color;
						}
						else if (k >= 'A' && k <= 'F') {
							curr_color = static_cast<cstring::C>(k - 'A' + 10);
							last_added_color = curr_color;
						}
						else {
							str.push_back({ '&', curr_color }); // wasn't &color, so + &
							str.push_back({ i, curr_color });
						}
						continue;
					}

					if (has_backslash_to_add) str.push_back({ '\\', curr_color });
					append({ i, curr_color });
				}

				//auto& cpyy = str.at(str.size() - 1);

				return *this;
			}

			Cstring& Cstring::append(const char_c& a)
			{
				str.push_back(a);
				last_added_color = str.back().cr;
				return *this;
			}
			Cstring& Cstring::append(const float& a)
			{
				char temp[32];
				if (fabs(a) < 1e15) sprintf_s(temp, "%.3f", a);
				else sprintf_s(temp, a > 0 ? "+++" : "---");
				return this->append(std::string(temp));
			}

			Cstring& Cstring::append(const double& a)
			{
				char temp[32];
				if (fabs(a) < 1e15) sprintf_s(temp, "%.5lf", a);
				else sprintf_s(temp, a > 0 ? "+++" : "---");
				return this->append(std::string(temp));
			}

			Cstring& Cstring::append(const char* a)
			{
				return this->append(std::string(a));
			}

			Cstring& Cstring::append(const char& a)
			{
				return this->append(std::string(&a, sizeof(char)));
			}

			Cstring& Cstring::append(const cstring::C& c)
			{
				last_added_color = c;
				return *this;
			}

			void Cstring::operator=(const Cstring& c)
			{
				str = c.str;
				last_added_color = c.last_added_color;
			}

			void Cstring::operator=(Cstring&& m) noexcept
			{
				str = std::move(m.str);
				last_added_color = std::move(last_added_color);
			}

			void Cstring::push_back(char_c&& u)
			{
				append(u);
			}

			std::basic_string<char_c>::iterator Cstring::begin()
			{
				return str.begin();
			}

			std::basic_string<char_c>::iterator Cstring::end()
			{
				return str.end();
			}

			std::basic_string<char_c>::const_iterator Cstring::begin() const
			{
				return str.begin();
			}

			std::basic_string<char_c>::const_iterator Cstring::end() const
			{
				return str.end();
			}

			char_c& Cstring::operator[](const size_t p) {
				if (p >= size()) throw Handling::Abort(__FUNCSIG__, "Out of range");
				return str[p];
			}
			const char_c& Cstring::operator[](const size_t p) const {
				if (p >= size()) throw Handling::Abort(__FUNCSIG__, "Out of range");
				return str[p];
			}
			const char_c& Cstring::front() const
			{
				if (size() == 0) throw Handling::Abort(__FUNCSIG__, "Empty string doesn't have front()!");
				return (*this)[0];
			}
			const char_c& Cstring::back() const
			{
				if (size() == 0) throw Handling::Abort(__FUNCSIG__, "Empty string doesn't have back()!");
				return (*this)[size()-1];
			}
			void Cstring::clear()
			{
				str.clear();
			}
			char_c* Cstring::data()
			{
				return str.data();
			}
			const char_c* Cstring::data() const
			{
				return str.data();
			}
			const cstring::C& Cstring::next_color() const
			{
				return last_added_color;
			}
			Cstring Cstring::filter_ascii_range(const char a, const char b) const
			{
				Cstring nstr;
				for (auto& i : str) {
					if (i.ch >= a && i.ch <= b) {
						nstr.append(i);
					}
				}
				nstr.last_added_color = last_added_color;
				return nstr;
			}

			Cstring operator+(const std::string& str, const Cstring& cstr)
			{
				Cstring conv = str;
				return conv + cstr;
			}
}
	}
}