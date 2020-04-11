#pragma once

// C
#include <assert.h>
// C++
#include <string>
#include <functional>
#include <vector>
#include <mutex>

// import
#include "..\abort\abort.h"
#include "..\allegroinitialization\allegroinitialization.h"


namespace LSW {
	namespace v5 {

		template<typename T>
		class SuperResource {
			template<typename S> struct _d {
				S* self = nullptr;
				std::string id;
				bool enabled = true;
			};
			template<typename Q> struct _i {
				std::function<bool(std::string&, Q*&)>	load; // cast void if different
				std::function<void(Q*&)>				unload;
				std::vector< _d<Q>* >					db;
				std::mutex                              dbm;
				bool                                    dbmv = false; // verifier
			};

			static _i<T> data;
			bool is_this_autodie = false;
		public:
			~SuperResource() { if (is_this_autodie) clear(); }

			void autodie(bool die = true) {
				is_this_autodie = die;
			}

			void set(const std::function <bool(std::string&, T*&)> hl, const std::function <void(T*&)> hd) {
				assert(hl);
				assert(hd);
				data.load = hl;
				data.unload = hd;
			}
			bool tryLock() {
				return (data.dbmv = data.dbm.try_lock());
			}
			size_t size() {
				return data.db.size();
			}
			auto begin() {
				return data.db.begin();
			}
			auto end() {
				return data.db.end();
			}
			void unlock() {
				if (!data.dbmv) throw Abort::Abort(__FUNCSIG__, "MUTEX UNLOCKED WHEN TRYING TO UNLOCK!", Abort::abort_level::FATAL_ERROR);
				else data.dbm.unlock();
				data.dbmv = false;
			}
			bool rename(const std::string id, const std::string newid) {
				data.dbm.lock();
				for (auto& i : data.db) {
					if (i->id == id) {
						i->id = newid;
						data.dbm.unlock();
						return true;
					}
				}
				data.dbm.unlock();
				return false;
			}
			bool setEnabled(const std::string id, const bool val) {
				data.dbm.lock();
				for (auto& i : data.db) {
					if (i->id == id) {
						i->enabled = val;
						data.dbm.unlock();
						return true;
					}
				}
				data.dbm.unlock();
				return false;
			}
			size_t setEnabled(const std::function<bool(const std::string)> f, const std::function<bool(const std::string)> nf) {
				size_t __ic = 0;
				data.dbm.lock();
				for (auto& i : data.db) {
					if (f(i->id)) {
						i->enabled = nf(i->id);
						__ic++;
					}
				}
				data.dbm.unlock();
				return __ic;
			}
			size_t rename(const std::function<bool(const std::string)> f, const std::function<std::string(const std::string)> nf)
			{
				size_t __ic = 0;
				data.dbm.lock();
				for (auto& i : data.db) {
					if (f(i->id)) {
						i->id = nf(i->id);
						__ic++;
					}
				}
				data.dbm.unlock();
				return __ic;
			}
			T* customLoad(const std::string id, std::function<bool(T*&)> f) {
				T* b = nullptr;
				bool r = get(id, b, true);

				if (!r) {
					_d<T>* dt = new _d<T>();
					dt->id = id;

					r = f(dt->self);
					if (!r || !dt->self) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);
					b = dt->self;

					data.dbm.lock();
					data.db.push_back(dt);
					data.dbm.unlock();
				}
				return b;
			}
			T* create(std::string id, std::string path = "") { return load(id, path); }
			T* load(std::string id, std::string path = "") {
				if (!data.load) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);

				T* b = nullptr;
				bool r = get(id, b, true);

				if (!r) {
					_d<T>* dt = new _d<T>();
					dt->id = id;

					if (path.length() > 0) r = data.load(path, dt->self);
					else r = data.load(dt->id, dt->self);

					if (!r || !dt->self) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id + ";path=" + path, Abort::abort_level::FATAL_ERROR);
					b = dt->self;

