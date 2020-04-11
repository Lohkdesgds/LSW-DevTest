#pragma once

// C
#include <Windows.h>
#include <ShlObj.h>
// C++
#include <string>
#include <vector>
#include <sstream>

namespace LSW {
	namespace v5 {
		namespace Tools {

			inline const char* paths_known[] = { "%appdata%", "%win_fonts%", "%win_photos_path%" }; //CSIDL_APPDATA, CSIDL_FONTS, CSIDL_MYPICTURES

			void createPath(std::string);
			LONGLONG getFileSize(const std::string&);
			bool _getFolderCSIDL(std::string&, const int&);
			void _clearPath(std::string&, const bool = false);

			// clear path and interpret %path%
			void interpretPath(std::string&);

			// do all the necessary stuff (to open a file already) and interpret path
			void handlePath(std::string&); // old: interpretClearAndCreatePath

			// FORMAT = "STARTSTUFF######ENDSTUFF" (# = number space)
			std::vector<std::string> genStrFormat(const std::string, const size_t, const size_t = 0);

			// value, how many bytes to test?
			std::vector<bool> translBinary(const int, const size_t);

			// the string, the separator (like :, =, ...), comment characters and characters in the end aka endline // setting: value#comment
			std::vector<std::pair<std::string, std::string>> breakLines(const std::string, const std::string = ":= ", const std::string = "#", const std::string = "\r\n");
		}
	}
}
