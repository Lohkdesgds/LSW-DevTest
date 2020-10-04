#include "common.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			custom_random::custom_random()
			{
				seed = rd() ^ (
					static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) +
					static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())
					);
				gen = std::mt19937(seed);
			}

			unsigned custom_random::random()
			{
				return distrib(gen);
			}


			LONGLONG get_file_size(const std::string& s)
			{
#ifdef UNICODE
				HANDLE hFile = CreateFile(Tools::wideup(s).c_str(), GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);
#else
				HANDLE hFile = CreateFile(s.c_str(), GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);
#endif


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

			std::vector<std::string> generate_string_format(const std::string format, const size_t max, const size_t startat)
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
			uint64_t get_thread_id()
			{
				std::stringstream ss;
				ss << std::this_thread::get_id();
				return std::stoull(ss.str());
			}
			std::vector<bool> translate_binary(const int v, const size_t lim)
			{
				std::vector<bool> b;

				for (size_t actual_bin = 0; actual_bin < lim; actual_bin++)
				{
					b.push_back(v & (1 << actual_bin));
				}

				return b;
			}
			std::string byte_auto_string(double end, const size_t t, const bool space) {
				int prefix = -1;

				while ((end /= (1e3)) >= 1.0) prefix++;
				end *= 1e3;

				char buf[1 << 7];
				char format[1 << 5];

				if (space) sprintf_s(format, "%c.%zulf %cs", '%', t, '%');
				else sprintf_s(format, "%c.%zulf%cs", '%', t, '%');

				sprintf_s(buf, format, end, prefix >= 0 ? common::known_size_ends[prefix] : "");

				return buf;
			}
			std::wstring wideup(const std::string& str) {
				if (str.empty())
					return std::wstring();

				size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
					str.data(), (int)str.size(), NULL, 0);
				if (charsNeeded == 0) return L"Error";

				std::vector<wchar_t> buffer(charsNeeded + 1);
				int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
					str.data(), (int)str.size(), &buffer[0], static_cast<int>(buffer.size()));
				if (charsConverted == 0) return L"Error";

				return std::wstring(&buffer[0], charsConverted);
			}
			const double limit_maximize(double gotten, const double prop)
			{
				return sin(gotten * 3.1415) * prop;
			}
			const unsigned int random()
			{
				static custom_random _rand_class; // sorry
				return _rand_class.random();
			}
			std::string generate_random_unique_string()
			{
				auto cool_n = random() % 1000; // 3 digits max
				auto now = std::chrono::system_clock::now().time_since_epoch();
				unsigned now_c = now.count() % static_cast<int>(1e10); // 115 days to repeat. Will you play for 115 days? even then there's a random number. You'll have to try REALLY HARD
				char sign[32];
				sprintf_s(sign, "%010u%03u", now_c, cool_n);
				return sign;

			}
			std::vector<std::pair<std::string, std::string>> break_lines_config(const std::string a, const std::string spr, const std::string comment, const std::string endline)
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