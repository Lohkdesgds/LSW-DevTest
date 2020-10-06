#include "logger.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			Logger::__idata Logger::g;



			FormatAs::FormatAs(std::string&& form)
			{
				if (form.find('%') == std::string::npos) throw Handling::Abort(__FUNCSIG__, "Invalid FORMAT! You have to use something like %d or %05.3lf as on a printf!");
				format = form;
			}

			FormatAs::FormatAs(FormatAs& fa)
			{
				format = fa.get_format();
			}

			const std::string& FormatAs::get_format()
			{
				return format;
			}

			void FormatAs::clear()
			{
				format.clear();
			}

			bool FormatAs::has_custom_format()
			{
				return !format.empty();
			}

			std::string Logger::_generate_date() const
			{
				char temp[26];

				time_t rawtime;
				tm ti;

				time(&rawtime);
				localtime_s(&ti, &rawtime);
				//ti = localtime(&rawtime);

				sprintf_s(temp, "[%02d/%02d/%02d-%02d:%02d:%02d]", (ti.tm_year + 1900) % 100, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec);

				return temp;
			}

			void Logger::set_console_color(const Tools::cstring::C cl)
			{
				if (cl != g.last_c || g.bypass_last_c_once) {
					g.bypass_last_c_once = false;
					int clr_c = static_cast<int>(cl);
					SetConsoleTextAttribute(hConsole, clr_c);
					g.last_c = cl;
				}
			}

			void Logger::print_start()
			{
				Tools::Cstring str;
				/*char temp[26];

				time_t rawtime;
				tm ti;

				time(&rawtime);
				localtime_s(&ti, &rawtime);
				//ti = localtime(&rawtime);

				sprintf_s(temp, "[%02d/%02d/%02d-%02d:%02d:%02d]", (ti.tm_year + 1900) % 100, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec);*/

				/*SYSTEMTIME t;
				GetLocalTime(&t);
				auto ss = sprintf_s(temp, "[%02d/%02d/%02d-%02d:%02d:%02d]", t.wYear % 100, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);*/
				if (!g.fp && g.file_write_enabled) {
					str.append("&4[&eFILEERRR&4]");
				}
				str.__change_last_color(Tools::cstring::C::GOLD);
				str.append(_generate_date());
				/*for (size_t pp = 0; pp < ss; pp++) {
					str.push_back({ temp[pp], cstring::C::GOLD });
				}*/
				print(str);
			}

			void Logger::send_last()
			{
				if (g.sendd) g.sendd(g.memline_s);
				g.memline_s.clear();
			}
			void Logger::print(const Tools::Cstring& cstr) {
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				int last_clr_c = -1;
				for (auto& i : cstr) {
					int clr_c = static_cast<int>(i.cr);
					if (last_clr_c != clr_c) {
						SetConsoleTextAttribute(hConsole, clr_c);
						last_clr_c = clr_c;
					}
					putchar(i.ch);
				}
			}
			void Logger::fprint(FILE*& fp, const Tools::Cstring& cstr) {
				if (!fp) return;
				for (auto& i : cstr) {
					fputc(i.ch, fp);
				}
			}

			void Logger::init(const std::string path)
			{
				g.m.lock();
				g.path = path;

				Handling::handle_path(g.path);

				if (!g.fp) {
					auto err = fopen_s(&g.fp, g.path.c_str(), "wb");
					if (err) {
						g.m.unlock();
						throw Handling::Abort(__FUNCSIG__, "Failed to open log: '" + path + "'", Handling::abort::abort_level::GIVEUP);
						return;
					}
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
			void Logger::debug(const std::string& str, E situation)
			{
				// https://docs.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-outputdebugstringw
				// "Applications should send very minimal debug output and provide a way for the user to enable or disable its use." -> it disables the feature if in release mode then.
#ifdef _DEBUG
				std::string back_str;
				switch (situation) {
				case E::INFO:
					back_str = "[INFO]";
					break;
				case E::WARN:
					back_str = "[WARN]";
					break;
				case E::ERRR:
					back_str = "[ERRR]";
					break;
				case E::DEBUG:
					back_str = "[DEBG]";
					break;
				}
				g.dbgm.lock();
				OutputDebugString(("@\tLSWv5\t>\t" + back_str + _generate_date() + "\t\t\t\t\t\t  " + str + "\n").c_str());
				g.dbgm.unlock();
#endif
			}
			void Logger::hook(std::function<void(Tools::Cstring)> f)
			{
				g.sendd = f;
			}
			void Logger::unhook()
			{
				hook(std::function<void(Tools::Cstring)>());
			}
			Logger& Logger::operator<<(const L& u)
			{
				switch (u)
				{
				case L::SL: // START LINE
					g.m.lock();
					g.m_b = true;
					print_start();
					break;
				case L::EL: // END LINE
					*this << ('\n');
					if (!g.m_b) {
						*this << ("\n---------- FATAL WARN ----------\nMUTEX WAS NOT LOCKED! THE CODE HAS ERRORS!");
						flush();
						throw Handling::Abort(__FUNCSIG__, "FATAL ERROR MUTEX SHOULDN'T BE UNLOCKED IF IT WASN'T PREVIOUSLY!", Handling::abort::abort_level::FATAL_ERROR);
					}
					g.m_b = false;
					g.m.unlock();	/// yes, visual studio thinks this is an epic WARN, but it will never fail if you use gfile << L::SL(F) << fsr(__FUNCSIG__) << ... << L::EL(F)
					send_last(); // send last coloured string to event
					break;
				case L::SLF: // START LINE AND SAVE ON FILE
					g.m.lock();
					g.m_b = true;
					g.file_write_enabled = true;
					print_start();
					break;
				case L::ELF: // END LINE AND SAVE ON FILE
					*this << ('\n');
					if (!g.m_b) {
						*this << ("\n---------- FATAL WARN ----------\nMUTEX WAS NOT LOCKED! THE CODE HAS ERRORS!");
						flush();
						throw Handling::Abort(__FUNCSIG__, "FATAL ERROR MUTEX SHOULDN'T BE UNLOCKED IF IT WASN'T PREVIOUSLY!", Handling::abort::abort_level::FATAL_ERROR);
					}
					g.file_write_enabled = false;
					g.m.unlock();	/// yes, visual studio thinks this is an epic WARN, but it will never fail if you use gfile << L::SL(F) << fsr(__FUNCSIG__) << ... << L::EL(F)
					flush();
					send_last(); // send last coloured string to event
					break;
				}
				return *this;
			}

			Logger& Logger::operator<<(FormatAs&& nfa)
			{
				latestFormat = nfa;
				return *this;
			}

			Logger& Logger::operator<<(Tools::Cstring&& clstr)
			{
				print(clstr);
				if (g.file_write_enabled) fprint(g.fp, clstr);
				if (clstr.size()) {
					//g.last_c = clstr[clstr.size() - 1].cr;
					g.last_c = clstr.__last_color();
				}
				g.memline_s += clstr;
				return *this;
			}
			Logger& Logger::operator<<(Tools::Cstring& clstr)
			{
				print(clstr);
				if (g.file_write_enabled) fprint(g.fp, clstr);
				if (clstr.size()) {
					//g.last_c = clstr[clstr.size() - 1].cr;
					g.last_c = clstr.__last_color();
				}
				g.memline_s += clstr;
				return *this;
			}
			Logger& Logger::operator<<(const std::string& o) // don't forget template lmao
			{
				Tools::Cstring cstr;
				char format[3];
				sprintf_s(format, "&%x", static_cast<int>(g.last_c));
				cstr = (format + o).c_str();
				return (this->operator<<(cstr));
			}
			Logger& Logger::operator<<(const char o[])
			{
				return (this->operator<<(std::string(o)));
			}
			Logger& Logger::operator<<(const char& o)
			{
				return (this->operator<<(std::string(&o, 1)));
			}
			Logger& Logger::operator<<(const int& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%d", o)));
					sprintf_s(buf.data(), buf.size(), "%d", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const float& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%.3f", o)));
					sprintf_s(buf.data(), buf.size(), "%.3f", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const double& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%.5lf", o)));
					sprintf_s(buf.data(), buf.size(), "%.5lf", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const unsigned& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%u", o)));
					sprintf_s(buf.data(), buf.size(), "%u", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const long& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%ld", o)));
					sprintf_s(buf.data(), buf.size(), "%ld", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const long long& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%lld", o)));
					sprintf_s(buf.data(), buf.size(), "%lld", o);
				}

				return (this->operator<<(buf));
			}
			Logger& Logger::operator<<(const size_t& o)
			{
				std::string buf;

				if (latestFormat.has_custom_format()) {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, latestFormat.get_format().c_str(), o)));
					sprintf_s(buf.data(), buf.size(), latestFormat.get_format().c_str(), o);
					latestFormat.clear();
				}
				else {
					buf.resize(1 + static_cast<size_t>(snprintf(nullptr, 0, "%zu", o)));
					sprintf_s(buf.data(), buf.size(), "%zu", o);
				}

				return (this->operator<<(buf));
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
					back_str = "&2[INFO]";
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
		}
	}
}