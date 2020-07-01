#include "tools.h"

namespace LSW {
	namespace v5 {

		namespace Tools {
			void createPath(std::string s)
			{
				std::string str;
				std::stringstream ss(s);
				std::string token;
				std::vector<std::string> paths;

				while (std::getline(ss, token, '\\'))
				{
					str += token;
					paths.push_back(str);
					str += '\\';
				}

				std::string u = paths.back();
				paths.pop_back();
				if (u.rfind(".") == std::string::npos) paths.push_back(u);

				for (auto& i : paths)
				{
					CreateDirectoryA(i.c_str(), NULL);
				}
			}
			LONGLONG getFileSize(const std::string& s)
			{
				HANDLE hFile = CreateFile(s.c_str(), GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
					return -1; // error condition, could call GetLastError to find out more

				LARGE_INTEGER size;
				if (!GetFileSizeEx(hFile, &size))
				{
					CloseHandle(hFile);
					return -1; // error condition, could call GetLastError to find out more
				}

				CloseHandle(hFile);
				return size.QuadPart;
			}
			bool _getFolderCSIDL(std::string& s, const int& u)
			{
				wchar_t Folder[1024];
				HRESULT hr = SHGetFolderPathW(0, u, 0, 0, Folder);
				if (SUCCEEDED(hr))
				{
					char str[1024] = { 0 };
					size_t i;
					wcstombs_s(&i, str, Folder, 1023);
					s = str;
					return true;
				}
				return false;
			}
			void _clearPath(std::string& s, const bool inverted) // true = '/'
			{
				std::string str;

				for (auto& i : s) {
					if (!inverted && (i == '/')) str += '\\';
					else if (inverted && (i == '\\')) str += '/';
					else str += i;
				}

				s = str;
				str.clear();
			}

			void interpretPath(std::string& local_t)
			{
				_clearPath(local_t);

				for (size_t p = local_t.find('%'); (p != std::string::npos); p = local_t.find('%'))
				{
					if (p == 0 || (local_t[p - 1] != '\\')) {
						std::string elsee = local_t.substr(p);
						std::string path_to_add;
						std::string change;

						size_t found = 0;
						bool rfound = false;
						for (auto& i : paths_known)
						{
							if (elsee.find(i) == 0)
							{
								rfound = true;
								break;
							}
							found++;
						}

						if (!rfound) {
							if (p > 0)
							{
								change = local_t.substr(0, p - 1);
							}
							change += "\\";
							change += local_t.substr(p);
							local_t = change;

							continue;
						}

						size_t nxtpos = strlen(paths_known[found]);

						switch (found)
						{
						case 0:
							_getFolderCSIDL(path_to_add, CSIDL_APPDATA);
							break;
						case 1:
							_getFolderCSIDL(path_to_add, CSIDL_FONTS);
							break;
						case 2:
							_getFolderCSIDL(path_to_add, CSIDL_MYPICTURES);
							break;
						}

						if (p > 0) change = local_t.substr(0, p);

						std::string end_path = local_t.substr(nxtpos + p, std::string::npos);
						change += path_to_add + end_path;
						local_t = change;
					}
				}
			}

			void handlePath(std::string& path)
			{
				interpretPath(path);
				createPath(path);
			}

			std::vector<std::string> genStrFormat(const std::string format, const size_t max, const size_t startat)
			{
				std::vector<std::string> rett;

				std::string start;
				size_t amountofzeros = 1;
				std::string end;

				short step = 0; // start, zeros, end

				// split
				for (auto& i : format)
				{
					switch (step) {
					case 0:
						if (i != '#') {
							start += i;
						}
						else {
							step++; // skip one amountofzeros
						}
						break;
					case 1:
						if (i == '#') {
							amountofzeros++;
						}
						else {
							step++;
							end += i;
						}
						break;
					case 2:
						end += i;
						break;
					}
				}

				std::string realformat = "%s%0" + std::to_string(amountofzeros) + "zu%s";

				char buff[512];

				for (size_t c = startat; c < max; c++) {
					sprintf_s(buff, realformat.c_str(), start.c_str(), c, end.c_str());
					rett.push_back(buff);
				}

				return rett;
			}
			uint64_t getThreadID()
			{
				std::stringstream ss;
				ss << std::this_thread::get_id();
				return std::stoull(ss.str());
			}
			/*int hexTransform(const char c)
			{
				auto cpy = std::toupper(c);
				if (cpy >= '0' && cpy <= '9') return cpy - '0';
				if (cpy >= 'A' && cpy <= 'F') return 10 + (cpy - 'A');
				throw Abort::Abort(__FUNCSIG__, "Out of range HEX value transformation!", Abort::abort_level::WARN);
				
			}*/
			std::vector<bool> translBinary(const int v, const size_t lim)
			{
				std::vector<bool> b;

				for (size_t actual_bin = 0; actual_bin < lim; actual_bin++)
				{
					b.push_back(v & (1 << actual_bin));
				}

				return b;
			}
			std::string byteAutoString(double end, const size_t t, const bool space) {
				int prefix = -1;

				while ((end /= (1e3)) >= 1.0) prefix++;
				end *= 1e3;

				char buf[1 << 7];
				char format[1 << 5];

				if (space) sprintf_s(format, "%c.%zulf %cs", '%', t, '%');
				else sprintf_s(format, "%c.%zulf%cs", '%', t, '%');

				sprintf_s(buf, format, end, prefix >= 0 ? known_size_ends[prefix] : "");

				return buf;
			}
			std::wstring wideUp(const std::string& str) {
				// Deprecated ;-;
				/*std::wstring_convert<std::codecvt_utf8 <wchar_t>, wchar_t> convert;
				return convert.from_bytes(s);*/

				if (str.empty())
					return std::wstring();

				size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
					str.data(), (int)str.size(), NULL, 0);
				if (charsNeeded == 0) throw Abort::Abort(__FUNCSIG__, "Failed to convert to wstring!", Abort::abort_level::GIVEUP);

				std::vector<wchar_t> buffer(charsNeeded + 1);
				int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
					str.data(), (int)str.size(), &buffer[0], static_cast<int>(buffer.size()));
				if (charsConverted == 0) throw Abort::Abort(__FUNCSIG__, "Failed to convert to wstring!", Abort::abort_level::GIVEUP);

