#pragma once

#include <Windows.h>
#include <ShlObj.h>

#include <string>
#include <vector>
#include <sstream>

namespace LSW {
	namespace v5 {
		namespace Handling {

			namespace path {
				inline const char* paths_known[] = { "%appdata%", "%fonts%", "%mypictures%", "%mydocuments%", "%mymusic%", "%myvideo%", "%desktop%", "%localdata%" };
				enum class paths_known_e {
					APPDATA,
					FONTS,
					PICTURES,
					DOCUMENTS,
					MUSIC,
					VIDEO,
					DESKTOP,
					LOCAL_APPDATA
				};
				const std::pair<paths_known_e, int> paths_pairs[] = {
					{ paths_known_e::APPDATA, CSIDL_APPDATA },
					{ paths_known_e::FONTS, CSIDL_FONTS },
					{ paths_known_e::PICTURES, CSIDL_MYPICTURES },
					{ paths_known_e::DOCUMENTS, CSIDL_MYDOCUMENTS },
					{ paths_known_e::MUSIC, CSIDL_MYMUSIC },
					{ paths_known_e::VIDEO, CSIDL_MYVIDEO },
					{ paths_known_e::DESKTOP, CSIDL_DESKTOPDIRECTORY },
					{ paths_known_e::LOCAL_APPDATA, CSIDL_LOCAL_APPDATA }
				};
			}

			// Creates the path (if needed)

			// Clears path and interpret %placeholders%
			/**
			 * Creates the path automatically (use \\)
			 * It will ignore files (filename.extension format). If you want literally as path, do like "C:\\folder\\other.folder.with.dots\\"
			 * @param {std::string} the path to be created (can be relative or full. It's recommended to do full or use %paths%)
			 */
			void create_path(const std::string&);

			/**
			 * Gets folder path based on string (one of those path::paths_known).
			 * @param {std::string&} sets this if path was found.
			 * @param {const char*} the %path% string.
			 * @returns true if successful.
			 */
			bool get_folder_csidl(std::string&, const char*);
			/**
			 * Gets folder path based on enum path::paths_known_e.
			 * @param {std::string&} sets this if path was found.
			 * @param {path::paths_known_e&} the path code.
			 * @returns true if successful.
			 */
			bool get_folder_csidl(std::string&, const path::paths_known_e&);
			/**
			 * Gets folder path based on direct CSIDL value (if valid, see path::paths_known_e).
			 * @param {std::string&} sets this if path was found.
			 * @param {int&} the CSIDL code.
			 * @returns true if successful.
			 */
			bool get_folder_csidl(std::string&, const int&);

			/**
			 * Interprets path (ignores stuff like %appdata\%, allows \%appdata% because that can be a path)
			 * + filters path with / (to \\ instead)
			 * @param {std::string&} the input and output, like "%appdata%\\cool_path_i_made\\file.exe"
			 */
			void interpret_path(std::string&);

			// Creates the path and interpret %placeholders%
			/**
			 * Interprets/solves %placeholders% (interpret_path) and guarantees path (create_path)
			 * @param {std::string&} the path you want to create and interpret (and save interpretation)
			 */
			void handle_path(std::string&);
		}
	}
}