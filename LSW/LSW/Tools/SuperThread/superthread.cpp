#include "superthread.h"
/*
namespace LSW {
	namespace v5 {
		namespace Tools {

			SuperThread::SuperThread(const std::function<void(boolThreadF)> f) {
				work = f;
			}
			SuperThread::~SuperThread() {
				kill();
			}
			// set a function to be run (if running, kill, then set)
			void SuperThread::set(const std::function<void(std::function<bool(void)>)> f) {
				if (!thread_done) kill();
				work = f;
			}
			// starts the thread. If there was one, kills and then starts new. arg: TRUE if you want to run DETACHED
			void SuperThread::start(const bool detached) {
				if (!thread_done) kill();
				should_stop = false;
				thread_done = false;
				thr = new std::thread([&]()->void { work([&] {return !should_stop; }); thread_done = true; });
				if (detached) thr->detach();
			}
			// signal to the thread that it should stop running (from boolThreadF argument)
			void SuperThread::stop() {
				should_stop = true;
			}
			// calls stop(), joins and do kill() to cleanup (IF THREAD IS DEADLOCKED OR SOMETHING IT WILL FREEZE HERE)
			void SuperThread::join() {
				if (!thread_done && thr) {
					stop();
					if (thr->joinable()) {
						thr->join(); // not detached, so it can join
						delete thr;
						thr = nullptr;
						thread_done = true; // just to be sure
					}
					else kill(); // clear / kill
				}
				thread_done = true; // just to be sure
				should_stop = false;
			}
			// if joinable, terminates thread, else just clean up
			void SuperThread::kill() {
				if (thr) {
					if (thr->joinable()) {
						thr->detach();
					}
					auto handlr = thr->native_handle();
					::TerminateThread(handlr, 1);
					delete thr;
					thr = nullptr;
					thread_done = true;
				}
			}
			// if there's a thread, true
			bool SuperThread::running() {
				return !thread_done;
			}
		}
	}
}*/