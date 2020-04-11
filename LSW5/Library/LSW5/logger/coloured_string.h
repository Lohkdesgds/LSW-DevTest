#pragma once


/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
ColouredString header:

- Coloured string is a string with color

= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
*/



// C++
#include <string>

namespace LSW {
	namespace v5 {

		enum class C { BLACK, DARK_BLUE, DARK_GREEN, DARK_AQUA, DARK_RED, DARK_PURPLE, GOLD, GRAY, DARK_GRAY, BLUE, GREEN, AQUA, RED, LIGHT_PURPLE, YELLOW, WHITE };

		struct char_c { char ch = '\0'; C cr = C::WHITE; };

		class coloured_string {
			std::basic_string<char_c> str;
		public:
			coloured_string() = default;
			coloured_string(coloured_string&);
			template<size_t siz> coloured_string(const char(&oth)[siz]) { *this = oth; }
			coloured_string(std::string&);

			char_c pop();
			size_t size() const;

			coloured_string& operator+=(coloured_string&);
			template<size_t siz> coloured_string& operator+=(const char(&oth)[siz]) {
				std::string aa = oth;
				return (this->operator=(aa));
			}
			coloured_string& operator+=(std::string);
			coloured_string& operator+=(const char&);
			coloured_string& operator+=(const int&);
			coloured_string& operator+=(const float&);
			coloured_string& operator+=(const double&);
			coloured_string& operator+=(const unsigned&);
			coloured_string& operator+=(const long&);
			coloured_string& operator+=(const long long&);
			coloured_string& operator+=(const size_t&);

			template<typename T> coloured_string& operator=(T&& a) { str.clear(); this->operator+=(a); return *this; }



			std::basic_string<char_c>::const_iterator begin() const;
			std::basic_string<char_c>::const_iterator end() const;
			char_c& operator[](size_t);

			void push_back(char_c&&);
			void clear();
		};
	}
}