#include "logger.h"

namespace LSW {
	namespace v5 {
		Logger::__idata Logger::g;

		void Logger::setConsoleColor(const C cl)
		{
			if (cl != g.last_c || g.bypass_last_c_once) {
				g.bypass_last_c_once = false;
				int clr_c = static_cast<int>(cl);
				SetConsoleTextAttribute(hConsole, clr_c);
				g.last_c = cl;
			}
		}

		void Logger::printClock()
		{
			coloured_string str;
			char temp[26];
			SYSTEMTIME t;
			GetLocalTime(&t);
			auto ss = sprintf_s(temp, "[%02d/%02d/%02d-%02d:%02d:%02d]", t.wYear % 100, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			for (size_t pp = 0; pp < ss; pp++) {
				str.push_back({ temp[pp], C::GOLD });
			}

			lsw_print(str);
		}

		void Logger::sendLast()
		{
			ALLEGRO_EVENT evv;
			evv.user.data1 = (intptr_t)&g.memline[g.memlinecount];
			evv.type = static_cast<int>(Shared::my_events::CUSTOM_EVENT_LOG_STRING);
			al_emit_user_event(&g.evsrc, &evv, nullptr);

			if (++g.memlinecount >= logger::max_lines_stored_by_memlog) g.memlinecount = 0;
			g.memline[g.memlinecount].clear();
		}
		
		void Logger::init(const std::string path)
		{
			g.m.lock();
			g.path = path;

			Tools::handlePath(g.path);

			if (!g.fp) {
				auto err = fopen_s(&g.fp, g.path.c_str(), "wb");
				if (err) {
					g.m.unlock();
					throw Abort::Abort(__FUNCSIG__, "Failed to open log: '" + path + "'", Abort::abort_level::GIVEUP);
					return;
				}

				lsw_al_init();
				al_init_user_event_source(&g.evsrc);
			}

			g.m.unlock();
		}
		void Logger::flush()
		{
			if (g.fp) {
				g.m.lock();
				fflush(g.fp);
				g.m.unlock();
			}
		}
		ALLEGRO_EVENT_SOURCE* Logger::getEvent()
		{
			if (g.fp) return &g.evsrc;
			return nullptr;
		}
		/*Logger& Logger::operator<<(const C& c)
		{
			g.last_c = c;
			g.bypass_last_c_once = true;
			return *this;
		}
		Logger& Logger::operator<<(const coloured_string& str)
		{
			lsw_print(str);
			return *this;
		}
		Logger& Logger::operator<<(const std::string& rstr)
		{
			coloured_string conv;
			C curr_color = g.last_c, last_added_color = g.last_c;
			bool ignore_once = false;
			bool percentage_c = false;

			for (auto& i : rstr) {
				if (i == '&' && !ignore_once) {
					percentage_c = true;
					continue;
				}

				if (i == '\\' && !ignore_once) {
					ignore_once = true;
					continue;
				}
				else ignore_once = false;

				if (percentage_c) { // &color (0-9,a-f)
					percentage_c = false;
					char k = toupper(i);
					if (k >= '0' && k <= '9') {
						curr_color = static_cast<C>(k - '0');
					}
					else if (k >= 'A' && k <= 'F') {
						curr_color = static_cast<C>(k - 'A' + 10);
					}
					else {
						conv.push_back({ '&', curr_color }); // wasn't &color, so + &
						conv.push_back({ i, curr_color });
					}
					continue;
				}
				conv.push_back({ i, curr_color });
				last_added_color = curr_color;
			}

			lsw_print(conv);
			if (last_added_color != curr_color) {
				setConsoleColor(curr_color);
			}
			return *this;
		}*/
		Logger& Logger::operator<<(const L& u)
		{
			switch (u)
			{
			case L::SL: // START LINE
				g.m.lock();
				g.m_b = true;
				printClock();
				break;
			case L::EL: // END LINE
				*this << ('\n');
				if (!g.m_b) {
					*this << ("\n---------- FATAL WARN ----------\nMUTEX WAS NOT LOCKED! THE CODE HAS ERRORS!");
					flush();
					throw Abort::Abort(__FUNCSIG__, "FATAL ERROR MUTEX SHOULDN'T BE UNLOCKED IF IT WASN'T PREVIOUSLY!", Abort::abort_level::FATAL_ERROR);
				}
				g.m_b = false;
				g.m.unlock();	/// yes, visual studio thinks this is an epic WARN, but it will never fail if you use gfile << L::SL(F) << fsr(__FUNCSIG__) << ... << L::EL(F)
				sendLast(); // send last coloured string to event
				break;
			case L::SLF: // START LINE AND SAVE ON FILE
				g.m.lock();
				g.m_b = true;
				g.file_write_enabled = true;
				printClock();
				break;
			case L::ELF: // END LINE AND SAVE ON FILE
				*this << ('\n');
				g.file_write_enabled = false;
				if (!g.m_b) {
					*this << ("\n---------- FATAL WARN ----------\nMUTEX WAS NOT LOCKED! THE CODE HAS ERRORS!");
					flush();
					throw Abort::Abort(__FUNCSIG__, "FATAL ERROR MUTEX SHOULDN'T BE UNLOCKED IF IT WASN'T PREVIOUSLY!", Abort::abort_level::FATAL_ERROR);
				}
				g.file_write_enabled = false;
				g.m.unlock();	/// yes, visual studio thinks this is an epic WARN, but it will never fail if you use gfile << L::SL(F) << fsr(__FUNCSIG__) << ... << L::EL(F)
				flush();
				sendLast(); // send last coloured string to event
				break;
			}
			return *this;
		}

		Logger& Logger::operator<<(coloured_string&& clstr)
		{
			lsw_print(clstr);
			lsw_fprint(g.fp, clstr);
			if (clstr.size()) g.last_c = clstr[clstr.size() - 1].cr;
			g.memline[g.memlinecount] += clstr;
			return *this;
		}
		Logger& Logger::operator<<(coloured_string& clstr)
		{
			lsw_print(clstr);
			lsw_fprint(g.fp, clstr);
			if (clstr.size()) {
				g.last_c = clstr[clstr.size() - 1].cr;
			}
			g.memline[g.memlinecount] += clstr;
			return *this;
		}

		const std::string fsr(std::string fname_pretty, const E situation)
		{
			if (fname_pretty.length() < logger::len_class) {
				for (size_t p = fname_pretty.length(); p < logger::len_class; p++)
				{
					fname_pretty += '_';
				}
			}
			else {
				for (size_t p = fname_pretty.length(); p > logger::len_class; p--)
				{
					fname_pretty.pop_back();
				}
			}

			for (auto& i : fname_pretty) i = ::toupper(i);

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			std::string back_str;

			switch (situation) {
			case E::INFO:
				back_str = "&7[INFO]";
				break;
			case E::WARN:
				back_str = "&c[WARN]";
				break;
			case E::ERRR:
				back_str = "&4[ERRR]";
				break;
			case E::DEBUG:
				back_str = "&5[DEBG]";
				break;
			}

			back_str += "&8[" + fname_pretty + "]&f ";

			return back_str;
		}
		Logger& Logger::operator<<(const std::string& o) // don't forget template lmao
		{
			coloured_string cstr;
			char format[3];
			sprintf_s(format, "&%x", Cast::s_cast<int>(g.last_c));
			cstr = (format + o).c_str();
			return (this->operator<<(cstr));
		}
		Logger& Logger::operator<<(const char& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const int& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const float& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const double& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const unsigned& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const long& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const long long& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
		Logger& Logger::operator<<(const size_t& o)
		{
			return (this->operator<<(coloured_string(o)));
		}
}
}