#pragma once

#include <Windows.h>

#include <chrono>
#include <string>
#include <mutex>
#include <functional>

#include "..\..\Handling\Abort\abort.h" // fails calls throw
#include "..\..\Handling\Path\path.h" // handle_path used on init
#include "..\..\Tools\CString\cstring.h" // uses this as string internally to print colored chars


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace logger {
				constexpr size_t max_lines_stored_by_memlog = 10;
				constexpr size_t each_line_stored_by_memlog = 140;
				constexpr size_t len_class = 45;
				constexpr size_t default_max_buffer_internally = 256;
			}

			enum class L { EL, SL, SLF, ELF };
			enum class E { INFO, WARN, ERRR, DEBUG };

			// valid on next printf call only (valid to: int, float, double, long, long long, size_t)
			class FormatAs {
				std::string format;
			public:
				FormatAs() = default;
				FormatAs(std::string&&);
				FormatAs(FormatAs&);
				const std::string& getFormat();
				void clear();
				bool hasCustomFormat();
			};

			class Logger {
				struct __idata {
					Tools::cstring::C last_c = Tools::cstring::C::WHITE;
					bool bypass_last_c_once = false;

					FILE* fp = nullptr;
					std::string path = "log.log";
					std::mutex m;
					bool m_b = false;
					bool file_write_enabled = false;

					/*ALLEGRO_EVENT_SOURCE evsrc = ALLEGRO_EVENT_SOURCE();

					Cstring memline[logger::max_lines_stored_by_memlog];
					size_t memlinecount = 0;*/

					Tools::Cstring memline_s;
					std::function<void(Tools::Cstring)> sendd;
				};

				static __idata g;
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

				FormatAs latestFormat;

				std::string _generate_date() const;

				void set_console_color(const Tools::cstring::C);
				void print_start();

				void send_last();


				void print(const Tools::Cstring&);

				void fprint(FILE*&, const Tools::Cstring&);
			public:
				void init(const std::string); // path, once
				void flush();

				void debug(const std::string&, const E = E::INFO);

				void hook(std::function<void(Tools::Cstring)>);
				void unhook();

				Logger& operator<<(const L&);
				Logger& operator<<(FormatAs&&);
				Logger& operator<<(Tools::Cstring&&);
				Logger& operator<<(Tools::Cstring&);
				Logger& operator<<(const std::string&);
				Logger& operator<<(const char[]);
				Logger& operator<<(const char&);
				Logger& operator<<(const int&);
				Logger& operator<<(const float&);
				Logger& operator<<(const double&);
				Logger& operator<<(const unsigned&);
				Logger& operator<<(const long&);
				Logger& operator<<(const long long&);
				Logger& operator<<(const size_t&);
			};


			const std::string fsr(std::string, const E = E::INFO); // Function class, function name, type

		}
	}
}