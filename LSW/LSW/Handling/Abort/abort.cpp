#include "abort.h"

namespace LSW {
	namespace v5 {
		namespace Handling {


			Abort::Abort(const std::string& fw, const std::string& dt, const abort::abort_level lv)
				: std::exception(("From: " + s_from_where + ";\nDetails: " + s_details + ";\nLevel: " + std::to_string(static_cast<int>(abort_level))).c_str(), static_cast<int>(abort_level))
			{
				s_from_where = fw;
				s_details = dt;
				abort_level = lv;
			}
			Abort::Abort(const Abort& abor)
				: std::exception(("From: " + abor.s_from_where + ";\nDetails: " + abor.s_details + ";\nLevel: " + std::to_string(static_cast<int>(abor.abort_level))).c_str(), static_cast<int>(abor.abort_level))
			{
				s_from_where = abor.s_from_where;
				s_details = abor.s_details;
				abort_level = abor.abort_level;
			}
			const std::string& Abort::get_from() const
			{
				return s_from_where;
			}
			const std::string& Abort::get_details() const
			{
				return s_details;
			}
			const abort::abort_level& Abort::get_level() const
			{
				return abort_level;
			}

			Abort::operator std::string() const
			{
				std::string lvl;
				switch (get_level()) {
				case abort::abort_level::FATAL_ERROR:
					lvl = "FATAL_ERROR";
					break;
				case abort::abort_level::GIVEUP:
					lvl = "GIVEUP";
					break;
				case abort::abort_level::OTHER:
					lvl = "OTHER";
					break;
				}
				return "From: " + get_from() + ";\nDetails: " + get_details() + ";\nLevel: " + lvl;
			}




			AutoAbort::AutoAbort(const autoabort::abort_mode mode, const size_t timeout_raw, std::function<void(void)> f) {
				std::chrono::milliseconds timeout = std::chrono::milliseconds(timeout_raw);
				abort_cancel = new bool(false);
				if (!abort_cancel) throw Abort::Abort(__FUNCSIG__, "Cannot alloc memory!");

				//auto thridkill = ::GetCurrentThread(); // pseudo code, 0xfffff... (https://weseetips.wordpress.com/2008/03/26/getcurrentthread-returns-pseudo-handle-not-the-real-handle/)
				HANDLE thridkill = 0;
				DuplicateHandle(GetCurrentProcess(), // Source Process Handle.
					GetCurrentThread(),  // Source Handle to dup.
					GetCurrentProcess(), // Target Process Handle.
					&thridkill,        // Target Handle pointer.
					0,                   // Options flag.
					TRUE,                // Inheritable flag
					DUPLICATE_SAME_ACCESS);// Options

				switch (mode) {
					// if timeout, run function and then EXIT THE WHOLE APP
				case autoabort::abort_mode::ASYNC_TIMEOUT_RUN_AND_EXIT:
					running = new std::thread([=] {
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel) return;
						if (f) f();
						exit(EXIT_FAILURE);
						});
					break;
					// if timeout, kill the THREAD that called it, run function and die.
				case autoabort::abort_mode::ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN:
					running = new std::thread([=] {
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel) return;
						::TerminateThread(thridkill, 1);
						if (abort_cancel) {
							delete abort_cancel;
							abort_cancel = nullptr;
						}
						if (f) f();
						running->detach();
						delete running; // self, probably will cause errors, but it is detached, so probably won't exit the app lmao
						});
					break;
					// if timeout, run function, kill the THREAD that called it and die.
				case autoabort::abort_mode::ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF:
					running = new std::thread([=] {
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel) return;
						if (f) f();
						::TerminateThread(thridkill, 1);
						if (abort_cancel) {
							delete abort_cancel;
							abort_cancel = nullptr;
						}
						running->detach();
						delete running; // self, probably will cause errors, but it is detached, so probably won't exit the app lmao

						});
					break;
				}
			}
			AutoAbort::~AutoAbort() {
				if (running) {
					if (abort_cancel) {
						*abort_cancel = true;
						if (running && running->joinable()) {
							running->join();
						}
					}
					else {
						if (running) {
							running->detach();
						}
					}
					if (running) {
						delete running;
						running = nullptr;
					}
				}
				if (abort_cancel) {
					delete abort_cancel;
					abort_cancel = nullptr;
				}
			}

			void AutoAbort::abort() {
				if (abort_cancel) *abort_cancel = true;
				else throw Abort::Abort(__FUNCSIG__, "Something REALLY BAD happened! abort_cancel is NULL. Cannot cancel the ABORT!");
			}




			std::string operator+(const Abort& a) {
				return static_cast<std::string>(a);
			}
		}
	}
}