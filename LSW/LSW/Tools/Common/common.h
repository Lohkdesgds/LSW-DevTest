#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <thread>
#include <random>
#include <stdarg.h>

#include "../../Handling/Abort/abort.h"

#undef max

// Any error here is because of Unicode. Use Multi-byte instead.

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace common {
				constexpr size_t known_size_len = 8;
				inline const char* known_size_ends[known_size_len] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
			}

			class custom_random {
				std::random_device rd;
				std::mt19937::result_type seed;
				std::mt19937 gen;
				std::uniform_int_distribution<unsigned> distrib;
			public:
				custom_random();

				unsigned random();
			};

			/// <summary>
			/// <para>Generate strings with a simple format.</para>
			/// </summary>
			/// <param name="{std::string}">The string format (#### are replaced with the numbers) e.g. START###END.</param>
			/// <param name="{size_t}">Until what?</param>
			/// <param name="{size_t}">Starts with what number?</param>
			/// <returns>{std::vector} Generated strings</returns>
			std::vector<std::string> generate_string_format(const std::string, const size_t, const size_t = 0);

			/// <summary>
			/// <para>Gets the thread ID.</para>
			/// <returns>{uint64_t} The thread ID.</returns>
			// gets Thread ID
			uint64_t get_thread_id();

			/// <summary>
			/// <para>Transform bits into a vector of bools (up to 32 bits).</para>
			/// <para>You can transform a char to 8 bits (bools).</para>
			/// </summary>
			/// <param name="{int} The value itself."></param>
			/// <param name="{size_t} How many bits?"></param>
			/// <returns>{std::vector} The vector of bits.</returns>
			std::vector<bool> translate_binary(const int, const size_t);

			/// <summary>
			/// <para>Transforms a double to a string formatted like "1.42k". BASE 10.</para>
			/// <para>Magnitudes: k, M, G, T, P, E, Z and Y.</para>
			/// </summary>
			/// <param name="{double}">The value.</param>
			/// <param name="{size_t}">Numbers after the dot.</param>
			/// <param name="{bool}">Do you prefer 1.3k or 1.3 k? (the space between the number and magnetude)</param>
			/// <returns>{std::string} The string generated.</returns>
			std::string byte_auto_string(double, const size_t = 1, const bool = false);

			// https://stackoverflow.com/a/26914562
			/// <summary>
			/// <para>Transforms a UTF8 string to a UTF16.</para>
			/// </summary>
			/// <param name="{std::string}">UTF8 string.</param>
			/// <returns>{std::wstring} UTF16 string</returns>
			//std::wstring wideup(const std::string&);

			/// <summary>
			/// <para>Limits the value using sin().</para>
			/// </summary>
			/// <param name="{double}">Your value to limit.</param>
			/// <param name="{double}">Multiplier (range, [-multiplier, multiplier]).</param>
			/// <returns>{double} The value after conversion.</returns>
			const double limit_maximize(double, const double = 1.0);

			/// <summary>
			/// <para>Generates a random number.</para>
			/// </summary>
			/// <returns>{unsigned} A random number.</returns>
			const unsigned int random();

			/// <summary>
			/// <para>Generates a random number-based string based on time and RNG.</para>
			/// <para>It should not repeat in 115 days. You won't play the game for so long, right?</para>
			/// </summary>
			/// <returns>{std::string} A random generated string.</returns>
			std::string generate_random_unique_string();

			/// <summary>
			/// <para>Breaks a config-like data.</para>
			/// <para>Configurations normally looks like KEY:VALUE#COMMENT (w/ breakline, default).</para>
			/// </summary>
			/// <param name="{std::string}">The input itself.</param>
			/// <param name="{std::string}">Separator tag (like := ).</param>
			/// <param name="{std::string}">Comment tag (like #).</param>
			/// <param name="{std::string}">Division between configurations tag (like literally breakline).</param>
			/// <returns>{std::vector} Vector of pairs (key, value) (comments are ignored).</returns>
			std::vector<std::pair<std::string, std::string>> break_lines_config(const std::string, const std::string = ":= ", const std::string = "#", const std::string = "\r\n");

			/// <summary>
			/// <para>Direct strnlen_s, automatic limit.</para>
			/// </summary>
			/// <param name="{char[]}">The string (char array)</param>
			/// <returns>{size_t} Length of the string.</returns>
			template<size_t N> inline const size_t strlen_s(char(&str)[N]) { return strnlen_s(str, N); }


			/// <summary>
			/// <para>Works like a printf_s, but the output is std::string.</para>
			/// </summary>
			/// <param name="{const char*}">The format.</param>
			/// <param name="{...}">The arguments.</param>
			/// <returns>{std::string} Returns the string generated.</returns>
			std::string sprintf_a(const char*, ...);

			/* * * * * * * * * * *
			 >>>> CAST TOOLS <<<<
			* * * * * * * * * * */

			// like autocast, but for type
			template<typename T> using r_cast_t = std::conditional_t<std::is_pointer<T>::value || std::is_array<T>::value, std::add_pointer_t<std::remove_all_extents_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>, std::remove_all_extents_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>;

			// regress (go back to main) cast
			/*template<typename Base, typename Cust = r_cast_t<Base>>
			inline Cust r_cast(Base b) noexcept { if (std::is_pointer_v<Base> || std::is_array_v<Base>) return s_cast<Cust*>(b); return s_cast<Cust>(b); }*/

		}
	}
}