#pragma once

// C
#define _CRT_RAND_S
#include <stdio.h>
#include <stdlib.h>
#include <physfs.h>
// C++
#include <string>
#include <functional>
// Others
#include "..\tools\tools.h"
#include "..\allegroinitialization\allegroinitialization.h"


namespace LSW {
	namespace v5 {

		enum class file_modes {READ = 1, WRITE, READ_WRITE_KEEP, READ_WRITE_OVERWRITE, APPEND_WRITE, APPEND_READ_WRITE};

		class SmartFILE {
			FILE* fp = nullptr;
			int mode = 0;
			long long size_of_this = 0;
			bool eoff = false;

			std::string convert(const file_modes);
		public:
			~SmartFILE();

			bool open(std::string, const file_modes);
			long long totalSize();
			void close();
			bool eof();

			size_t read(std::string&, const size_t);
			size_t write(std::string&, const size_t = 0);
		};

		class PhysFS {
			static bool physfs_initialized_once;
			std::function<void(std::string)> prunt;

			std::string readDir(const char* = nullptr); // can call itself if folders in
		public:
			PhysFS();

			// disable PhysFS everywhere
			void disableALL();

			// add new path
			void addPath(std::string);
			// remove this path
			void delPath(const std::string);

			void hookPrint(const std::function<void(std::string)>);

			// apply the paths
			void apply();
			// cancel apply
			void cancel();

			// dir like cmd dir
			void dir();
		};
	}
}