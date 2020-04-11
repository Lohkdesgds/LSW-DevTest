#include "filesystem.h"

namespace LSW {
	namespace v5 {
		errno_t lsw_fopen(FILE*& fp, const char* path, const char* mode)
		{
			std::string npath = path;
			Tools::interpretPath(npath);
			return fopen_s(&fp, npath.c_str(), mode);
		}
		errno_t lsw_fopen_f(FILE*& fp, const char* path, const char* mode)
		{
			std::string npath = path;
			Tools::handlePath(npath);
			return fopen_s(&fp, npath.c_str(), mode);
		}
		void lsw_fread(FILE*& fp, std::string& buf, const size_t siz) {
			if (!fp) return;

			for (size_t s = 0; s < siz; s++) {
				char ubuf;
				if (!fread_s(&ubuf, 1, sizeof(char), 1, fp)) {
					return;
				}
				buf += ubuf;
			}
		}
		void lsw_fwrite(const std::string& buf, FILE*& fp) {
			if (!fp) return;
			for (size_t p = 0; p < buf.length(); p++) {
				if (!fwrite(&buf[0], sizeof(char), 1, fp)) {
					return;
				}
			}
		}
	}
}