				return std::wstring(&buffer[0], charsConverted);
			}
			const double maxone(double gotten, const double prop)
			{
				/*while (gotten > 1.0) {
					for(double u = 1e96; u > 10.0; u /= 1e4) if (gotten > u) gotten -= u;
					if (gotten > 1.0) gotten = 2.0 - gotten;
					else gotten -= 1.0;
				}*/
				return sin(gotten * 3.1415) * prop;
			}
			const unsigned int rand()
			{
				unsigned int r = 0;
				rand_s(&r);
				return r;
			}
			std::string generateRandomUniqueStringN()
			{
				auto cool_n = Tools::rand() % 1000; // 3 digits max
				auto now = MILLI_NOW;
				unsigned now_c = now.count() % static_cast<int>(1e10); // 115 days to repeat. Will you play for 115 days? even then there's a random number. You'll have to try REALLY HARD
				char sign[32];
				sprintf_s(sign, "%010u%03u", now_c, cool_n);
				return sign;

			}
			std::vector<std::pair<std::string, std::string>> breakLines(const std::string a, const std::string spr, const std::string comment, const std::string endline)
			{
				std::vector<std::pair<std::string, std::string>> vectu;
				std::pair<std::string, std::string> pair;
				bool flipswitch = false;
				bool ignore_now = false;

				for (size_t ii = 0; ii < a.size(); ii++)
				{
					if (endline.find(a[ii]) != std::string::npos) {
						if (pair.first.length() || pair.second.length()) vectu.push_back(pair);
						pair = std::pair<std::string, std::string>();
						flipswitch = false;
						ignore_now = false;
						continue;
					}
					if (comment.find(a[ii]) != std::string::npos) {
						ignore_now = true;
					}

					if (!ignore_now) {
						if (spr.find(a[ii]) != std::string::npos && !flipswitch) {
							while (ii < a.size() && spr.find(a[++ii]) != std::string::npos);
							ii--;
							flipswitch = true;
							continue;
						}

						if (!flipswitch) {
							pair.first += a[ii];
						}
						else {
							pair.second += a[ii];
						}
					}
				}

				if (pair.first.length() || pair.second.length()) {
					vectu.push_back(pair);
				}

				return vectu;
			}
		}
	}
}