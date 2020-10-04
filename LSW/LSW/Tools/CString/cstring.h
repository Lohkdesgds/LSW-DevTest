#pragma once

#include <string>

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace cstring {
				enum class C { BLACK, DARK_BLUE, DARK_GREEN, DARK_AQUA, DARK_RED, DARK_PURPLE, GOLD, GRAY, DARK_GRAY, BLUE, GREEN, AQUA, RED, LIGHT_PURPLE, YELLOW, WHITE };
			}

			struct char_c {
				char ch = '\0';
				cstring::C cr = cstring::C::WHITE;
				const bool operator==(const char_c o) const { return ch == o.ch && cr == o.cr; }
			};

			class Cstring {
				std::basic_string<char_c> str;
				cstring::C last_added_color = cstring::C::WHITE; /// THIS CHANGES WITH +=, clear(), pop() and stuff!
			public:
				Cstring() = default;
				template<typename T> Cstring(T& u) {
					clear();
					append(u);
				}

				const size_t find(const char) const;
				const size_t find(const char_c) const;
				const size_t rfind(const char) const;
				const size_t rfind(const char_c) const;

				const size_t find(const std::string&) const;
				const size_t find(const Cstring&) const;

				// one day rfind, dunno


				Cstring substr(const size_t, const size_t = static_cast<size_t>(-1)) const;

				// clean STD::STRING with NO color information
				std::string s_str() const;

				char_c pop();
				size_t size() const;

				Cstring& append(const Cstring&);
				template<size_t siz>
				Cstring& append(const char(&oth)[siz]) {
					std::string aa = oth;
					return (this->operator+=(aa));
				}
				Cstring& append(const std::string&);
				Cstring& append(const char_c&);
				Cstring& append(const float&);
				Cstring& append(const double&);
				Cstring& append(const char*);
				Cstring& append(const char&);
				template<typename T>
				Cstring& append(const T& u) {
					return this->append(std::to_string(u));
				}

				template<typename T> Cstring& operator+=(const T& u) {
					return this->append(u);
				};
				template<typename T> Cstring operator+(const T& u) {
					Cstring a = *this;
					return a.append(u);
				};
				template<typename T> Cstring& operator=(const T& u) {
					clear();
					return append(u);
				};
				void push_back(char_c&&);


				std::basic_string<char_c>::const_iterator begin() const;
				std::basic_string<char_c>::const_iterator end() const;

				char_c& operator[](const size_t);
				const char_c& operator[](const size_t) const;

				void clear();

				char_c* data();
				const char_c* data() const;

				cstring::C __last_color() const;
				void __change_last_color(const cstring::C);
			};
		}
	}
}