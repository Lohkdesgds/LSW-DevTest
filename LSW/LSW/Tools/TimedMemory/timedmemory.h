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

			template<typename T>
			class TimedMemory {
				std::shared_ptr<T> mem;
				std::thread kill_t;
				bool die = false;
			public:
				TimedMemory() = delete;
				template<typename... Args>
				TimedMemory(size_t delta_o, Args... args) {
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
				template<typename... Args>
				TimedMemory(std::chrono::milliseconds delta, Args... args) {
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
				~TimedMemory() {
					if (!die) {
						die = true;
						if (kill_t.joinable()) kill_t.join();
						mem.reset();
					}
				}

				std::shared_ptr<T> get() {
					return mem;
				}
				const std::shared_ptr<T> get() const {
					return mem;
				}

			};
		}
	}
}