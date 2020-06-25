#pragma once


/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
SuperMap header:

- SuperMap is an unlimited amount of SuperPairs combined into a giant 'std::map like' mapping configuration
- You can have multiple types (defined by each pair) leading to undefined values (no one really knows what's going on here lmao)

= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
*/


// C
#include <stdio.h>
// C++
#include <mutex>
#include <vector>
#include <typeinfo>
#include <typeindex>

// import
#include "superpair.h"
// underlying: [depth]
// [1] -> autocaster.h
// [1] -> abort.h

namespace LSW {
	namespace v5 {

		template<typename T>
		class SuperMap {
			std::vector<SuperPair<T>> sps;
		public:
			SuperMap() = default;
			~SuperMap() {
				sps.clear();
			}
			void clear() {
				sps.clear();
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Import functions (auto copy)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			SuperMap(std::initializer_list<SuperPair<T>> spTi)
			{
				for (auto& i : spTi) {
					SuperPair<T> spp;
					spp = i;
					add(spp);
				}
			}
			SuperMap(const SuperMap& mp) {
				for (auto& i : *mp.__getcopy()) {
					SuperPair<T> spp;
					spp = i;
					add(spp);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Simple functions

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			void add(const SuperPair<T>& sp) {
				sps.push_back(sp);
			}
			const std::vector<SuperPair<T>>* __getcopy() const {
				return &sps;
			}

			auto begin() { return sps.begin(); }
			auto end() { return sps.end(); }
			auto begin() const { return sps.begin(); }
			auto end() const { return sps.end(); }

			const size_t size() const { return sps.size(); }

			void erase(const size_t start) { sps.erase(sps.begin() + start); }
			void erase(const size_t start, const size_t end) { sps.erase(sps.begin() + start, sps.begin() + end); }


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			OPERATORS [], () (non constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			T* operator[](K key) {
				for (auto& i : sps) if (T* ptr; ptr = i[key]) return ptr;
				return nullptr;
			}
			template<typename K, size_t size_o>
			T* operator[](K(&key)[size_o]) {
				for (auto& i : sps) if (T* ptr; ptr = i(key, size_o)) return ptr;
				return nullptr;
			}


			template<typename K>
			T* operator()(K key) {
				for (auto& i : sps) if (T* ptr; ptr = i(key)) return ptr;
				return nullptr;
			}
			template<typename K, size_t size_o>
			T* operator()(K(&key)[size_o]) {
				for (auto& i : sps) if (T* ptr; ptr = i(key, size_o)) return ptr;
				return nullptr;
			}
			template<typename K>
			T* operator()(K* key, size_t size) {
				for (auto& i : sps) if (T* ptr; ptr = i(key, size)) return ptr;
				return nullptr;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			OPERATORS [], () (CONSTANT)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			const T* operator[](K key) const {
				for (auto& i : sps) if (const T* ptr = i(key); ptr) return ptr;
				return nullptr;
			}
			template<typename K, size_t size_o>
			const T* operator[](K(&key)[size_o]) const {
				for (auto& i : sps) if (const T* ptr = i(key, size_o); ptr) return ptr;
				return nullptr;
			}


			template<typename K>
			const T* operator()(K key) const {
				for (auto& i : sps) if (const T* ptr = i[key]; ptr) return ptr;
				return nullptr;
			}
			template<typename K, size_t size_o>
			const T* operator()(K(&key)[size_o]) const {
				for (auto& i : sps) if (const T* ptr = i(key, size_o); ptr) return ptr;
				return nullptr;
			}
			template<typename K>
			const T* operator()(K* key, size_t size) const {
				for (auto& i : sps) if (const T* ptr = i(key, size); ptr) return ptr;
				return nullptr;
			}
		};

	}
}