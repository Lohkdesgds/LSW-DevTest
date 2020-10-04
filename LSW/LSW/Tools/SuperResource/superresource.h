#pragma once

// C++
#include <string>
#include <functional>
#include <vector>
#include <mutex>

#include "..\..\Handling\Abort\abort.h"
#include "..\SuperMutex\supermutex.h"


namespace LSW {
	namespace v5 {
		namespace Tools {

			template<typename T>
			class SuperResource {

				template<typename Q>
				struct internal_data {
					std::shared_ptr<Q> _data;
					std::string _id;
					bool _enabled = true;
				};

				template<typename Q>
				struct internal_resources {
					SuperMutex m;
					std::vector<internal_data<Q>> objs;
					std::shared_ptr<Q> main_o;
				};

				static internal_resources<T> saves;
			public:
				// gets "main" object
				std::shared_ptr<T> get_main();
				// sets "main" object
				void set_main(const std::string);

				// locks internally for begin()/end() stuff
				void lock();
				// unlocks internally so it can accept new stuff back
				void unlock();
				// iterator begin
				auto begin();
				// iterator end
				auto end();

				// renames one object to something else
				bool rename(const std::string, const std::string);
				// set if object should be found normally
				bool set_enabled(const std::string, const bool);

				// renames multiple objects that match function to whatever your function returns based on old name
				size_t rename(const std::function<bool(const std::string)>, const std::function<std::string(const std::string)>);
				// set objects that match to whatever you set based on name in function
				size_t set_enabled(const std::function<bool(const std::string)>, const std::function<bool(const std::string)>);

				// creates internally and returns reference. If there was something there, returns reference
				std::shared_ptr<T> create(const std::string, const bool = true);
				// same as create
				std::shared_ptr<T> load(const std::string, const bool = true);

				// gets smart pointer by id, last arg true if you want to search for everyone (even the disabled ones)
				bool get(const std::string, std::shared_ptr<T>&, const bool = false);

				// remove by string id
				bool remove(const std::string);
				// remove by smart pointer match
				bool remove(const std::shared_ptr<T>);

				// gets a list that matches your function
				std::vector<std::shared_ptr<T>> get_list(const std::function<bool(const std::string)>);

				// swap match with this smart pointer. returns old pointer
				std::shared_ptr<T> swap(const std::string, std::shared_ptr<T>&);

				// clear everything inside
				void clear();
			};


			// gets "main" object
			template<typename T>
			inline std::shared_ptr<T> SuperResource<T>::get_main()
			{
				saves.m.lock();
				auto ret = saves.main_o;
				saves.m.unlock();
			}
			// sets "main" object
			template<typename T>
			inline void SuperResource<T>::set_main(const std::string id) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (i._id == id) {
						saves.main_o = i._data;
						break;
					}
				}
				saves.m.unlock();
			}

			// locks internally for begin()/end() stuff
			template<typename T>
			inline void SuperResource<T>::lock() {
				saves.m.lock();
			}
			// unlocks internally so it can accept new stuff back
			template<typename T>
			inline void SuperResource<T>::unlock() {
				saves.m.unlock();
			}
			// iterator begin
			template<typename T>
			inline auto SuperResource<T>::begin() {
				return saves.objs.begin();
			}
			// iterator end
			template<typename T>
			inline auto SuperResource<T>::end() {
				return saves.objs.end();
			}

			// renames one object to something else
			template<typename T>
			inline bool SuperResource<T>::rename(const std::string id, const std::string new_id) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._id = new_id;
						break;
					}
				}
				saves.m.unlock();
			}
			// set if object should be found normally
			template<typename T>
			inline bool SuperResource<T>::set_enabled(const std::string id, const bool enable) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._enabled = enable;
						break;
					}
				}
				saves.m.unlock();
			}

			// renames multiple objects that match function to whatever your function returns based on old name
			template<typename T>
			inline size_t SuperResource<T>::rename(const std::function<bool(const std::string)> f, const std::function<std::string(const std::string)> s) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (f(i._id)) {
						i._id = s(i._id);
					}
				}
				saves.m.unlock();
			}
			// set objects that match to whatever you set based on name in function
			template<typename T>
			inline size_t SuperResource<T>::set_enabled(const std::function<bool(const std::string)> f, const std::function<bool(const std::string)> s) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (f(i._id)) {
						i._enabled = s(i._id);
					}
				}
				saves.m.unlock();
			}

			// creates internally and returns reference. If there was something there, returns reference
			template<typename T>
			inline std::shared_ptr<T> SuperResource<T>::create(const std::string new_id, const bool enable) {
				std::shared_ptr<T> ret;
				if (get(new_id, ret, true)) return ret;

				internal_data<T> new_data;
				new_data._id = new_id;
				new_data._enabled = enable;
				new_data._data = std::make_shared<T>();

				saves.m.lock();
				saves.objs.push_back(new_data);
				saves.m.unlock();

				return new_data._data;
			}
			// same as create
			template<typename T>
			inline std::shared_ptr<T> SuperResource<T>::load(const std::string new_id, const bool enable) {
				std::shared_ptr<T> ret;
				if (get(new_id, ret, true)) return ret;

				internal_data<T> new_data;
				new_data._id = new_id;
				new_data._enabled = enable;
				new_data._data = std::make_shared<T>();

				saves.m.lock();
				saves.objs.push_back(new_data);
				saves.m.unlock();

				return new_data._data;
			}

			// gets smart pointer by id, last arg true if you want to search for everyone (even the disabled ones)
			template<typename T>
			inline bool SuperResource<T>::get(const std::string id, std::shared_ptr<T>& sptr, const bool also_disabled) {
				bool good = false;
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (i._id == id && (i._enabled || also_disabled)) {
						sptr = i._data;
						good = true;
						break;
					}
				}
				saves.m.unlock();
				return good;
			}

			// remove by string id
			template<typename T>
			inline bool SuperResource<T>::remove(const std::string id) {
				bool good = false;
				saves.m.lock();
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (i._id == id) {
						saves.objs.erase(saves.objs.begin() + p);
						good = true;
						break;
					}
				}
				saves.m.unlock();
				return good;
			}
			// remove by smart pointer match
			template<typename T>
			inline bool SuperResource<T>::remove(const std::shared_ptr<T> sptr) {
				bool good = false;
				saves.m.lock();
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (i._data == sptr) {
						saves.objs.erase(saves.objs.begin() + p);
						good = true;
						break;
					}
				}
				saves.m.unlock();
				return good;
			}

			// gets a list that matches your function
			template<typename T>
			inline std::vector<std::shared_ptr<T>> SuperResource<T>::get_list(const std::function<bool(const std::string)> f) {
				std::vector<std::shared_ptr<T>> lst;
				saves.m.lock();
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (f(i._id)) {
						lst.push_back(i._data);
					}
				}
				saves.m.unlock();
				return lst;
			}

			// swap match with this smart pointer. returns old pointer
			template<typename T>
			inline std::shared_ptr<T> SuperResource<T>::swap(const std::string id, std::shared_ptr<T>& nptr) {
				saves.m.lock();
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._data.swap(nptr);
					}
				}
				saves.m.unlock();
				return nptr;
			}

			// clear everything inside
			template<typename T>
			inline void SuperResource<T>::clear() {
				saves.m.lock();
				saves.objs.clear();
				saves.m.unlock();
			}


			template<typename T> SuperResource<T>::internal_resources<T> SuperResource<T>::saves;
		}
	}
}