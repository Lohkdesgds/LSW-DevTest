#include "download.h"

namespace LSW {
	namespace v5 {
		namespace Download {
			bool Download::get(const char* url)
			{
				HINTERNET connect = InternetOpen("LSW Downloader V5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
				if (!connect) return false;
				HINTERNET OpenAddress = InternetOpenUrlA(connect, url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
				if (!OpenAddress) return false;

				char DataReceived[download_max_block_size];
				DWORD NumberOfBytesRead = 0;

				buf.clear();

				while (InternetReadFile(OpenAddress, DataReceived, download_max_block_size, &NumberOfBytesRead) && NumberOfBytesRead) {
					for (DWORD a = 0; a < NumberOfBytesRead; a++)
					{
						buf += DataReceived[a];
					}
				}

				InternetCloseHandle(OpenAddress);
				InternetCloseHandle(connect);
				return true;
			}
			std::string& Download::read()
			{
				return buf;
			}

		}
	}
}
