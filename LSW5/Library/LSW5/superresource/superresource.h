#pragma once

// C
#define _CRT_RAND_S
#include <assert.h>
// C++
#include <string>
#include <functional>
#include <vector>
#include <mutex>
// Others
#include "..\abort\abort.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\supermutex\supermutex.h"


namespace LSW {
	namespace v5 {

		template<typename Q>
		class generic_class {
			std::shared_ptr<Q> _data;
			std::string id;
			bool enabled = true;
			std::function<bool(std::string&, Q*&)> load;
			std::function<void(Q*&)> unload;
		public:
			generic_class(std::function<bool(std::string&, Q*&)>& l, std::function<void(Q*&)>& u, const std::string& nid, std::string str) {
				if (!l) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! Load function is null!");
				if (!u) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! Unoad function is null!");
				load = l;
				unload = u;
				id = nid;
				if (!create(str)) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! (create returned false)");
			}
			~generic_class() {
				destroy();
			}

			// path, id
			bool create(std::string& str) {
				if (_data) throw Abort::Abort(__FUNCSIG__, "Exception! Create called two times!");
				Q* aaa = nullptr;
				auto fin = load(str, aaa);
				auto cpy = unload; // explicit copy
				_data = std::shared_ptr<Q>(aaa, [cpy](Q* q) {
					Q* aaa = q;
					cpy(aaa);
					});
				return fin;
			}
			void destroy() {
				if (!_data) throw Abort::Abort(__FUNCSIG__, "Exception! Create called two times!");
				/*if (_data.use_count() == 1) {
					Q* aaa = &(*_data);
					unload(aaa);
				}*/
				_data.reset();
			}
			auto& data() {
				return _data;
			}
			const auto& data() const {
				return _data;
			}
			auto operator*() {
				return *_data;
			}
			const auto operator*() const {
				return *_data;
			}
			Q* operator->() {
				return _data.operator->();
			}
			const Q* operator->() const {
				return _data.operator->();
			}
			std::string& getID() {
				return id;
			}
			const std::string& getID() const {
				return id;
			}
			bool& getEnabled() {
				return enabled;
			}
			const bool& getEnabled() const {
				return enabled;
			}

			bool operator==(const generic_class& oth) const {
				return this->id == oth.getID();
			}
			bool operator==(const void* data) const {
				return &(*this->data()) == data; // see later
			}
			bool operator==(const std::string& str) const {
				return this->id == str;
			}
		};

		template<typename T>
		class SuperResource {

			template<typename Q>
			struct _data {
				std::function<bool(std::string&, Q*&)> load;
				std::function<void(Q*&)> unload;

				LSW::v5::SuperMutex m;
				std::vector<generic_class<Q>> vec;
				std::shared_ptr<Q> main_o; // useful for "priority" stuff like the main Mixer, main Camera and stuff like that

				_data(std::function<bool(std::string&, Q*&)> a, std::function<void(Q*&)> b) { load = a; unload = b; }
			};

			static _data<T> data;
			bool is_this_autodie = false;
		public:
			~SuperResource() { if (is_this_autodie) clear(); }

			// gets the main resource (the one set to be main via setMain)
			std::shared_ptr<T> getMain() {
				return data.main_o;
			}
			/* set a particular resource as main resource (good for main Mixer, Camera, and stuff)
			   WARN: it won't automatically switch if you delete the same from SuperResource and replace with other one.
			   WARN: if it is deleted from SuperResource, it might not get deleted until cleanMain() or a new setMain() is called. (no worries about invalid pointer)*/
			bool setMain(const std::string id) {
				for (auto& i : data.vec) {
					if (i == id) {
						data.main_o = i.data();
						return true;
					}
				}
				return false;
			}
			// removes any main stuff
			void cleanMain() {
				data.main_o.reset();
			}

