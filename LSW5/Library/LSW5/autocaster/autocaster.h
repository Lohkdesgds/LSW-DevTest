#pragma once

/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
Autocaster header:

- Autocaster is a useful default cast method to cast stuff around.

= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
*/

#include <type_traits>

namespace LSW {
	namespace v5 {
		namespace Cast {

			// simple (static) cast
			template<typename K, typename T>
			inline K s_cast(T val) noexcept { return static_cast<K>(val); }

			// regress (go back to main) cast
			template<typename Base, typename Cust = std::remove_all_extents_t<std::remove_const_t<Base>>>
			inline Cust r_cast(Base b) noexcept { if (std::is_pointer_v<Base> || std::is_array_v<Base>) return s_cast<Cust*>(b); return s_cast<Cust>(b); }

			// force cast
			template<typename K, typename T>
			inline K f_cast(T val) noexcept { return (K)val; }

			// auto cast
			template <typename T>
			constexpr auto a_cast(T e) noexcept -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> { return static_cast<std::underlying_type_t<T>>(e); }


			template<typename T> using r_cast_t = std::remove_all_extents_t<std::remove_const_t<T>>;
		}
	}
}