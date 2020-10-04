#pragma once

#include <Windows.h>

#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <thread>
#include <random>

#undef max

// Any error here is because of Unicode. Use Multi-byte instead.

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace common {
				inline const char* known_size_ends[] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
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

			LONGLONG get_file_size(const std::string&);

			// FORMAT = "STARTSTUFF######ENDSTUFF" (# = number space)
			std::vector<std::string> generate_string_format(const std::string, const size_t, const size_t = 0);

			// gets Thread ID
			uint64_t get_thread_id();

			// value, how many bytes to test?
			std::vector<bool> translate_binary(const int, const size_t);

			// BASE10! auto conv bytes to string like 1000 -> 1 kB, with size itself and how many .numbers kB, bool is if you want "1.0 k" instead of "1.0k"
			std::string byte_auto_string(double, const size_t = 1, const bool = false);

			// gets std::string UTF8 and converts to wchar_t (UTF16) // https://stackoverflow.com/a/26914562
			std::wstring wideup(const std::string&);

			const double limit_maximize(double, const double = 1.0);

			// generates random number (based on custom_random class)
			const unsigned int random();

			// generates random string with numbers that shouldn't repeat until 115 days running! N means it is actually a big number, no alpha
			std::string generate_random_unique_string();

			// the string, the separator (like :, =, ...), comment characters and characters in the end aka endline // setting: value#comment
			std::vector<std::pair<std::string, std::string>> break_lines_config(const std::string, const std::string = ":= ", const std::string = "#", const std::string = "\r\n");

			template<size_t N> inline const size_t strlen_s(char(&str)[N]) { return strnlen_s(str, N); }

			// sprintf, but creates its memory and save them as std::string
			template<size_t len = 512> std::string sprintf_a(const char* format, ...) {
				std::string str;
				str.resize(len);

				va_list args;
				va_start(args, format);

				auto readd = vsprintf_s(str.data(), str.size(), format, args);

				va_end(args);

				str.resize(readd); // set what is there
				str.shrink_to_fit(); // clean up the rest of it (won't clear after it)
				return str;
			}

			/* * * * * * * * * * *
			 >>>> CAST TOOLS <<<<
			* * * * * * * * * * */

			// like autocast, but for type
			template<typename T> using r_cast_t = std::conditional_t<std::is_pointer<T>::value || std::is_array<T>::value, std::add_pointer_t<std::remove_all_extents_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>, std::remove_all_extents_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>;

			// regress (go back to main) cast
			template<typename Base, typename Cust = r_cast_t<Base>>
			inline Cust r_cast(Base b) noexcept { if (std::is_pointer_v<Base> || std::is_array_v<Base>) return s_cast<Cust*>(b); return s_cast<Cust>(b); }

		}
	}
}