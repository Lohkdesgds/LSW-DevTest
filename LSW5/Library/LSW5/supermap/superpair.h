#pragma once


/*
= - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - =
SuperPair header:

- SuperPair is a subdivision of SuperMap. It has its own file because it is HUGE.
- SuperPair is each pair with any kind of key tied to a value. It is the std::pair, but with any (1 or more keys) to the same value.

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
#include "..\autocaster\autocaster.h"
#include "..\abort\abort.h"


namespace LSW {
	namespace v5 {

		/*************************************************************************************

		# DataAny
		> Any data. Just that.
		> PASSED 20200318

		**************************************************************************************/


		template<typename T>
		class __data_superpair {
			T* dat = nullptr;
			size_t siz = 0;
			mutable std::mutex m;
		public:
			// default constructor
			__data_superpair(size_t size) {
				if (size) alloc(size);
			}
			__data_superpair(T* def, size_t size) {
				if (size && def) {
					alloc(size);
					memcpy_s(dat, siz, def, size);
				}
			}
			__data_superpair(const __data_superpair& daa) {
				alloc(daa.size());
				memcpy_s(dat, siz, daa.c_data(), siz);
			}
			// default destructor
			~__data_superpair() {
				dealloc();
			}
			// dealloc data
			void dealloc() {
				m.lock();
				if (dat) {
					delete[] dat;
					dat = nullptr;
				}
				m.unlock();
			}
			// alloc data
			void alloc(size_t size) {
				if (size) {
					if (dat) dealloc();
					m.lock();
					dat = new T[siz = size];
					m.unlock();
				}
			}
			// data itself
			T* data() {
				return dat;
			}
			// const data
			T* c_data() const {
				return dat;
			}
			// size based on type, real size in bytes is this * sizeof(type)
			size_t size() const {
				return siz;
			}
			__data_superpair& operator=(const __data_superpair& daa) {
				alloc(daa.size());
				memcpy_s(dat, siz, daa.c_data(), siz);
				return *this;
			}
		};


		/*************************************************************************************

			# SuperPair: V6.0A
			> Many keys to a same position
			> Direct SuperMap: [arg] match someone? get key then lmao
			> PASSED 20200318

		**************************************************************************************/

		template<typename T>
		class SuperPair {
			std::vector<std::pair<std::type_index, __data_superpair<char>>> pairs;
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
			// alt
			template<typename K, typename... Args>
			inline void handleInput(K* ref, size_t size, Args... args)
			{
				set(ref, size);
				handleInput(args...);
			}
			template<typename K>
			inline void handleInput(K* ref, size_t size)
			{
				set(ref, size);
			}
			template<typename K, typename... Args>
			inline void handleInput(K* ref, int size, Args... args)
			{
				set(ref, size);
				handleInput(args...);
			}
			template<typename K>
			inline void handleInput(K* ref, int size)
			{
				set(ref, size);
			}
			// SUPER HANDLE //

		public:
			SuperPair() = default;// {
				//printf_s("new SuperPair &: %lld\n", (uintptr_t)this);
			//}
			~SuperPair() {
				//printf_s("dealloc of SuperPair &: %lld\n", (uintptr_t)this);
				for (auto& i : pairs) i.second.dealloc();
				pairs.clear();
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Import functions (auto copy)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template<typename K>
			SuperPair(const SuperPair<K>& sp) {
				//printf_s("new SuperPair &: %lld\n", (uintptr_t)this);
				for (auto& i : *sp.__getcopy()) {
					pairs.push_back(std::pair<std::type_index, __data_superpair<char>>{ i.first, __data_superpair<char>(i.second.c_data(), i.second.size()) }); // copies data, not reference
				}
				holding = sp.getValue();
			}
			template<typename... Args>
			SuperPair(const T& val, Args... args)
			{
				//printf_s("new SuperPair &: %lld\n", (uintptr_t)this);
				holding = val;
				handleInput(args...);
			}
			SuperPair(const T& val)
			{
				//printf_s("new SuperPair &: %lld\n", (uintptr_t)this);
				holding = val;
			}
			template<typename K>
			SuperPair& operator=(const SuperPair<K>& sp)
			{
				for (auto& i : *sp.__getcopy()) {
					pairs.push_back(std::pair<std::type_index, __data_superpair<char>>{ i.first, __data_superpair<char>(i.second.c_data(), i.second.size()) }); // copies data, not reference
				}
				holding = sp.getValue();
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Simple functions

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			void setValue(T& nval) {
				holding = nval;
			}
			T& getValue() {
				return holding;
			}
			const T& getValue() const {
				return holding;
			}
			const std::vector<std::pair<std::type_index, __data_superpair<char>>>* __getcopy() const {
				return &pairs;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			SET (non constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			bool set(K key) {
				if (std::is_pointer<K>::value) return false;
				if (std::is_array<K>::value) return false;
				auto* copyin = Cast::r_cast<K*>(&key);
				size_t actual_size_c = sizeof(*copyin);

				// copyin is a value, not pointer
				for (auto& i : pairs) {
					if (i.first == typeid(*copyin)) {
						memcpy_s(i.second.data(), i.second.size(), copyin, actual_size_c);
						return true;
					}
				}
				// new
				pairs.push_back({ typeid(*copyin), __data_superpair<char>((char*)copyin, actual_size_c) });
				return true;
			}
			template<typename K, size_t size_o>
			bool set(K(&key)[size_o]) {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				// copyin is a value, not pointer
				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						i.second.alloc(actual_size_c);
						memcpy_s(i.second.data(), i.second.size(), copyin, actual_size_c);
						return true;
					}
				}
				// new
				pairs.push_back({ typeid(copyin), __data_superpair<char>((char*)copyin, actual_size_c) });
				return true;
			}
			template<typename K>
			bool set(K* key, size_t size) {
				auto* copyin = Cast::r_cast<K*>(key);
				size_t actual_size_c = sizeof(*copyin) * size;

				// copyin is a value, not pointer
				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						i.second.alloc(actual_size_c);
						memcpy_s(i.second.data(), i.second.size(), copyin, actual_size_c);
						return true;
					}
				}
				// new
				pairs.push_back({ typeid(copyin), __data_superpair<char>((char*)copyin, actual_size_c) });
				return true;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			OPERATORS [], () (non constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			T* operator[](K key) {
				if (std::is_pointer<K>::value) return nullptr;
				if (std::is_array<K>::value) return nullptr;
				auto* copyin = Cast::r_cast<K*>(&key);
				size_t actual_size_c = sizeof(*copyin);

				for (auto& i : pairs) {
					if (i.first == typeid(*copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K, size_t size_o>
			T* operator[](K(&key)[size_o]) {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}


			template<typename K>
			T* operator()(K key) {
				if (std::is_pointer<K>::value) return nullptr;
				if (std::is_array<K>::value) return nullptr;
				auto* copyin = Cast::r_cast<K*>(&key);
				size_t actual_size_c = sizeof(*copyin);

				for (auto& i : pairs) {
					if (i.first == typeid(*copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K, size_t size_o>
			T* operator()(K(&key)[size_o]) {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K>
			T* operator()(K* key, size_t size_o) {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			OPERATORS [], () (CONSTANT)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			const T* operator[](K key) const {
				if (std::is_pointer<K>::value) return nullptr;
				if (std::is_array<K>::value) return nullptr;
				auto* copyin = Cast::r_cast<K*>(&key);
				size_t actual_size_c = sizeof(*copyin);

				for (auto& i : pairs) {
					if (i.first == typeid(*copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K, size_t size_o>
			const T* operator[](K(&key)[size_o]) const {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}


			template<typename K>
			const T* operator()(K key) const {
				if (std::is_pointer<K>::value) return nullptr;
				if (std::is_array<K>::value) return nullptr;
				auto* copyin = Cast::r_cast<K*>(&key);
				size_t actual_size_c = sizeof(*copyin);

				for (auto& i : pairs) {
					if (i.first == typeid(*copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K, size_t size_o>
			const T* operator()(K(&key)[size_o]) const {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}
			template<typename K>
			const T* operator()(K* key, size_t size_o) const {
				size_t size = size_o;
				auto* copyin = Cast::r_cast<K*>(key);
				if (typeid(copyin) == typeid(char*)) size = strnlen_s(key, size_o);
				size_t actual_size_c = sizeof(*copyin) * size;

				for (auto& i : pairs) {
					if (i.first == typeid(copyin)) {
						if (i.second.size() == actual_size_c && memcmp(i.second.c_data(), copyin, actual_size_c) == 0) return &holding;
					}
				}
				return nullptr;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			DEL (non constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			void del(K key) {
				if (std::is_pointer<K>::value) return;
				if (std::is_array<K>::value) return;
				auto* copyin = Cast::r_cast<K*>(&key);

				for (size_t p = 0; p < pairs.size(); p++) {
					auto& i = pairs[p];
					if (i.first == typeid(*copyin)) {
						i.second.dealloc(); // not needed, but why not?
						pairs.erase(pairs.begin() + p--);
					}
				}
			}
			template<typename K, size_t size>
			void del(K(&key)[size]) {
				auto* copyin = Cast::r_cast<K*>(&key);

				for (size_t p = 0; p < pairs.size(); p++) {
					auto& i = pairs[p];
					if (i.first == typeid(copyin)) {
						i.second.dealloc(); // not needed, but why not?
						pairs.erase(pairs.begin() + p--);
					}
				}
			}
			template<typename K>
			void del(K* key, size_t size) {
				auto* copyin = Cast::r_cast<K*>(&key);

				for (size_t p = 0; p < pairs.size(); p++) {
					auto& i = pairs[p];
					if (i.first == typeid(copyin)) {
						i.second.dealloc(); // not needed, but why not?
						pairs.erase(pairs.begin() + p--);
					}
				}
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			hasType (constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			bool hasType() const {
				bool is_ptr = (std::is_pointer<K>::value || std::is_array<K>::value);
				auto* copyin = (lsw_classic_removal<K>*)nullptr;

				for (size_t p = 0; p < pairs.size(); p++) {
					auto& i = pairs[p];
					if (is_ptr) {
						if (i.first == typeid(copyin)) return true;
					}
					else {
						if (i.first == typeid(*copyin)) return true;
					}
				}
				return false;
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			getType (non constant)

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			template<typename K>
			bool getType(K& val, size_t siz = 1) const {
				bool is_ptr = (std::is_pointer<K>::value || std::is_array<K>::value);
				auto* copyin = (lsw_classic_removal<K>*)nullptr;

				for (size_t p = 0; p < pairs.size(); p++) {
					auto& i = pairs[p];
					if (is_ptr) {
						if (i.first == typeid(copyin)) {
							if (siz < i.second.size()) continue;
							memcpy_s(i.second.c_data(), i.second.size(), (void*)val, i.second.size());
							return true;
						}
					}
					else {
						if (i.first == typeid(*copyin)) {
							val = (K)*i.second.c_data();
							return true;
						}
					}
				}
				return false;
			}

			template<typename K, size_t size>
			bool getType(K(&val)[size]) const {
				auto* copyin = (lsw_classic_removal<K>*)val;
				return getType(copyin, size);
			}
		};
	}
}