#pragma once

#include <string>
#include <functional>
#include <physfs.h>

// Others
#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Handling\Path\path.h"
#include "..\..\Tools\Common\common.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			struct file_info {
				std::string path;
				int64_t size = 0;
				std::string size_str;
			};

			class PathManager {
				static bool physfs_initialized_once;

				std::vector<file_info> read_directory(const std::string& = "") const; // can call itself if folders in
			public:
				PathManager();
				~PathManager();

				// disable PathManager everywhere
				//void disable_all() const;

				// add new path
				void add_path(std::string);
				// remove this path
				void del_path(const std::string);

				// apply the paths interface to thread
				void apply() const;
				// cancel apply
				void unapply() const;

				// dir like cmd dir
				//void dir();

				// paths in interface
				std::vector<std::string> paths_set() const;
				// files available
				std::vector<file_info> files_in_paths() const;
			};

		}
	}
}