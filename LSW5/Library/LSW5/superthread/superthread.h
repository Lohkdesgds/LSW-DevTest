#pragma once

// C
#define _CRT_RAND_S
#include <assert.h>
// C++
#include <thread>
#include <functional>
// Others
#include "..\allegroinitialization\allegroinitialization.h"

//#define LSW_THREAD_ALLEGRO


namespace LSW {
	namespace v5 {
		namespace Threads {

			using boolThreadF = std::function<bool(void)>;

#ifdef LSW_THREAD_ALLEGRO

			void* __run_i_al(ALLEGRO_THREAD* thr, void* arg) {
				assert(arg);
				std::function<void(void)>* f = (std::function<void(void)>*)arg;
				(*f)(); // func_al
				return nullptr;
			}

			template<typename T>
			class CustomThread { // based on Allegro
				ALLEGRO_THREAD* thr = nullptr;
				std::function<T(std::function<bool(void)>)> work;
				T resul{};
				const std::function<bool(void)> func_awake = [&]()->bool { return awake(); };
				const std::function<void(void)> func_al = [&]()->void { resul = work(func_awake); };

				bool awake() {
					if (thr) {
						return !al_get_thread_should_stop(thr);
					}
					return false;
				}
				void checkSys() {
					lsw_init();
				}
			public:
				CustomThread() {
					checkSys();
				}
				CustomThread(const std::function<T(std::function<bool(void)>)> f = std::function<T(std::function<bool(void)>)>()) {
					checkSys();
					work = f;
				}
				void setFunc(const std::function<T(std::function<bool(void)>)> f) {
					work = f;
				}
				void start() {
					if (thr) kill();
					thr = al_create_thread(&__run_i_al, (void*)const_cast<std::function<void(void)>*>(&func_al));
					al_start_thread(thr);
				}
				void stop() {
					if (thr) {
						al_set_thread_should_stop(thr);
					}
				}
				// also calls stop() and kill() to stop thread loop and clear everything (duplicated stop() from you might not be bad, just duplicated, you know)
				void join() {
					if (thr) {
						stop();
						al_join_thread(thr, nullptr);
						kill(); // clear
					}
				}
				void kill() {
					if (thr) {
						al_destroy_thread(thr);
						thr = nullptr;
					}
				}
				T result() {
					return resul;
				}
				bool existThread() {
					return thr != nullptr;
				}
				void resetResultTo(T val) {
					resul = val;
				}
			};

#else

			template<typename T>
			class CustomThread {
				std::thread* thr = nullptr;
				bool should_stop = false;
				std::function<T(std::function<bool(void)>)> work;
				T resul{};
				const std::function<bool(void)> func_awake = [&]()->bool { return awake(); };
				const std::function<void(void)> func_al = [&]()->void { resul = work(func_awake); };

				bool awake() {
					if (thr) {
						return !should_stop;
					}
					return false;
				}
			public:
				CustomThread(const std::function<T(std::function<bool(void)>)> f = std::function<T(std::function<bool(void)>)>()) {
					work = f;
				}
				void setFunc(const std::function<T(std::function<bool(void)>)> f) {
					work = f;
				}
				void start() {
					if (thr) kill();
					should_stop = false;
					thr = new std::thread(func_al);
				}
				void stop() {
					if (thr) {
						should_stop = true;
					}
				}
				// also calls stop() and kill() to stop thread loop and clear everything (duplicated stop() from you might not be bad, just duplicated, you know)
				void join() {
					if (thr) {
						stop();
						thr->join();
						kill(); // clear
					}
				}
				// std::thread doesn't allow to kill before join()!
				void kill() {
					if (thr) {
						delete thr;
						thr = nullptr;
					}
				}
				T result() {
					return resul;
				}
				bool existThread() {
					return thr != nullptr;
				}
				void resetResultTo(T val) {
					resul = val;
				}
			};

#endif
		}
	}
}