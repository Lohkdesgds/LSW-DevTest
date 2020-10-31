#pragma once

// C++
#include <string>
#include <functional>
#include <vector>
#include <mutex>

#include "../../Handling/Abort/abort.h"
#include "../SuperMutex/supermutex.h"


namespace LSW {
	namespace v5 {
		namespace Tools {

			template<typename T>
			class Resource;


			/// <summary>
			/// <para>SuperResource is a one-per-type global resource manager.</para>
			/// <para></para>
			/// </summary>
			template<typename T>
			class SuperResource {

				template<typename Q>
				struct internal_data {
					Resource<Q> _data;
					std::string _id;
					bool _enabled = true;
				};

				template<typename Q>
				struct internal_resources {
					SuperMutex m;
					std::vector<internal_data<Q>> objs;
					Resource<Q> main_o;
				};

				static internal_resources<T> saves;
			public:
				/// <summary>
				/// <para>Gets a object you've set as main (like an atlas or default font).</para>
				/// </summary>
				/// <returns>{Resource} The shared_ptr of the main object.</returns>
				Resource<T> get_main();

				/// <summary>
				/// <para>Sets a object in the SuperResource already as main. (it will still exist if you clear the SuperResource)</para>
				/// </summary>
				/// <param name="{std::string}">Resource's ID.</param>
				/// <returns>{bool} True if found and set.</returns>
				bool set_main(const std::string);

				/// <summary>
				/// <para>Locks internal mutex so you can use begin() and end() (or a for auto).</para>
				/// </summary>
				void lock();

				/// <summary>
				/// <para>Unlocks internal mutex after your use of begin() and end() (or for auto).</para>
				/// </summary>
				void unlock();

				/// <summary>
				/// <para>Begin of the vector.</para>
				/// </summary>
				/// <returns>{iterator} The begin of the vector.</returns>
				auto begin();

				/// <summary>
				/// <para>End of the vector.</para>
				/// </summary>
				/// <returns>{iterator} The end of the vector.</returns>
				auto end();

				/// <summary>
				/// <para>Renames a object X to something else.</para>
				/// </summary>
				/// <param name="{std::string}">The ID right now.</param>
				/// <param name="{std::string}">The new ID.</param>
				/// <returns>{bool} True if found and set.</returns>
				bool rename(const std::string, const std::string);

				/// <summary>
				/// <para>Renames a object as enabled or disabled (filter).</para>
				/// </summary>
				/// <param name="{std::string}">The ID.</param>
				/// <param name="{bool}">Enabled?</param>
				/// <returns>{bool} True if found and set.</returns>
				bool set_enabled(const std::string, const bool);

				/// <summary>
				/// <para>Renames objects based on a formula to a function that generates strings based on old name.</para>
				/// </summary>
				/// <param name="{std::function}">The function matcher.</param>
				/// <param name="{std::function}">The new ID generator based on old string.</param>
				/// <returns>{size_t} Amount of objects changed.</returns>
				size_t rename(const std::function<bool(const std::string)>, const std::function<std::string(const std::string)>);

				/// <summary>
				/// <para>Sets objects enabled or not based on formulas.</para>
				/// </summary>
				/// <param name="{std::function}">The function matcher.</param>
				/// <param name="{std::function}">The function to set if it's enabled or disabled by ID.</param>
				/// <returns>{size_t} Amount of objects changed.</returns>
				size_t set_enabled(const std::function<bool(const std::string)>, const std::function<bool(const std::string)>);

				/// <summary>
				/// <para>Creates the object with ID internally that can be accessed anywhere.</para>
				/// </summary>
				/// <param name="{std::string}">The new object ID.</param>
				/// <param name="{bool}">Is is enabled?</param>
				/// <returns>{Resource} The object created.</returns>
				Resource<T> create(const std::string, const bool = true);

				/// <summary>
				/// <para>Creates the object with ID internally that can be accessed anywhere. (same as create)</para>
				/// </summary>
				/// <param name="{std::string}">The new object ID.</param>
				/// <param name="{bool}">Is is enabled?</param>
				/// <returns>{Resource} The object created.</returns>
				Resource<T> load(const std::string, const bool = true);

				/// <summary>
				/// <para>Gets a object using ID.</para>
				/// </summary>
				/// <param name="{std::string}">The object ID.</param>
				/// <param name="{shared_ptr}">The shared_ptr reference to hold the object.</param>
				/// <param name="{bool}">Search for disabled objects too?</param>
				/// <returns>{bool} True if found.</returns>
				bool get(const std::string, Resource<T>&, const bool = false);