			void autodie(bool die = true) {
				is_this_autodie = die;
			}
			void lock() {
				data.m.lock();
			}
			size_t size() {
				return data.vec.size();
			}
			auto begin() {
				return data.vec.begin();
			}
			auto end() {
				return data.vec.end();
			}
			void unlock() {
				data.m.unlock();
			}
			bool rename(const std::string id, const std::string newid) { // GOOD
				lock();
				for (auto& i : data.vec) {
					if (i == id) {
						i.getID() = newid;
						unlock();
						return true;
					}
				}
				unlock();
				return false;
			}
			bool setEnabled(const std::string id, const bool val) { // GOOD
				lock();
				for (auto& i : data.vec) {
					if (i == id) {
						i.getEnabled() = val;
						unlock();
						return true;
					}
				}
				unlock();
				return false;
			}
			size_t rename(const std::function<bool(const std::string)> f, const std::function<std::string(const std::string)> nf) // GOOD
			{
				size_t __ic = 0;
				lock();
				for (auto& i : data.vec) {
					if (f(i.getID())) {
						i.getID() = nf(i.getID());
						__ic++;
					}
				}
				unlock();
				return __ic;
			}
			size_t setEnabled(const std::function<bool(const std::string)> f, const std::function<bool(const std::string)> nf) { // GOOD
				size_t __ic = 0;
				lock();
				for (auto& i : data.vec) {
					if (f(i.getID())) {
						i.getEnabled() = nf(i.getID());
						__ic++;
					}
				}
				unlock();
				return __ic;
			}
			std::shared_ptr<T> customLoad(const std::string id, std::function<bool(T*&)> f) { // GOOD
				std::shared_ptr<T> ret;
				bool r = get(id, ret, true);

				if (!r) {
					//ret = std::make_shared<generic_class<T>>(data.load, data.unload, id);
					std::function<bool(std::string&, T*&)> _f = [=](std::string& a, T*& v)->bool { return f(v); };
					generic_class<T> ret2 = generic_class<T>(_f, data.unload, id, "");

					if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

					data.m.lock();
					data.vec.push_back(ret2);
					data.m.unlock();

					ret = ret2.data();
				}
				return ret;
			}
			std::shared_ptr<T> create(std::string id, std::string path = "") { return load(id, path); } // GOOD
			std::shared_ptr<T> load(std::string id, std::string path = "") { // GOOD
				if (!data.load) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);

				std::shared_ptr<T> ret;
				bool r = get(id, ret, true);

				if (!r) {
					generic_class<T> ret2 = generic_class<T>(data.load, data.unload, id, path);

					if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

					data.m.lock();
					data.vec.push_back(ret2);
					data.m.unlock();

					ret = ret2.data();
				}
				return ret;
			}
			void create(std::vector<std::string> n, std::vector<std::string> q) { load(n, q); } // GOOD
			void load(std::vector<std::string> n, std::vector<std::string> q, float* perc = nullptr) // GOOD
			{
				assert(n.size() == q.size());

				for (size_t p = 0; p < n.size(); p++)
				{
					if (perc) *perc = 1.0f * p / n.size();

					load(n[p], q[p]);
				}
				if (perc) *perc = 1.00;
			}
			bool get(const std::string id, std::shared_ptr<T>& ret, const bool force_enabled = false) { // GOOD
				data.m.lock();
				for (auto& i : data.vec) {
					if (i == id && (i.getEnabled() || force_enabled)) {
						ret = i.data();
						data.m.unlock();
						return true;
					}
				}
				data.m.unlock();
				return false;
			}
			bool remove(const std::string id) {
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
				data.m.lock();
				for (size_t p = 0; p < data.vec.size(); p++) {
					auto& i = data.vec[p];
					if (i == id) {
						data.vec.erase(data.vec.begin() + p);
						data.m.unlock();
						return true;
					}
				}
				data.m.unlock();
				return false;
			}
			size_t remove(const std::function<bool(const std::string)> f)
			{
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
				data.m.lock();

				size_t __ic = 0;

				for (size_t p = 0; p < data.vec.size(); p++) {
					auto& i = data.vec[p];
					if (f(i.getID())) {
						data.vec.erase(data.vec.begin() + p);
						__ic++;
					}
				}
				data.m.unlock();
				return __ic;
			}
			std::vector<std::shared_ptr<T>> getList(const std::function<bool(const std::string)> f)
			{
				std::vector<std::shared_ptr<T>> lst;
				for (size_t p = 0; p < data.vec.size(); p++)
				{
					auto& i = data.vec[p];
					if (f(i.getID())) {
						lst.push_back(i.data());
					}
				}
				return lst;
			}
			// swap ptrs
			std::shared_ptr<T> swap(const std::string id, std::shared_ptr<T>& p) {
				data.m.lock();
				for (auto& i : data.vec) {
					if (i == id) {
						i.data().swap(p);
						data.m.unlock();
						return p;
					}
				}
				data.m.unlock();
				return std::shared_ptr<T>();
			}
			// creates new and replace/reset
			std::shared_ptr<T> swap(const std::string id, std::string path = "") {
				data.m.lock();
				for (auto& i : data.vec) {
					if (i == id) {
						generic_class<T> ret2 = generic_class<T>(data.load, data.unload, id, path);

						if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

						i.data().swap(ret2.data());
						data.m.unlock();
						return i.data();
					}
				}
				data.m.unlock();
				return std::shared_ptr<T>();
			}
			// creates new and replace/reset
			std::shared_ptr<T> swapCustomLoad(const std::string id, std::function<bool(T*&)> f) {
				data.m.lock();
				for (auto& i : data.vec) {
					if (i == id) {
						std::function<bool(std::string&, T*&)> _f = [=](std::string& a, T*& v)->bool { return f(v); };
						generic_class<T> ret2 = generic_class<T>(_f, data.unload, id, "");

						if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

						i.data().swap(ret2.data());
						data.m.unlock();
						return i.data();
					}
				}
				data.m.unlock();
				return std::shared_ptr<T>();
			}
			void clear() {
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
				data.m.lock();
				data.vec.clear();
				data.m.unlock();
			}
		};

