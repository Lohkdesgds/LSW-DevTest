#pragma once


/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
Printer header:

- Printer can print almost everything
# WORKING since 20200410

= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
*/



// C
#include <Windows.h>
#include <stdio.h>
// C++
#include <string>

// import
#include "coloured_string.h"

namespace LSW {
	namespace v5 {	

		void lsw_print(const coloured_string&);
		void lsw_print(const std::string&);		
		void lsw_print(const char&);
		void lsw_print(const int&);
		void lsw_print(const float&);
		void lsw_print(const double&);
		void lsw_print(const unsigned&);
		void lsw_print(const long&);
		void lsw_print(const long long&);
		void lsw_print(const size_t&);

		template<size_t size> inline void lsw_print(const char(&str)[size]) {
			for (auto& i : str) putchar(i);
		}

	}
}