				/// <summary>
				/// <para>Remove a object by ID.</para>
				/// </summary>
				/// <param name="{std::string}">The object ID.</param>
				/// <returns>{bool} True if found and removed.</returns>
				bool remove(const std::string);

				/// <summary>
				/// <para>Remove a object by its own shared_ptr value.</para>
				/// </summary>
				/// <param name="{Resource}">The object itself.</param>
				/// <returns>{bool} True if found and removed.</returns>
				bool remove(const Resource<T>);

				/// <summary>
				/// <para>Gets a list of objects using a function to filter them.</para>
				/// </summary>
				/// <param name="{std::function}">A function to filter what you want.</param>
				/// <returns>{std::vector} A vector of those objects.</returns>
				std::vector<Resource<T>> get_list(const std::function<bool(const std::string)>);

				/// <summary>
				/// <para>Swaps a object with another at the position this ID is found.</para>
				/// </summary>
				/// <param name="{std::string}">Object ID to replace.</param>
				/// <param name="{Resource}">The one that will be swapped.</param>
				/// <returns>{Resource} The swapped object that is not there anymore. (or itself if not found)</returns>
				Resource<T> swap(const std::string, Resource<T>&);

				/// <summary>
				/// <para>Clears the whole internal vector.</para>
				/// </summary>
				void clear();
			};



			template<typename T>
			class Resource {
				std::shared_ptr<T> r;
			public:
				Resource() = default;
				Resource(const Resource&);
				Resource(const std::shared_ptr<T>&);
				Resource(Resource&&);
				Resource(std::shared_ptr<T>&&);

				bool populate(const std::string&, const bool = false);
				bool generate(const std::string&, const bool = false);

				void operator=(const Resource&);
				void operator=(const std::shared_ptr<T>&);
				void operator=(Resource&&);
				void operator=(std::shared_ptr<T>&&);

				bool empty() const;

				T* operator->();
				T* operator->() const;

				T& operator*();
				T& operator*() const;

				bool operator!() const;

				bool operator==(const Resource&) const;

				operator std::shared_ptr<T>();
				operator const T&() const;

				T* get();
				const T* get() const;

				void swap(Resource&);

				std::shared_ptr<T>& get_shared();
			};






			template<typename T>
			inline Resource<T> SuperResource<T>::get_main()
			{
				Tools::AutoLock luck(saves.m);
				return saves.main_o;
			}

			template<typename T>
			inline bool SuperResource<T>::set_main(const std::string id) {
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (i._id == id) {
						saves.main_o = i._data;
						return true;
					}
				}
				return false;
			}

			template<typename T>
			inline void SuperResource<T>::lock() {
				saves.m.lock();
			}
			
			template<typename T>
			inline void SuperResource<T>::unlock() {
				saves.m.unlock();
			}

			template<typename T>
			inline auto SuperResource<T>::begin() {
				return saves.objs.begin();
			}
			
			template<typename T>
			inline auto SuperResource<T>::end() {
				return saves.objs.end();
			}

