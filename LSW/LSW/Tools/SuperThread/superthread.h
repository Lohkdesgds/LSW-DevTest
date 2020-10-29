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

			// function that returns TRUE if you should KEEP RUNNING.
			using boolThreadF = std::function<bool(void)>;

			template<typename T>
			class SuperThreadT {
				std::thread thr;
				bool should_stop = false;
				bool _thread_done_flag = true;
				Promise<T> promise;

				// if you have to set after kill, easy way
				template<typename Q = T, std::enable_if_t<!std::is_void_v<Q>, int> = 0>
				void _set_promise_forced();

				// if you have to set after kill, easy way
				template<typename Q = T, std::enable_if_t<std::is_void_v<Q>, int> = 0>
				void _set_promise_forced();
			public:
				SuperThreadT() = default;

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
			SuperThreadT<T>::SuperThreadT(const std::function<T(boolThreadF)> f)
			{
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
				promise = std::move(Promise<T>([&,f] {
					return f([&] {return !should_stop; });
				}));
			}

			template<typename T>
			template<typename Q, std::enable_if_t<std::is_void_v<Q>, int>>
			void SuperThreadT<T>::set(const std::function<T(boolThreadF)> f)
			{
				promise = std::move(Promise<T>([&,f] {					
					f([&] {return !should_stop; });
				}));
			}

			template<typename T>
			Future<T> SuperThreadT<T>::start()
			{
				Future<T> fut = promise.get_future();
				should_stop = false;
				thr = std::thread([&] { _thread_done_flag = false; promise.work(); _thread_done_flag = true; });
				return fut;
			}

			template<typename T>
			void SuperThreadT<T>::stop()
			{
				should_stop = true;
			}

			template<typename T>
			void SuperThreadT<T>::join()
			{
				should_stop = true;
				if (thr.joinable()) thr.join();
				else kill();
			}

			template<typename T>
			void SuperThreadT<T>::kill()
			{
				should_stop = true;
				if (_thread_done_flag) {
					if (!promise.has_set()) _set_promise_forced();
					if (thr.joinable()) thr.join(); // thread done flag up, it should not lock here.
				}
				else { // might be internally blocked or the user is crazy and wants to kill it anyways.
					if (thr.joinable()) {
						thr.detach();
					}
					auto handlr = thr.native_handle();
					::TerminateThread(handlr, 1);
					if (!promise.has_set()) _set_promise_forced();
					_thread_done_flag = true;
				}
			}

			template<typename T>
			bool SuperThreadT<T>::running() const
			{
				return !_thread_done_flag;
			}

			using SuperThread = SuperThreadT<void>;
		}
	}
}