#pragma once

#include <windows.h>
#include <wininet.h>
#include <ShlObj.h>
#include <string>

#pragma comment (lib, "wininet.lib")

// as the Download does not need anyone else, it can be in its own namespace

namespace LSW {
	namespace v5 {
		namespace Download {

			constexpr auto download_max_block_size = 8192;

			class Download {
				std::string buf;
			public:
				bool get(const char*);
				std::string& read();
			};
		}
	}
}