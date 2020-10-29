#pragma once

/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
SuperPair header:

- A subdivision of SuperMap. It has its own file because it is HUGE.
- SuperPair is each pair with any kind of key tied to a value. It is the std::pair, but with any (1 or more keys) to the same value.
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
*/


#include <stdio.h>

#include <mutex>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <any>

#include "..\Common\common.h"


namespace LSW {
	namespace v5 {
		namespace Tools {

			/*************************************************************************************

				# SuperPair: V7.0A
				> Many keys to a same position
				> Direct SuperMap: [arg] match someone? get key then lmao
				> PASSED 20200822

			**************************************************************************************/

			/// <summary>
			/// <para>SuperPair is something like std::pair, but it holds a value to any multiple keys.</para>
			/// </summary>
			template<typename T>
			class SuperPair {
				std::vector<std::any> keys;
				T holding{};

				// SUPER HANDLE //
				template<typename K, typename... Args>
				inline void handleInput(K& ref, Args... args)
				{
					set(ref);
					handleInput(args...);
				}
				template<typename K>
				inline void handleInput(K& ref)
				{
					set(ref);
				}
				// SUPER HANDLE //

			public:
				SuperPair() = default;
				~SuperPair() { keys.clear(); }


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Import functions (auto copy)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{SuperPair}">A SuperPair of the same type to copy.</param>
				template<typename K>
				SuperPair(const SuperPair<K>& sp) {
					keys = sp.keys;
					holding = sp.holding;
				}

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{SuperPair}">A SuperPair of the same type to move.</param>
				template<typename K>
				SuperPair(SuperPair<K>&& sp) {
					keys = std::move(sp.keys);
					holding = std::move(sp.holding);
				}

				/// <summary>
				/// <para>A way to set the value and some keys already.</para>
				/// </summary>
				/// <param name="{T}">The value it will hold.</param>
				/// <param name="{Args...}">Keys to the value.</param>
				template<typename... Args>
				SuperPair(const T& val, Args... args)
				{
					holding = val;
					handleInput(args...);
				}

				/// <summary>
				/// <para>A way to set the value already.</para>
				/// </summary>
				/// <param name="{T}">The value it will hold.</param>
				SuperPair(const T& val)
				{
					holding = val;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Simple functions

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Sets the internal value directly.</para>
				/// </summary>
				/// <param name="{T}">The value to be set.</param>
				void set_value(const T& nval) {
					holding = nval;
				}

				/// <summary>
				/// <para>Gets the value it's holding.</para>
				/// </summary>
				/// <returns>{T} The value's reference.</returns>
				T& get_value() {
					return holding;
				}

				/// <summary>
				/// <para>Gets the value it's holding.</para>
				/// </summary>
				/// <returns>{T} The value's reference.</returns>
				const T& get_value() const {
					return holding;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				SET (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Sets or updates a key.</para>
				/// </summary>
				/// <param name="{Q}">A key.</param>
				template<typename Q>
				void set(Q nkey) {
					for (auto& i : keys) {
						if (i.type() == typeid(r_cast_t<Q>(nkey))) {
							i = r_cast_t<Q>(nkey);
						}
					}
					std::any lol = r_cast_t<Q>(nkey);
					keys.push_back(lol);
				}

				/// <summary>
				/// <para>Sets or updates a key.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				template<>
				void set(const char* nkey) {
					set((char*)nkey);
				}

				/// <summary>
				/// <para>Sets or updates a key.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				template<>
				void set(char* nkey) {
					for (auto& i : keys) {
						if (i.type() == typeid(std::string)) {
							i = std::string(nkey);
						}
					}
					std::any lol = std::string(nkey);
					keys.push_back(lol);
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				OPERATORS [], () (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{Q}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<typename Q>
				T* operator[](Q key)
				{
					for (auto& i : keys) {
						if (i.type() == typeid(key)) {
							if (std::any_cast<base_reverse_cast<Q>>(i) == key)
								return &holding;
						}
					}
					return nullptr;
				}

				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<>
				T* operator[](const char* key) {
					return (*this)[(char*)key];
				}

				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<>
				T* operator[](char* key)
				{
					for (auto& i : keys) {
						if (i.type() == typeid(std::string)) {
							if (std::any_cast<std::string>(i) == key)
								return &holding;
						}
					}
					return nullptr;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				OPERATORS [], () (CONSTANT)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{Q}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<typename Q>
				const T* operator[](Q key) const
				{
					for (auto& i : keys) {
						if (i.type() == typeid(key)) {
							if (std::any_cast<base_reverse_cast<Q>>(i) == key)
								return &holding;
						}
					}
					return nullptr;
				}

				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<>
				const T* operator[](const char* key) const {
					return (*this)[(char*)key];
				}

				/// <summary>
				/// <para>If match, returns the pointer to value, else returns nullptr.</para>
				/// </summary>
				/// <param name="{char*}">A key.</param>
				/// <returns>{T} The value pointer or nullptr if the key doesn't match or doesn't exist.</returns>
				template<>
				const T* operator[](char* key) const
				{
					for (auto& i : keys) {
						if (i.type() == typeid(std::string)) {
							if (std::any_cast<std::string>(i) == key)
								return &holding;
						}
					}
					return nullptr;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				DEL (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Removes a specific key by type.</para>
				/// <para># PS: CHAR* and CONST CHAR* are transformed to std::string when they're set. You won't find char* type.</para>
				/// </summary>
				template<typename Q>
				void remove() {
					for (size_t p = 0; p < keys.size(); p++) {
						if (keys[p].type() == typeid(r_cast_t<Q>))
						{
							keys.erase(keys.begin() + p);
							return;
						}
					}
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				has_type (constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Gets if a key of a type is set or not.</para>
				/// <para># PS: CHAR* and CONST CHAR* are transformed to std::string when they're set. You won't find char* type.</para>
				/// </summary>
				/// <returns>{bool} True if there is one of this type.</returns>
				template<typename Q>
				bool has_type() const
				{
					for (auto& i : keys) if (i.type() == typeid(r_cast_t<Q>)) return true;
					return false;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				get_type (constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				/// <summary>
				/// <para>Gets the key of type K.</para>
				/// <para># PS: CHAR* and CONST CHAR* are transformed to std::string when they're set. You won't find char* type.</para>
				/// </summary>
				/// <param name="{K}">The variable to be set with the value.</param>
				/// <returns></returns>
				template<typename K>
				bool get_type(K& val) const {
					for (auto& i : keys) {
						if (i.type() == typeid(r_cast_t<K>))
						{
							val = std::any_cast<K>(i);
							return true;
						}
					}
					return false;
				}
			};
		}
	}
}