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
#include "..\SuperPair\superpair.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

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
					add(spTi);
				}
				SuperMap(const SuperMap& mp)
				{
					add(mp);
				}

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Simple functions

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				void add(const SuperMap& mp)
				{
					for (auto& i : mp.sps) {
						SuperPair<T> spp;
						spp = i;
						add(spp);
					}
				}
				void add(std::initializer_list<SuperPair<T>> spTi) {
					for (auto& i : spTi) {
						SuperPair<T> spp;
						spp = i;
						add(spp);
					}
				}
				void add(const SuperPair<T>& sp) {
					sps.push_back(sp);
				}

				auto begin() { return sps.begin(); }
				auto end() { return sps.end(); }
				auto begin() const { return sps.begin(); }
				auto end() const { return sps.end(); }

				const size_t size() const { return sps.size(); }

				void erase(const size_t start) { sps.erase(sps.begin() + start); }
				void erase(const size_t start, const size_t end) { sps.erase(sps.begin() + start, sps.begin() + end); }


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				GetPair (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				template<typename K>
				SuperPair<T>* get_pair(K key) {
					for (auto& i : sps) if (T* ptr; ptr = i[key]) return &i;
					return nullptr;
				}
				template<typename K, size_t size_o>
				SuperPair<T>* get_pair(K(&key)[size_o]) {
					for (auto& i : sps) if (T* ptr; ptr = i(key, size_o)) return &i;
					return nullptr;
				}
				template<typename K>
				SuperPair<T>* get_pair(K* key, size_t size) {
					for (auto& i : sps) if (T* ptr; ptr = i(key, size)) return &i;
					return nullptr;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				OPERATORS [], () (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				template<typename K>
				T* operator[](K key) {
					for (auto& i : sps) if (T* ptr; ptr = i[key]) return ptr;
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
			};

		}
	}
}