#include "autoabort.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			AutoAbort::AutoAbort(const autoabort::abort_mode mode, const size_t timeout_raw, std::function<void(void)> f) {
				std::chrono::milliseconds timeout = std::chrono::milliseconds(timeout_raw);
				abort_cancel = new bool(false);
				if (!abort_cancel) throw Handling::Abort::Abort(__FUNCSIG__, "Cannot alloc memory!");

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
					running.set([&,f,thridkill](boolThreadF) {
						bool* abort_cancel_cpy = abort_cancel;
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel_cpy)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel_cpy) {
							delete abort_cancel_cpy;
							abort_cancel = nullptr;
							return;
						}
						if (f) f();
						exit(EXIT_FAILURE);
					});
					running.start();
					break;
					// if timeout, kill the THREAD that called it, run function and die.
				case autoabort::abort_mode::ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN:
					running.set([&,f, thridkill](boolThreadF) {
						bool* abort_cancel_cpy = abort_cancel;
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel_cpy)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel_cpy) {
							delete abort_cancel_cpy;
							abort_cancel = nullptr;
							return;
						}
						::TerminateThread(thridkill, 1);
						if (abort_cancel_cpy) {
							delete abort_cancel_cpy;
							abort_cancel_cpy = nullptr;
						}
						if (f) f();
						//running.kill(); // self, probably will cause errors, but it is detached, so probably won't exit the app lmao
					});
					running.start();
					break;
					// if timeout, run function, kill the THREAD that called it and die.
				case autoabort::abort_mode::ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF:
					running.set([&,f, thridkill](boolThreadF) {
						bool* abort_cancel_cpy = abort_cancel;
						std::chrono::milliseconds timeout_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + timeout;
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) < timeout_t && !(*abort_cancel_cpy)) std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (*abort_cancel_cpy) {
							delete abort_cancel_cpy;
							abort_cancel = nullptr;
							return;
						}
						if (f) f();
						::TerminateThread(thridkill, 1);
						if (abort_cancel_cpy) {
							delete abort_cancel_cpy;
							abort_cancel_cpy = nullptr;
						}
						//running.kill(); // self, probably will cause errors, but it is detached, so probably won't exit the app lmao

					});
					running.start();
					break;
				}
			}
			AutoAbort::~AutoAbort() {
				if (running.running()) {
					running.kill();
				}
				if (abort_cancel) {
					delete abort_cancel;
					abort_cancel = nullptr;
				}
			}

			void AutoAbort::abort() {
				if (abort_cancel) *abort_cancel = true;
				else throw Handling::Abort::Abort(__FUNCSIG__, "Something REALLY BAD happened! abort_cancel is NULL. Cannot cancel the ABORT!");
			}

		}
	}
}