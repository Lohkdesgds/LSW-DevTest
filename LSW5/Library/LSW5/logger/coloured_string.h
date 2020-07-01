#pragma once

// C
#define _CRT_RAND_S
// C++
#include <string>

namespace LSW {
	namespace v5 {

		enum class C { BLACK, DARK_BLUE, DARK_GREEN, DARK_AQUA, DARK_RED, DARK_PURPLE, GOLD, GRAY, DARK_GRAY, BLUE, GREEN, AQUA, RED, LIGHT_PURPLE, YELLOW, WHITE };

		struct char_c {
			char ch = '\0';
			C cr = C::WHITE;
			const bool operator==(const char_c o) const { return ch == o.ch && cr == o.cr; }
		};

		class coloured_string {
			std::basic_string<char_c> str;
		public:
			coloured_string() = default;
			coloured_string(coloured_string&);
			coloured_string(const coloured_string&);
			template<size_t siz> coloured_string(const char(&oth)[siz]) { *this = oth; }
			coloured_string(std::string&);

			coloured_string(const char&);
			coloured_string(const int&);
			coloured_string(const float&);
			coloured_string(const double&);
			coloured_string(const unsigned&);
			coloured_string(const long&);
			coloured_string(const long long&);
			coloured_string(const size_t&);

			const size_t find(const char) const;
			const size_t find(const char_c) const;
			const size_t rfind(const char) const;
			const size_t rfind(const char_c) const;

			const size_t find(const std::string&) const;
			const size_t find(const coloured_string&) const;
			// one day rfind, dunno


			coloured_string substr(const size_t, const size_t = static_cast<size_t>(-1));

			// clean STD::STRING with NO color information
			std::string s_str();

			char_c pop();
			size_t size() const;

			coloured_string& operator+=(const coloured_string&);
			template<size_t siz> coloured_string& operator+=(const char(&oth)[siz]) {
				std::string aa = oth;
				return (this->operator+=(aa));
			}
			coloured_string& operator+=(const std::string&);
			coloured_string& operator+=(const char&);
			coloured_string& operator+=(const char_c&);
			coloured_string& operator+=(const int&);
			coloured_string& operator+=(const float&);
			coloured_string& operator+=(const double&);
			coloured_string& operator+=(const unsigned&);
			coloured_string& operator+=(const long&);
			coloured_string& operator+=(const long long&);
			coloured_string& operator+=(const size_t&);

			template<typename T> coloured_string& operator+(const T& u) { return (*this += u); };

			coloured_string& operator=(const coloured_string&);
			template<size_t siz> coloured_string& operator=(const char(&oth)[siz]) {
				str.clear();
				std::string aa = oth;
				return (this->operator=(aa));
			}
			coloured_string& operator=(const std::string&);
			coloured_string& operator=(const char&);
			coloured_string& operator=(const char_c&);
			coloured_string& operator=(const int&);
			coloured_string& operator=(const float&);
			coloured_string& operator=(const double&);
			coloured_string& operator=(const unsigned&);
			coloured_string& operator=(const long&);
			coloured_string& operator=(const long long&);
			coloured_string& operator=(const size_t&);



			std::basic_string<char_c>::const_iterator begin() const;
			std::basic_string<char_c>::const_iterator end() const;
			char_c& operator[](const size_t);
			const char_c& operator[](const size_t) const;

			void push_back(char_c&&);
			void clear();

			char_c* data();
			const char_c* data() const;
		};
	}
}