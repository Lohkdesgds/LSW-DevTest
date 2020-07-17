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
			C last_added_color = C::WHITE; /// THIS CHANGES WITH +=, clear(), pop() and stuff!
		public:
			coloured_string() = default;
			template<typename T> coloured_string(T& u) {
				clear();
				append(u);
			}

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

			coloured_string& append(const coloured_string&);
			template<size_t siz>
			coloured_string& append(const char(&oth)[siz]) {
				std::string aa = oth;
				return (this->operator+=(aa));
			}
			coloured_string& append(const std::string&);
			coloured_string& append(const char_c&);
			coloured_string& append(const float&);
			coloured_string& append(const double&);
			coloured_string& append(const char*);
			coloured_string& append(const char&);
			template<typename T>
			coloured_string& append(const T& u) {
				return this->append(std::to_string(u));
			}

			template<typename T> coloured_string& operator+=(const T& u) {
				return this->append(u);
			};
			template<typename T> coloured_string operator+(const T& u) {
				coloured_string a = *this;
				return a.append(u);
			};
			template<typename T> coloured_string& operator=(const T& u) {
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

			C __last_color() const;
			void __change_last_color(const C);
		};
	}
}