		/* PRETTY USEFUL LAMBDA STUFF FOR BIG_TEMPLATES */
		template<typename H> const auto lambda_null_load = [](std::string& p, H*& r) -> bool { return false; };
		template<typename H> const auto lambda_null_unload = [](H*& b) -> void { b = nullptr;  return; };


		template<typename K> const auto lambda_default_load = [](std::string& p, K*& r) -> bool { return (r = new K()); };
		template<typename K> const auto lambda_default_unload = [](K*& b) -> void { if (b) delete b; b = nullptr; };


		inline const auto lambda_bitmap_load = [](std::string& p, ALLEGRO_BITMAP*& b) -> bool { return ((b = al_load_bitmap(p.c_str()))); };
		inline const auto lambda_bitmap_unload = [](ALLEGRO_BITMAP*& b) -> void {if (al_is_system_installed() && b) { al_destroy_bitmap(b); b = nullptr; } };

		inline const auto lambda_font_load = [](std::string& p, ALLEGRO_FONT*& b) -> bool { return ((b = al_load_ttf_font(p.c_str(), 75.0, 0))); };
		inline const auto lambda_font_unload = [](ALLEGRO_FONT*& b) -> void { if (al_is_system_installed() && b) { al_destroy_font(b); b = nullptr; } };

		inline const auto lambda_sample_load = [](std::string& p, ALLEGRO_SAMPLE*& b) -> bool { return ((b = al_load_sample(p.c_str()))); };
		inline const auto lambda_sample_unload = [](ALLEGRO_SAMPLE*& b) -> void { if (al_is_system_installed() && b) { al_destroy_sample(b); b = nullptr; } };


		template<typename T> SuperResource<T>::_data<T> SuperResource<T>::data = { lambda_default_load<T>, lambda_default_unload<T> };

		// type specific
		template <> SuperResource<ALLEGRO_BITMAP>::_data<ALLEGRO_BITMAP>	SuperResource<ALLEGRO_BITMAP>::data = { lambda_bitmap_load, lambda_bitmap_unload };
		template <> SuperResource<ALLEGRO_FONT>::_data<ALLEGRO_FONT>		SuperResource<ALLEGRO_FONT>::data = { lambda_font_load, lambda_font_unload };
		template <> SuperResource<ALLEGRO_SAMPLE>::_data<ALLEGRO_SAMPLE>	SuperResource<ALLEGRO_SAMPLE>::data = { lambda_sample_load, lambda_sample_unload };


	}
}