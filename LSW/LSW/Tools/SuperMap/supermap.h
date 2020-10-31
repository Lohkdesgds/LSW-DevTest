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
#include "../SuperPair/superpair.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			/// <summary>
			/// <para>SuperMap is a std::map like class, but one that uses SuperPair as pairs.</para>
			/// <para>SuperPairs allow multiple different keys to result in a same value, and each value to have different key combinations.</para>
			/// </summary>
			/// <typeparam name="T"></typeparam>
			template<typename T>
			class SuperMap {
				std::vector<SuperPair<T>> sps;
			public:
				SuperMap() = default;
				~SuperMap() {
					sps.clear();
				}

				/// <summary>
				/// <para>Clears the map.</para>
				/// </summary>
				void clear() {
					sps.clear();
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Import functions (auto copy)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Sets a list of multiple SuperPairs.</para>
				/// </summary>
				/// <param name="{initializer_list}">A initializer list of SuperPairs.</param>
				SuperMap(std::initializer_list<SuperPair<T>> spTi)
				{
					add(spTi);
				}

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{SuperMap}">The other SuperMap to copy.</param>
				SuperMap(const SuperMap& mp)
				{
					add(mp);
				}

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{SuperMap}">The other SuperMap to move.</param>
				SuperMap(SuperMap&& mp)
				{
					sps = std::move(mp.sps);
				}

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Simple functions

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Adds a SuperMap into this SuperMap.</para>
				/// </summary>
				/// <param name="{SuperMap}">The SuperMap being added.</param>
				/// <returns></returns>
				void add(const SuperMap& mp)
				{
					for (auto& i : mp.sps) {
						SuperPair<T> spp;
						spp = i;
						add(spp);
					}
				}

				/// <summary>
				/// <para>Adds a initializer list of multiple SuperPairs.</para>
				/// </summary>
				/// <param name="{initializer_list}">A initializer list of SuperPairs.</param>
				void add(std::initializer_list<SuperPair<T>> spTi) {
					for (auto& i : spTi) {
						SuperPair<T> spp;
						spp = i;
						add(spp);
					}
				}

				/// <summary>
				/// <para>Adds a single SuperPair.</para>
				/// </summary>
				/// <param name="{SuperPair}">A SuperPair to add.</param>
				void add(const SuperPair<T>& sp) {
					sps.push_back(sp);
				}


				/// <summary>
				/// <para>Begin of the vector of SuperPairs.</para>
				/// </summary>
				/// <returns>{iterator} Begin iterator.</returns>
				auto begin() { return sps.begin(); }

				/// <summary>
				/// <para>End of the vector of SuperPairs.</para>
				/// </summary>
				/// <returns>{iterator} End iterator.</returns>
				auto end() { return sps.end(); }

				/// <summary>
				/// <para>Begin of the vector of SuperPairs.</para>
				/// </summary>
				/// <returns>{iterator} Begin iterator.</returns>
				auto begin() const { return sps.begin(); }

				/// <summary>
				/// <para>End of the vector of SuperPairs.</para>
				/// </summary>
				/// <returns>{iterator} End iterator.</returns>
				auto end() const { return sps.end(); }


				/// <summary>
				/// <para>The size of the internal vector of SuperPairs.</para>
				/// </summary>
				/// <returns>{size_t} Vector size.</returns>
				const size_t size() const { return sps.size(); }

				/// <summary>
				/// <para>Erases a specific position of the internal vector of SuperPairs.</para>
				/// </summary>
				/// <param name="{size_t}">The position to be deleted.</param>
				void erase(const size_t start) { if (start < size()) sps.erase(sps.begin() + start); }

				/// <summary>
				/// <para>Erases an amount of SuperPairs of the internal vector.</para>
				/// </summary>
				/// <param name="{size_t}">The first position to be deleted.</param>
				/// <param name="{size_t}">The last position to be deleted.</param>
				void erase(const size_t start, const size_t end) { if (start < end && start + end < size()) sps.erase(sps.begin() + start, sps.begin() + end); }


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				GetPair (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Gets a SuperPair based on key if matches.</para>
				/// </summary>
				/// <param name="{K}">A key.</param>
				/// <returns>{SuperPair} The SuperPair with that key.</returns>
				template<typename K>
				SuperPair<T>* get_pair(K key) {
					for (auto& i : sps) if (T* ptr; ptr = i[key]) return &i;
					return nullptr;
				}

				/// <summary>
				/// <para>Gets a SuperPair based on key if matches.</para>
				/// </summary>
				/// <param name="{K}">A key.</param>
				/// <returns>{SuperPair} The SuperPair with that key.</returns>
				template<typename K, size_t size_o>
				SuperPair<T>* get_pair(K(&key)[size_o]) {
					for (auto& i : sps) if (T* ptr; ptr = i(key, size_o)) return &i;
					return nullptr;
				}

				/// <summary>
				/// <para>Gets a SuperPair based on key if matches.</para>
				/// </summary>
				/// <param name="{K}">A key.</param>
				/// <param name="{size_t}">The key length.</param>
				/// <returns>{SuperPair} The SuperPair with that key.</returns>
				template<typename K>
				SuperPair<T>* get_pair(K* key, size_t size) {
					for (auto& i : sps) if (T* ptr; ptr = i(key, size)) return &i;
					return nullptr;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				OPERATORS [], () (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Get the value with that key.</para>
				/// </summary>
				/// <param name="{K}">The key.</param>
				/// <returns>{T} The value, if match, else nullptr.</returns>
				template<typename K>
				T* operator[](K key) {
					for (auto& i : sps) if (T* ptr; ptr = i[key]) return ptr;
					return nullptr;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				OPERATORS [], () (CONSTANT)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Get the value with that key.</para>
				/// </summary>
				/// <param name="{K}">The key.</param>
				/// <returns>{T} The value, if match, else nullptr.</returns>
				template<typename K>
				const T* operator[](K key) const {
					for (auto& i : sps) if (const T* ptr = i(key); ptr) return ptr;
					return nullptr;
				}
			};

		}
	}
}