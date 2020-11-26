#pragma once

#include <string>

// Others
#include "../../Handling/Initialize/initialize.h"
#include "../../Handling/Path/path.h"
#include "../../Tools/Common/common.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace smartfile {
				enum class file_modes { READ = 1, WRITE, READ_WRITE_KEEP, READ_WRITE_OVERWRITE, APPEND_WRITE, APPEND_READ_WRITE };
				enum class file_seek { CURRENT = ALLEGRO_SEEK_CUR, END = ALLEGRO_SEEK_END, BEGIN = ALLEGRO_SEEK_SET };
			}

			/// <summary>
			/// <para>SmartFile is a smart file way to read and write.</para>
			/// </summary>
			class SmartFile {
				ALLEGRO_FILE* fp = nullptr;
				//long long size_of_this = 0;
				bool eoff = false;

				std::string convert(const smartfile::file_modes);
			public:
				SmartFile();
				~SmartFile();

				/// <summary>
				/// <para>Open a file.</para>
				/// </summary>
				/// <param name="{std::string}">Path (allows %tags%, read-only).</param>
				/// <param name="{file_modes}">File opening mode.</param>
				/// <returns>{bool} True if successfully opened.</returns>
				bool open(std::string, const smartfile::file_modes);

				/// <summary>
				/// <para>Have you open()ed once?</para>
				/// </summary>
				/// <returns>{bool} True if open.</returns>
				bool is_open() const;

				/// <summary>
				/// <para>Tells file size right now.</para>
				/// </summary>
				/// <returns>{int64_t} Size.</returns>
				int64_t total_size() const;

				/// <summary>
				/// <para>Close file (if any).</para>
				/// </summary>
				void close();

				/// <summary>
				/// <para>End of file?</para>
				/// </summary>
				/// <returns>{bool} True if eof.</returns>
				bool eof() const;

				/// <summary>
				/// <para>Seek file from a point.</para>
				/// </summary>
				/// <param name="{int64_t}">Offset.</param>
				/// <param name="{file_seek}">From where?</param>
				void seek(const int64_t, const smartfile::file_seek);

				/// <summary>
				/// <para>Read some bytes from file.</para>
				/// </summary>
				/// <param name="{std::string}">Buffer to write.</param>
				/// <param name="{size_t}">Bytes.</param>
				/// <returns>{size_t} Bytes read.</returns>
				size_t read(std::string&, const size_t);

				/// <summary>
				/// <para>Read until reaches a character (included in copy) (slower).</para>
				/// </summary>
				/// <param name="{std::string}">Buffer to write.</param>
				/// <param name="{char}">The stop character.</param>
				/// <returns>{size_t} Bytes read.</returns>
				size_t read_until(std::string&, const char);

				/// <summary>
				/// <para>Writes some data to the file.</para>
				/// </summary>
				/// <param name="{std::string}">Data.</param>
				/// <param name="{size_t}">Max bytes (0 = all).</param>
				/// <returns>{size_t} Bytes written.</returns>
				size_t write(const std::string&, const size_t = 0);
			};

			/// <summary>
			/// <para>Quickly open, get size and close a file (interprets %tags%, readonly).</para>
			/// </summary>
			/// <param name="{std::string}">Path.</param>
			/// <returns></returns>
			int64_t quick_get_file_size(std::string);

		}
	}
}