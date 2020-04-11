#pragma once

// C
#include <stdio.h>
#include <stdlib.h>
// C++
#include <string>

// import
#include "..\tools\tools.h"


namespace LSW {
	namespace v5 {

		errno_t lsw_fopen(FILE*&, const char*, const char*);
		errno_t lsw_fopen_f(FILE*&, const char*, const char*);

		// "slower" but reliable fread
		void lsw_fread(FILE*&, std::string&, const size_t);
		// "slower" but reliable fwrite
		void lsw_fwrite(const std::string&, FILE*&);
	}
}