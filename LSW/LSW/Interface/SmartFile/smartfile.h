#pragma once

#include <string>

// Others
#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Handling\Path\path.h"
#include "..\..\Tools\Common\common.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace smartfile {
				enum class file_modes { READ = 1, WRITE, READ_WRITE_KEEP, READ_WRITE_OVERWRITE, APPEND_WRITE, APPEND_READ_WRITE };
				enum class file_seek { CURRENT = ALLEGRO_SEEK_CUR, END = ALLEGRO_SEEK_END, BEGIN = ALLEGRO_SEEK_SET };
			}

			class SmartFile {
				ALLEGRO_FILE* fp = nullptr;
				long long size_of_this = 0;
				bool eoff = false;

				std::string convert(const smartfile::file_modes);
			public:
				SmartFile();
				~SmartFile();

				bool open(std::string, const smartfile::file_modes);
				bool is_open() const;
				long long total_size() const;
				void close();
				bool eof() const;

				void seek(const int, const smartfile::file_seek);

				size_t read(std::string&, const size_t);
				size_t write(const std::string&, const size_t = 0);
			};

		}
	}
}