					data.dbm.lock();
					data.db.push_back(dt);
					data.dbm.unlock();
				}
				return b;
			}
			void create(std::vector<std::string> n, std::vector<std::string> q) { load(n, q); }
			void load(std::vector<std::string> n, std::vector<std::string> q, float* perc = nullptr)
			{
				assert(n.size() == q.size());

				for (size_t p = 0; p < n.size(); p++)
				{
					if (perc) *perc = 1.0f * p / n.size();

					load(n[p], q[p]);
				}
				if (perc) *perc = 1.00;
			}
			bool get(const std::string id, T*& p, const bool force_enabled = false) {
				data.dbm.lock();
				for (auto& i : data.db) {
					if (i->id == id && (i->enabled || force_enabled)) {
						p = i->self;
						data.dbm.unlock();
						return true;
					}
				}
				data.dbm.unlock();
				return false;
			}
			bool remove(const std::string id) {
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);

				data.dbm.lock();
				for (size_t p = 0; p < data.db.size(); p++)
				{
					if (data.db[p]->id == id) {
						data.unload(data.db[p]->self);
						data.db.erase(data.db.begin() + p--);
						data.dbm.unlock();
						return true;
					}
				}
				data.dbm.unlock();
				return false;
			}
			size_t remove(const std::function<bool(const std::string)> f)
			{
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);

				size_t __ic = 0;
				data.dbm.lock();
				for (size_t p = 0; p < data.db.size(); p++)
				{
					if (f(data.db[p]->id)) {
						data.unload(data.db[p]->self);
						data.db.erase(data.db.begin() + p--);
						__ic++;
					}
				}
				data.dbm.unlock();
				return __ic;
			}
			std::vector<T*> getList(const std::function<bool(const std::string)> f)
			{
				std::vector<T*> lst;
				for (size_t p = 0; p < data.db.size(); p++)
				{
					if (f(data.db[p]->id)) {
						lst.push_back(data.db[p]->self);
					}
				}
				return lst;
			}
			bool swap(const std::string id, T*& p) {
				data.dbm.lock();
				for (auto& i : data.db) {
					if (i->id == id) {
						T*& c = p;
						p = i->self;
						i->self = c;
						data.dbm.unlock();
						return true;
					}
				}
				data.dbm.unlock();
				return false;
			}
			void clear() {
				if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
				data.dbm.lock();
				for (auto& i : data.db)
				{
					data.unload(i->self);
				}
				data.db.clear();
				data.dbm.unlock();
			}
		};

		/* PRETTY USEFUL LAMBDA STUFF FOR BIG_TEMPLATES */
		template<typename H> const auto lambda_null_load = [](std::string& p, H*& r) -> bool { return false; };
		template<typename H> const auto lambda_null_unload = [](H*& b) -> void { b = nullptr;  return; };


		template<typename K> const auto lambda_default_load = [](std::string& p, K*& r) -> bool { return (r = new K()); };
		template<typename K> const auto lambda_default_unload = [](K*& b) -> void { if (b) delete b; b = nullptr; };


		inline const auto lambda_bitmap_load = [](std::string& p, ALLEGRO_BITMAP*& b) -> bool { return ((b = al_load_bitmap(p.c_str()))); };
		inline const auto lambda_bitmap_unload = [](ALLEGRO_BITMAP*& b) -> void {if (al_is_system_installed() && b) { al_destroy_bitmap(b); b = nullptr; } };

		inline const auto lambda_font_load = [](std::string& p, ALLEGRO_FONT*& b) -> bool { return ((b = al_load_ttf_font(p.c_str(), 25e2, 0))); };
		inline const auto lambda_font_unload = [](ALLEGRO_FONT*& b) -> void { if (al_is_system_installed() && b) { al_destroy_font(b); b = nullptr; } };

		inline const auto lambda_sample_load = [](std::string& p, ALLEGRO_SAMPLE*& b) -> bool { return ((b = al_load_sample(p.c_str()))); };
		inline const auto lambda_sample_unload = [](ALLEGRO_SAMPLE*& b) -> void { if (al_is_system_installed() && b) { al_destroy_sample(b); b = nullptr; } };


		template<typename T> SuperResource<T>::_i<T> SuperResource<T>::data = { Constants::lambda_default_load<T>, Constants::lambda_default_unload<T> };

		// type specific
		template <> SuperResource<ALLEGRO_BITMAP>::_i<ALLEGRO_BITMAP>	SuperResource<ALLEGRO_BITMAP>::data = { lambda_bitmap_load, lambda_bitmap_unload };
		template <> SuperResource<ALLEGRO_FONT>::_i<ALLEGRO_FONT>		SuperResource<ALLEGRO_FONT>::data = { lambda_font_load, lambda_font_unload };
		template <> SuperResource<ALLEGRO_SAMPLE>::_i<ALLEGRO_SAMPLE>	SuperResource<ALLEGRO_SAMPLE>::data = { lambda_sample_load, lambda_sample_unload };


	}
}