			template<typename T>
			inline bool SuperResource<T>::rename(const std::string id, const std::string new_id) {
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._id = new_id;
						return true;
					}
				}
				return false;
			}
			
			template<typename T>
			inline bool SuperResource<T>::set_enabled(const std::string id, const bool enable) {
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._enabled = enable;
						return true;
					}
				}
				return false;
			}

			template<typename T>
			inline size_t SuperResource<T>::rename(const std::function<bool(const std::string)> f, const std::function<std::string(const std::string)> s) {
				size_t amount = 0;
				AutoLock luck(saves.m);

				for (auto& i : saves.objs) {
					if (f(i._id)) {
						i._id = s(i._id);
						amount++;
					}
				}
				return amount;
			}
			
			template<typename T>
			inline size_t SuperResource<T>::set_enabled(const std::function<bool(const std::string)> f, const std::function<bool(const std::string)> s) {
				size_t amount = 0;
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (f(i._id)) {
						i._enabled = s(i._id);
						amount++;
					}
				}
				return amount;
			}

			template<typename T>
			inline Resource<T> SuperResource<T>::create(const std::string new_id, const bool enable) {
				Resource<T> ret;
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
			
			template<typename T>
			inline Resource<T> SuperResource<T>::load(const std::string new_id, const bool enable) {
				Resource<T> ret;
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

			template<typename T>
			inline bool SuperResource<T>::get(const std::string id, Resource<T>& sptr, const bool also_disabled) {
				bool good = false;
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (i._id == id && (i._enabled || also_disabled)) {
						sptr = i._data;
						good = true;
						break;
					}
				}
				return good;
			}

			template<typename T>
			inline bool SuperResource<T>::remove(const std::string id) {
				bool good = false;
				AutoLock luck(saves.m);
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (i._id == id) {
						saves.objs.erase(saves.objs.begin() + p);
						good = true;
						break;
					}
				}
				return good;
			}
			
			template<typename T>
			inline bool SuperResource<T>::remove(const Resource<T> sptr) {
				bool good = false;
				AutoLock luck(saves.m);
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (i._data == sptr) {
						saves.objs.erase(saves.objs.begin() + p);
						good = true;
						break;
					}
				}
				return good;
			}

			template<typename T>
			inline std::vector<Resource<T>> SuperResource<T>::get_list(const std::function<bool(const std::string)> f) {
				std::vector<Resource<T>> lst;
				AutoLock luck(saves.m);
				for (size_t p = 0; p < saves.objs.size(); p++) {
					auto& i = saves.objs[p];
					if (f(i._id)) {
						lst.push_back(i._data);
					}
				}
				return lst;
			}

			template<typename T>
			inline Resource<T> SuperResource<T>::swap(const std::string id, Resource<T>& nptr) {
				AutoLock luck(saves.m);
				for (auto& i : saves.objs) {
					if (i._id == id) {
						i._data.swap(nptr);
					}
				}
				return nptr;
			}

			template<typename T>
			inline void SuperResource<T>::clear() {
				AutoLock luck(saves.m);
				saves.objs.clear();
			}




			template<typename T>
			inline Resource<T>::Resource(const Resource& a)
			{
				*this = a;
			}

			template<typename T>
			inline Resource<T>::Resource(const std::shared_ptr<T>& a)
			{
				*this = a;
			}

			template<typename T>
			inline Resource<T>::Resource(Resource&& a)
			{
				*this = std::move(a);
			}

			template<typename T>
			inline Resource<T>::Resource(std::shared_ptr<T>&& a)
			{
				*this = std::move(a);
			}

			template<typename T>
			inline bool Resource<T>::populate(const std::string& w, const bool b)
			{
				SuperResource<T> src;
				Resource aa;
				if (src.get(w, aa, b)) {
					*this = std::move(aa);
					return r.get();
				}
				return false;
			}

			template<typename T>
			inline bool Resource<T>::generate(const std::string& w, const bool b)
			{
				SuperResource<T> src;
				*this = std::move(src.create(w, b));
				return r.get();
			}

			template<typename T>
			inline void Resource<T>::operator=(const Resource& a)
			{
				r = a.r;
			}

			template<typename T>
			inline void Resource<T>::operator=(const std::shared_ptr<T>& a)
			{
				r = a;
			}

			template<typename T>
			inline void Resource<T>::operator=(Resource&& a)
			{
				r = std::move(a.r);
			}

			template<typename T>
			inline void Resource<T>::operator=(std::shared_ptr<T>&& a)
			{
				r = std::move(a);
			}

			template<typename T>
			inline bool Resource<T>::empty() const
			{
				return this && r.get();
			}

			template<typename T>
			inline T* Resource<T>::operator->()
			{
				return r.get();
			}

			template<typename T>
			inline T* Resource<T>::operator->() const
			{
				return r.get();
			}

			template<typename T>
			inline T& Resource<T>::operator*()
			{
				return *r;
			}

			template<typename T>
			inline T& Resource<T>::operator*() const
			{
				return *r;
			}

			template<typename T>
			inline bool Resource<T>::operator!() const
			{
				return !empty();
			}

			template<typename T>
			inline bool Resource<T>::operator==(const Resource& o) const
			{
				return r == o.r;
			}

			template<typename T>
			inline Resource<T>::operator std::shared_ptr<T>()
			{
				return r;
			}

			template<typename T>
			inline Resource<T>::operator const T& () const
			{
				return *r.get();
			}

			template<typename T>
			inline T* Resource<T>::get()
			{
				return r.get();
			}

			template<typename T>
			inline const T* Resource<T>::get() const
			{
				return r.get();
			}

			template<typename T>
			inline void Resource<T>::swap(Resource& o)
			{
				r.swap(o.r);
			}

			template<typename T>
			inline std::shared_ptr<T>& Resource<T>::get_shared()
			{
				return r;
			}



			template<typename T> SuperResource<T>::internal_resources<T> SuperResource<T>::saves;
		}
	}
}