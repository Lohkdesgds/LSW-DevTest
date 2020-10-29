#pragma once

#include <any>
#include <string>

namespace LSW {
	namespace v5 {
		namespace Tools {

			/// <summary>
			/// <para>Custom std::any that works with char arrays (transforming them to std::string)</para>
			/// <para>operator= and get template changed if T is char* or const char*</para>
			/// </summary>
			class Any : public std::any {
				std::string if_string_buf;
			public:
				template<typename... Args> Any(Args... args) : std::any(args...) {}

				template<typename T> T get() {
					return std::any_cast<T>(*this);
				}
				template<> char* get() {
					return const_cast<char*>((if_string_buf = std::any_cast<std::string>(*this)).c_str());
				}
				template<> const char* get() {
					return ((if_string_buf = std::any_cast<std::string>(*this)).c_str());
				}

				using std::any::operator=;

				Any& operator=(char* arr) {
					std::string str = arr;
					(*this) = std::move(str);
				}
				Any& operator=(const char* arr) {
					std::string str = arr;
					(*this) = std::move(str);
				}
			};

		}
	}
}