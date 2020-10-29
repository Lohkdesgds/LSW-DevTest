#pragma once

#include <string>

#include "..\..\Handling\Abort\abort.h"

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

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{Cstring}">Cstring to copy.</param>
				Cstring(const Cstring&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{Cstring}">Cstring to move.</param>
				Cstring(Cstring&&) noexcept;

				/// <summary>
				/// <para>Constructor copy.</para>
				/// </summary>
				/// <param name="{T}">Copies if possible.</param>
				template<typename T> Cstring(const T& u) {
					clear();
					append(u);
				}

				/// <summary>
				/// <para>Search for a char.</para>
				/// </summary>
				/// <param name="{char}">Char to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t find(const char) const;

				/// <summary>
				/// <para>Search for a c_char.</para>
				/// </summary>
				/// <param name="{char}">C_char to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t find(const char_c) const;

				/// <summary>
				/// <para>Search for a char backwards.</para>
				/// </summary>
				/// <param name="{char}">Char to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t rfind(const char) const;

				/// <summary>
				/// <para>Search for a c_char backwards.</para>
				/// </summary>
				/// <param name="{char}">C_char to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t rfind(const char_c) const;

				/// <summary>
				/// <para>Search for a string.</para>
				/// </summary>
				/// <param name="{char}">String to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t find(const std::string&) const;

				/// <summary>
				/// <para>Search for a Cstring.</para>
				/// </summary>
				/// <param name="{char}">Cstring to find.</param>
				/// <returns>{size_t} Position or -1 if not found.</returns>
				const size_t find(const Cstring&) const;



				/// <summary>
				/// <para>Trim the Cstring.</para>
				/// </summary>
				/// <param name="{size_t}">Position.</param>
				/// <param name="{size_t}">Length.</param>
				/// <returns>{Cstring} Returns the trimmed string.</returns>
				Cstring substr(const size_t, const size_t = static_cast<size_t>(-1)) const;

				/// <summary>
				/// <para>Transforms this Cstring to a simple string.</para>
				/// </summary>
				/// <returns>{std::string} The string.</returns>
				std::string s_str() const;


				/// <summary>
				/// <para>Pops out the back.</para>
				/// </summary>
				/// <returns>{char_c} The char_c popped out.</returns>
				char_c pop();

				/// <summary>
				/// <para>The size of the string.</para>
				/// </summary>
				/// <returns>{size_t} The string size.</returns>
				size_t size() const;


				/// <summary>
				/// <para>Appends strings.</para>
				/// </summary>
				/// <param name="{Cstring}">The string to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const Cstring&);

				/// <summary>
				/// <para>Appends char string.</para>
				/// </summary>
				/// <param name="{char[]}">The string to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				template<size_t siz>
				Cstring& append(const char(&oth)[siz]) {
					std::string aa = oth;
					return (this->operator+=(aa));
				}

				/// <summary>
				/// <para>Appends string.</para>
				/// </summary>
				/// <param name="{std::string}">The string to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const std::string&);

				/// <summary>
				/// <para>Appends a char_c.</para>
				/// </summary>
				/// <param name="{char_c}">The char_c to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const char_c&);

				/// <summary>
				/// <para>Appends float.</para>
				/// </summary>
				/// <param name="{float}">The float to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const float&);

				/// <summary>
				/// <para>Appends double.</para>
				/// </summary>
				/// <param name="{double}">The double to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const double&);

				/// <summary>
				/// <para>Appends char* string.</para>
				/// </summary>
				/// <param name="{char*}">The char* string to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const char*);

				/// <summary>
				/// <para>Appends char.</para>
				/// </summary>
				/// <param name="{char}">The char to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const char&);

				/// <summary>
				/// <para>Sets last color as this one (the ones added later will have this color).</para>
				/// </summary>
				/// <param name="{cstring::C}">The color.</param>
				/// <returns>{Cstring} Itself.</returns>
				Cstring& append(const cstring::C&);

				/// <summary>
				/// <para>Appends a type T string.</para>
				/// </summary>
				/// <param name="{T}">The T to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				template<typename T>
				Cstring& append(const T& u) {
					return this->append(std::to_string(u));
				}

				/// <summary>
				/// <para>Appends a type T.</para>
				/// </summary>
				/// <param name="{T}">The T to append.</param>
				/// <returns>{Cstring} Itself.</returns>
				template<typename T> Cstring& operator+=(const T& u) {
					return this->append(u);
				};

				/// <summary>
				/// <para>Appends a type T.</para>
				/// </summary>
				/// <param name="{T}">The T to append.</param>
				/// <returns>{Cstring} Result.</returns>
				template<typename T> Cstring operator+(const T& u) {
					Cstring a = *this;
					return a.append(u);
				};

				/// <summary>
				/// <para>Clears and set T as value.</para>
				/// </summary>
				/// <param name="{T}">The T to set.</param>
				template<typename T> void operator=(const T& u) {
					clear();
					append(u);
				};

				/// <summary>
				/// <para>Copy operator.</para>
				/// </summary>
				/// <param name="{Cstring}">The Cstring to copy from.</param>
				void operator=(const Cstring&);

				/// <summary>
				/// <para>Move operator.</para>
				/// </summary>
				/// <param name="{Cstring}">The Cstring to move from.</param>
				void operator=(Cstring&&) noexcept;

				/// <summary>
				/// <para>Appends a char_c.</para>
				/// </summary>
				/// <param name="{char_c}">The char_c to append.</param>
				void push_back(char_c&&);


				/// <summary>
				/// <para>Begin iterator.</para>
				/// </summary>
				/// <returns>{iterator} Begin iterator.</returns>
				std::basic_string<char_c>::const_iterator begin() const;

				/// <summary>
				/// <para>End iterator.</para>
				/// </summary>
				/// <returns>{iterator} End iterator.</returns>
				std::basic_string<char_c>::const_iterator end() const;


				/// <summary>
				/// <para>Gets a specific char_c.</para>
				/// </summary>
				/// <param name="{size_t}">Position.</param>
				/// <returns>{char_c} The value in that position.</returns>
				char_c& operator[](const size_t);

				/// <summary>
				/// <para>Gets a specific char_c.</para>
				/// </summary>
				/// <param name="{size_t}">Position.</param>
				/// <returns>{char_c} The value in that position.</returns>
				const char_c& operator[](const size_t) const;

				/// <summary>
				/// <para>Gets the first character.</para>
				/// </summary>
				/// <returns>{char_c} The character.</returns>
				const char_c& front() const;

				/// <summary>
				/// <para>Gets the last character.</para>
				/// </summary>
				/// <returns>{char_c} The character.</returns>
				const char_c& back() const;


				/// <summary>
				/// <para>Clears the memory.</para>
				/// </summary>
				void clear();

				/// <summary>
				/// <para>Gets the raw pointer.</para>
				/// </summary>
				/// <returns>{char_c*} The data pointer.</returns>
				char_c* data();

				/// <summary>
				/// <para>Gets the raw pointer.</para>
				/// </summary>
				/// <returns>{char_c*} The data pointer.</returns>
				const char_c* data() const;

				/// <summary>
				/// <para>What color will a simple string be if you append to this?</para>
				/// </summary>
				/// <returns>{cstring::C} The last color.</returns>
				const cstring::C& next_color() const;

				/// <summary>
				/// <para>Filters the string by range.</para>
				/// </summary>
				/// <param name="{char}">Lower value (more or equal to).</param>
				/// <param name="{char}">Higher value (less or equal to).</param>
				/// <returns>{Cstring} The filtered string.</returns>
				Cstring filter_ascii_range(const char = 32, const char = 126) const;
			};
		}
	}
}