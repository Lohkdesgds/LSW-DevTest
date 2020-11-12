#pragma once


// C++
#include <functional>

#include "../../Handling/Abort/abort.h"
#include "../../Handling/Initialize/initialize.h"
#include "../Future/future.h"
#include "../Common/common.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace superthread {

				enum class performance_mode {
					BALANCED, // up to 2000 events / sec
					LOW_POWER, // up to 500 events / sec
					VERY_LOW_POWER, // up to 125 events / sec
					PERFORMANCE, // up to 10000 events / sec
					HIGH_PERFORMANCE // undefined max number of events
				};

			}

			// function that returns TRUE if you should KEEP RUNNING.
			using boolThreadF = std::function<bool(void)>;

			template<typename T>
			class SuperThreadT {
				ALLEGRO_THREAD* thr = nullptr;
				bool _thread_done_flag = true;
				Promise<T> promise;
				superthread::performance_mode perform{}; // balanced

				void _perf();

				static void* __run_i_al(ALLEGRO_THREAD* thr, void* arg);

				// if you have to set after kill, easy way
				template<typename Q = T, std::enable_if_t<!std::is_void_v<Q>, int> = 0>
				void _set_promise_forced();

				// if you have to set after kill, easy way
				template<typename Q = T, std::enable_if_t<std::is_void_v<Q>, int> = 0>
				void _set_promise_forced();
			public:
				/// <summary>
				/// <para>Initialize necessary stuff.</para>
				/// </summary>
				SuperThreadT();

				/// <summary>
				/// <para>Constructor that sets directly the performance mode.</para>
				/// </summary>
				/// <param name="{performance_mode}">How fast should this run?</param>
				SuperThreadT(const superthread::performance_mode&);

				/// <summary>
				/// <para>Constructor that sets directly the function.</para>
				/// </summary>
				/// <param name="{std::function}">The function loop based on bool function argument.</param>
				SuperThreadT(const std::function<T(boolThreadF)>);
				~SuperThreadT();


				/// <summary>
				/// <para>Sets the function that this thread will run (loop or not).</para>
				/// </summary>
				/// <param name="{std::function}">The function loop based on bool function argument.</param>
				template<typename Q = T, std::enable_if_t<!std::is_void_v<Q>, int> = 0>
				void set(const std::function<T(boolThreadF)>);

				/// <summary>
				/// <para>Sets the function that this thread will run (loop or not).</para>
				/// </summary>
				/// <param name="{std::function}">The function loop based on bool function argument.</param>
				template<typename Q = T, std::enable_if_t<std::is_void_v<Q>, int> = 0>
				void set(const std::function<T(boolThreadF)>);

				/// <summary>
				/// <para>Starts the thread and gives you a Future if you need the response later of type T.</para>
				/// </summary>
				/// <returns>{Future} The Future that will hold the value the function you've set returns.</returns>
				Future<T> start();

				/// <summary>
				/// <para>Multitasking sometimes doesn't work without some adjustments. What performance mode you want?</para>
				/// </summary>
				/// <param name="{performance_mode}">How hard should it hit the CPU?</param>
				void set_performance_mode(const superthread::performance_mode&);

				/// <summary>
				/// <para>Sets internal bool to say stop.</para>
				/// </summary>
				void stop();

				/// <summary>
				/// <para>Tries to join the thread (if possible) (can lock).</para>
				/// <para>PS: It will set to stop before actually trying to join.</para>
				/// </summary>
				void join();

				/// <summary>
				/// <para>Kills the thread via terminate.</para>
				/// </summary>
				void kill();

				/// <summary>
				/// <para>Returns true if there's a thread running right now.</para>
				/// </summary>
				/// <returns>{bool} Running?</returns>
				bool running() const;
			};


			// IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION //


			template<typename T>
			template<typename Q, std::enable_if_t<!std::is_void_v<Q>, int>>
			void SuperThreadT<T>::_set_promise_forced()
			{
				promise.set_value(T{});
			}


			template<typename T>
			template<typename Q, std::enable_if_t<std::is_void_v<Q>, int>>
			void SuperThreadT<T>::_set_promise_forced()
			{
				promise.set_value();
			}


			template<typename T>
			inline void SuperThreadT<T>::_perf()
			{
				switch (perform) {
				case superthread::performance_mode::BALANCED:
					std::this_thread::yield();
					std::this_thread::sleep_for(std::chrono::microseconds(500)); // top 2000 loops
					std::this_thread::yield();
					break;
				case superthread::performance_mode::LOW_POWER:
					std::this_thread::yield();
					std::this_thread::sleep_for(std::chrono::milliseconds(2)); // top 500 loops
					std::this_thread::yield();
					break;
				case superthread::performance_mode::VERY_LOW_POWER:
					std::this_thread::yield();
					std::this_thread::sleep_for(std::chrono::milliseconds(8)); // top 125 loops
					std::this_thread::yield();
					break;
				case superthread::performance_mode::PERFORMANCE:
					std::this_thread::yield();
					std::this_thread::sleep_for(std::chrono::microseconds(100)); // top 10000 loops
					std::this_thread::yield();
					break;
				case superthread::performance_mode::HIGH_PERFORMANCE:
					std::this_thread::yield(); // sync
					break;
				}
			}

			template<typename T>
			inline void* SuperThreadT<T>::__run_i_al(ALLEGRO_THREAD* thr, void* arg)
			{
				if (!arg) throw Handling::Abort(__FUNCSIG__, "Invalid thread argument internally!");
				Promise<T>* f = (Promise<T>*)arg;
				f->work(); // has !al_get_thread_should_stop(thr) internally
				return thr;
			}

			template<typename T>
			inline SuperThreadT<T>::SuperThreadT()
			{
				Handling::init_basic();
			}

			template<typename T>
			SuperThreadT<T>::SuperThreadT(const superthread::performance_mode& m)
			{
				Handling::init_basic();
				set_performance_mode(m);
			}

			template<typename T>
			SuperThreadT<T>::SuperThreadT(const std::function<T(boolThreadF)> f)
			{
				Handling::init_basic();
				set(f);
			}

			template<typename T>
			SuperThreadT<T>::~SuperThreadT()
			{
				kill();
			}


			template<typename T>
			template<typename Q, std::enable_if_t<!std::is_void_v<Q>, int>>
			void SuperThreadT<T>::set(const std::function<T(boolThreadF)> f)
			{
				join();
				promise = std::move(Promise<T>([&,f] {
					try {
						_thread_done_flag = false;
						T cpy = f([&] { _perf(); return !al_get_thread_should_stop(thr); });
						_thread_done_flag = true;
						return std::move(cpy);
					}
					catch (const Handling::Abort& e) { // for now. later: save and get elsewhere
						std::cout << "Exception at SuperThread #" << Tools::get_thread_id() << ": " << e.what() << std::endl;
					}
					catch (...) { // for now. later: save and get elsewhere
						std::cout << "Unknown exception at SuperThread #" << Tools::get_thread_id() << "." << std::endl;
					}
					return T{};
				}));
			}

			template<typename T>
			template<typename Q, std::enable_if_t<std::is_void_v<Q>, int>>
			void SuperThreadT<T>::set(const std::function<T(boolThreadF)> f)
			{
				join();
				promise = std::move(Promise<T>([&, f] {
					try {
						_thread_done_flag = false;
						f([&] { _perf(); return !al_get_thread_should_stop(thr); });
						_thread_done_flag = true;
					}
					catch (const Handling::Abort& e) { // for now. later: save and get elsewhere
						std::cout << "Exception at SuperThread #" << Tools::get_thread_id() << ": " << e.what() << std::endl;
					}
					catch (...) { // for now. later: save and get elsewhere
						std::cout << "Unknown exception at SuperThread #" << Tools::get_thread_id() << "." << std::endl;
					}
				}));
			}

			template<typename T>
			Future<T> SuperThreadT<T>::start()
			{
				join();
				Future<T> fut = promise.get_future();
				thr = al_create_thread(__run_i_al, &promise);
				al_start_thread(thr);
				
				return fut;
			}

			template<typename T>
			inline void SuperThreadT<T>::set_performance_mode(const superthread::performance_mode& mode)
			{
				perform = mode;
			}

			template<typename T>
			void SuperThreadT<T>::stop()
			{
				if (thr) al_set_thread_should_stop(thr);
			}

			template<typename T>
			void SuperThreadT<T>::join()
			{
				if (thr) {
					//al_set_thread_should_stop(thr);
					al_join_thread(thr, nullptr);
					while (!_thread_done_flag) std::this_thread::sleep_for(std::chrono::milliseconds(20));
					al_destroy_thread(thr);
					if (!promise.has_set()) _set_promise_forced();
					thr = nullptr;
				}
			}

			template<typename T>
			void SuperThreadT<T>::kill()
			{
				if (thr) {
					al_set_thread_should_stop(thr);
					if (_thread_done_flag) {
						if (!promise.has_set()) _set_promise_forced();
						al_join_thread(thr, nullptr);
						al_destroy_thread(thr);
						thr = nullptr;
					}
					else { // might be internally blocked or the user is crazy and wants to kill it anyways.
						al_destroy_thread(thr);
						thr = nullptr;
						if (!promise.has_set()) _set_promise_forced();
						_thread_done_flag = true;
					}
				}
			}

			template<typename T>
			bool SuperThreadT<T>::running() const
			{
				return thr && !_thread_done_flag;
			}

			using SuperThread = SuperThreadT<void>;
		}
	}
}