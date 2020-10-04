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
				~SuperPair() {
					keys.clear();
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Import functions (auto copy)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				template<typename K>
				SuperPair(const SuperPair<K>& sp) {
					keys = sp.keys;
					holding = sp.get_value();
				}
				template<typename... Args>
				SuperPair(const T& val, Args... args)
				{
					holding = val;
					handleInput(args...);
				}
				SuperPair(const T& val)
				{
					holding = val;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				Simple functions

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


				void set_value(T& nval) {
					holding = nval;
				}
				T& get_value() {
					return holding;
				}
				const T& get_value() const {
					return holding;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				SET (non constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


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
				template<>
				void set(const char* nkey) {
					set((char*)nkey);
				}
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
				template<>
				T* operator[](const char* key) {
					return (*this)[(char*)key];
				}
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
				template<>
				const T* operator[](const char* key) const {
					return (*this)[(char*)key];
				}
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


				template<typename Q>
				bool has_type() const
				{
					for (auto& i : keys) if (i.type() == typeid(r_cast_t<Q>)) return true;
					return false;
				}


				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

				get_type (constant)

				* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


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