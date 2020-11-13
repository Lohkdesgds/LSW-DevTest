#pragma once

namespace LSW {
	namespace v5 {
		namespace Tools {
			
			template<typename... Args>
			inline Any::Any(Args... args) : std::any(args...)
			{
				// nothing, yes
			}

			inline Any& Any::operator=(char* arr)
			{
				std::string str = arr;
				(*this) = std::move(str);
				return *this;
			}

			inline Any& Any::operator=(const char* arr)
			{
				std::string str = arr;
				(*this) = std::move(str);
				return *this;
			}

			template<typename T>
			inline T Any::get()
			{
				return std::any_cast<T>(*this);
			}
			
			template<>
			inline char* Any::get()
			{
				return const_cast<char*>((if_string_buf = std::any_cast<std::string>(*this)).c_str());
			}
			
			template<>
			inline const char* Any::get()
			{
				return ((if_string_buf = std::any_cast<std::string>(*this)).c_str());
			}
			
			template<typename T>
			inline T Any::get() const
			{
				return std::any_cast<T>(*this);
			}

		}
	}
}