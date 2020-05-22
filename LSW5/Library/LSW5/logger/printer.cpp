#include "printer.h"

namespace LSW {
	namespace v5 {

		void lsw_print(const coloured_string& cstr) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			for (auto& i : cstr) {
				int clr_c = static_cast<int>(i.cr);
				SetConsoleTextAttribute(hConsole, clr_c);
				putchar(i.ch);
			}
		}
		void lsw_print(const std::string& str) {
			for (auto& i : str) putchar(i);
		}
		void lsw_print(const char& ch) {
			putchar(ch);
		}
		void lsw_print(const int& i) {
			printf_s("%d", i);
		}
		void lsw_print(const float& i) {
			printf_s("%.3f", i);
		}
		void lsw_print(const double& i) {
			printf_s("%.5lf", i);
		}
		void lsw_print(const unsigned& i) {
			printf_s("%u", i);
		}
		void lsw_print(const long& i) {
			printf_s("%ld", i);
		}
		void lsw_print(const long long& i) {
			printf_s("%lld", i);
		}
		void lsw_print(const size_t& i) {
			printf_s("%zu", i);
		}


		void lsw_fprint(FILE*& fp, const coloured_string& cstr) {
			if (!fp) return;
			for (auto& i : cstr) {
				fputc(i.ch, fp);
			}
		}
		void lsw_fprint(FILE*& fp, const std::string& str) {
			if (!fp) return;
			fwrite(str.c_str(), sizeof(char), str.length(), fp);
		}
		void lsw_fprint(FILE*& fp, const char& ch) {
			if (!fp) return;
			fputc(ch, fp);
		}
		void lsw_fprint(FILE*& fp, const int& i) {
			if (!fp) return;
			fprintf_s(fp, "%d", i);
		}
		void lsw_fprint(FILE*& fp, const float& i) {
			if (!fp) return;
			fprintf_s(fp, "%.3f", i);
		}
		void lsw_fprint(FILE*& fp, const double& i) {
			if (!fp) return;
			fprintf_s(fp, "%.5lf", i);
		}
		void lsw_fprint(FILE*& fp, const unsigned& i) {
			if (!fp) return;
			fprintf_s(fp, "%u", i);
		}
		void lsw_fprint(FILE*& fp, const long& i) {
			if (!fp) return;
			fprintf_s(fp, "%ld", i);
		}
		void lsw_fprint(FILE*& fp, const long long& i) {
			if (!fp) return;
			fprintf_s(fp, "%lld", i);
		}
		void lsw_fprint(FILE*& fp, const size_t& i) {
			if (!fp) return;
			fprintf_s(fp, "%zu", i);
		}
	}
}