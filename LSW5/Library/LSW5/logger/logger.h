#pragma once

#include <allegro5/allegro.h>

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mutex>

// import
#include "..\Abort\abort.h"
#include "coloured_string.h"
#include "printer.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\tools\tools.h"


namespace LSW {
	namespace v5 {
		namespace logger {
			constexpr size_t max_lines_stored_by_memlog = 10;
			constexpr size_t each_line_stored_by_memlog = 140;
			constexpr size_t len_class = 45;
			constexpr auto CUSTOM_EVENT_LOG_STRING = 512;
		}

		enum class L { EL, SL, SLF, ELF };
		enum class E { INFO, WARN, ERRR, DEBUG };

		class Logger {
			struct __idata {
				C last_c = C::WHITE;
				bool bypass_last_c_once = false;

				FILE* fp = nullptr;
				std::string path = "log.log";
				std::mutex m;
				bool m_b = false;
				bool file_write_enabled = true;

				ALLEGRO_EVENT_SOURCE evsrc = ALLEGRO_EVENT_SOURCE();

				coloured_string memline[logger::max_lines_stored_by_memlog];
				size_t memlinecount = 0;
			};

			static __idata g;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			void setConsoleColor(const C);
			void printClock();

			void sendLast();
		public:
			void init(const std::string); // path, once
			void flush();

			ALLEGRO_EVENT_SOURCE* getEvent();

			Logger& operator<<(const L&);
			template<typename T> Logger& operator<<(T var) {
				coloured_string clstr;
				clstr = var;
				lsw_print(clstr);
				g.memline[g.memlinecount] += var; // vai dar erro
				return *this;
			}
		};


		const std::string fsr(std::string fname_pretty, const E situation = E::INFO); // Function class, function name, type




	}
}