#pragma once


#include <Windows.h>
#include <functional>
#include <thread>
#include <string>


namespace LSW {
	namespace v5 {
		namespace Tools {

			class Launcher {
				HANDLE g_hChildStd_OUT_Rd = nullptr;
				HANDLE g_hChildStd_OUT_Wr = nullptr;
				SECURITY_ATTRIBUTES saAttr{};

				PROCESS_INFORMATION piProcInfo{}; // handle = piProcInfo.hProcess
#ifdef UNICODE
				STARTUPINFOA siStartInfo{};
#else
				STARTUPINFO siStartInfo{};
#endif

				char inter_buf[8];

				std::function<void(const std::string)> prunt = std::function<void(const std::string)>();
				std::thread* autosav = nullptr;
				bool keep = false;
				bool still_running = false;

				void keep_reading();
			public:
				~Launcher();

				void hook_output(const std::function<void(const std::string)>);
				bool launch(std::string);
				bool running();
				void kill();
			};

		}
	}
}