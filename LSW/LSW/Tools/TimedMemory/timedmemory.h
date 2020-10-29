#pragma once

#include <memory>
#include <thread>
#include <iostream>

#include "..\..\Handling\Abort\abort.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace timedmemory {
				constexpr std::chrono::milliseconds default_delta_min = std::chrono::milliseconds(50); // ms
			}

			/// <summary>
			/// <para>So you want a memory to be available for some time, then clear? This is probably what you need.</para>
			/// <para>Not very efficient, but very easy to use, TimedMemory will clear its internal reference of a shared_ptr when timeout happends.</para>
			/// <para>We don't want crashes, and so do I. The memory is cleared after timeout AND your full use, so if you take a little longer, no worries, shared ptr is there for you.</para>
			/// <para># One thing: keep this object alive longer than the timeout, or it might crash or be cleaned up earlier.</para>
			/// </summary>
			template<typename T>
			class TimedMemory {
				std::shared_ptr<T> mem;
				std::thread kill_t;
				bool die = false;
			public:
				TimedMemory() = delete;

				/// <summary>
				/// <para>Creates the memory with timeout and arguments to new.</para>
				/// </summary>
				/// <param name="{size_t}">Timeout in milliseconds.</param>
				/// <param name="{Args...}">Arguments to the new call.</param>
				template<typename... Args>
				TimedMemory(size_t, Args...);

				/// <summary>
				/// <para>Creates the memory with timeout and arguments to new.</para>
				/// </summary>
				/// <param name="{std::chrono::milliseconds}">Timeout.</param>
				/// <param name="{Args...}">Arguments to the new call.</param>
				template<typename... Args>
				TimedMemory(std::chrono::milliseconds, Args...);

				~TimedMemory();


				/// <summary>
				/// <para>Gets a copy of the shared_ptr so no errors.</para>
				/// </summary>
				/// <returns>{shared_ptr} A copy of internal shared_ptr.</returns>
				std::shared_ptr<T> get();

				/// <summary>
				/// <para>Gets a copy of the shared_ptr so no errors.</para>
				/// </summary>
				/// <returns>{shared_ptr} A copy of internal shared_ptr.</returns>
				const std::shared_ptr<T> get() const;

			};


			template<typename T>
			template<typename... Args>
			TimedMemory<T>::TimedMemory(size_t delta_o, Args... args) {
				mem = std::make_shared<T>(args...);
				std::chrono::milliseconds delta = std::chrono::milliseconds(delta_o);
				kill_t = std::thread([&, delta] {
					if (delta > timedmemory::default_delta_min) {
						auto noww = std::chrono::high_resolution_clock::now();
						while (std::chrono::high_resolution_clock::now() - noww < (delta - timedmemory::default_delta_min)) {
							std::this_thread::sleep_for(timedmemory::default_delta_min);
						}
						std::this_thread::sleep_for(timedmemory::default_delta_min);
					}
					else std::this_thread::sleep_for(delta);
					mem.reset();
					});
			}

			template<typename T>
			template<typename... Args>
			TimedMemory<T>::TimedMemory(std::chrono::milliseconds delta, Args... args) {
				mem = std::make_shared<T>(args...);
				kill_t = std::thread([&, delta] {
					if (delta > timedmemory::default_delta_min) {
						auto noww = std::chrono::high_resolution_clock::now();
						while (std::chrono::high_resolution_clock::now() - noww < (delta - timedmemory::default_delta_min)) {
							std::this_thread::sleep_for(timedmemory::default_delta_min);
						}
						std::this_thread::sleep_for(timedmemory::default_delta_min);
					}
					else std::this_thread::sleep_for(delta);
					mem.reset();
					});
			}

			template<typename T>
			TimedMemory<T>::~TimedMemory() {
				if (!die) {
					die = true;
					if (kill_t.joinable()) kill_t.join();
					mem.reset();
				}
			}


			template<typename T>
			std::shared_ptr<T> TimedMemory<T>::get() {
				return mem;
			}

			template<typename T>
			const std::shared_ptr<T> TimedMemory<T>::get() const {
				return mem;
			}
		}
	}
}