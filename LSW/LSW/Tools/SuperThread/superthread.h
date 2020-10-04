#pragma once

// C
#include <assert.h>
#include <Windows.h>
// C++
#include <thread>
#include <functional>

#include "..\Future\future.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			using boolThreadF = std::function<bool(void)>;

			template<typename T>
			class SuperThreadT {
				std::thread* thr = nullptr;
				bool should_stop = false;
				bool thread_done = true;
				std::function<T(std::function<bool(void)>)> work;

				Promise<T>* ree = nullptr;

			public:
				SuperThreadT() = default;
				SuperThreadT(const std::function<T(boolThreadF)> f) {
					work = f;
				}
				~SuperThreadT() {
					kill();
				}
				// set a function to be run (if running, kill, then set)
				void set(const std::function<T(std::function<bool(void)>)> f) {
					if (!thread_done) kill();
					work = f;
				}
				// starts the thread. If there was one, kills and then starts new. arg: TRUE if you want to run DETACHED

				Future<T> start(const bool detached = false) {
					if (!thread_done) kill();
					if (ree) {
						ree->set_value(T{});
						delete ree;
					}
					ree = new Promise<T>([&] { return work([&] {return !should_stop; });});
					should_stop = false;
					thread_done = false;
					thr = new std::thread([&]()->void { ree->work(); thread_done = true; });
					if (detached) thr->detach();

					return std::move(ree->get_future());
				}
				// signal to the thread that it should stop running (from boolThreadF argument)
				void stop() {
					should_stop = true;
				}
				// calls stop(), joins and do kill() to cleanup (IF THREAD IS DEADLOCKED OR SOMETHING IT WILL FREEZE HERE)
				void join() {
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
				void kill() {
					if (thr) {
						if (thr->joinable()) {
							thr->detach();
						}
						auto handlr = thr->native_handle();
						::TerminateThread(handlr, 1);
						delete thr;
						thr = nullptr;
						ree->set_value(T{});
						thread_done = true;
					}
					should_stop = false;
				}
				// if there's a thread, true
				bool running() {
					return !thread_done;
				}
			};

			template<>
			class SuperThreadT<void> {
				std::thread* thr = nullptr;
				bool should_stop = false;
				bool thread_done = true;
				std::function<void(std::function<bool(void)>)> work;

				Promise<void>* ree = nullptr;

			public:
				SuperThreadT() = default;
				SuperThreadT(const std::function<void(boolThreadF)> f) {
					work = f;
				}
				~SuperThreadT() {
					kill();
				}
				// set a function to be run (if running, kill, then set)
				void set(const std::function<void(std::function<bool(void)>)> f) {
					if (!thread_done) kill();
					work = f;
				}
				// starts the thread. If there was one, kills and then starts new. arg: TRUE if you want to run DETACHED

				Future<void> start(const bool detached = false) {
					if (!thread_done) kill();
					if (ree) {
						ree->set_value();
						delete ree;
					}
					ree = new Promise<void>([&] { work([&] {return !should_stop; });});
					should_stop = false;
					thread_done = false;
					thr = new std::thread([&]()->void { ree->work(); thread_done = true; });
					if (detached) thr->detach();

					return std::move(ree->get_future());
				}
				// signal to the thread that it should stop running (from boolThreadF argument)
				void stop() {
					should_stop = true;
				}
				// calls stop(), joins and do kill() to cleanup (IF THREAD IS DEADLOCKED OR SOMETHING IT WILL FREEZE HERE)
				void join() {
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
				void kill() {
					if (thr) {
						if (thr->joinable()) {
							thr->detach();
						}
						auto handlr = thr->native_handle();
						::TerminateThread(handlr, 1);
						delete thr;
						thr = nullptr;
						if (ree) {
							if (!ree->has_set()) {
								ree->set_value();
							}
						}
						thread_done = true;
					}
					should_stop = false;
				}
				// if there's a thread, true
				bool running() {
					return !thread_done;
				}
			};

			using SuperThread = SuperThreadT<void>;
		}
	}
}