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
			std::function<bool(Q*&)> load;
			std::function<void(Q*&)> unload;
		public:
			generic_class(std::function<bool(Q*&)>& l, std::function<void(Q*&)>& u, const std::string& nid) {
				if (!l) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! Load function is null!");
				if (!u) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! Unoad function is null!");
				load = l;
				unload = u;
				id = nid;
				if (!create()) throw Abort::Abort(__FUNCSIG__, "Cannot create generic_class! (create returned false)");
			}
			~generic_class() {
				destroy();
			}

			// path, id
			bool create() {
				if (_data) throw Abort::Abort(__FUNCSIG__, "Exception! Create called two times!");
				Q* aaa = nullptr;
				auto fin = load(aaa);
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
				std::function<bool(Q*&)> load;
				std::function<void(Q*&)> unload;

				LSW::v5::SuperMutex m;
				std::vector<generic_class<Q>> vec;
				std::shared_ptr<Q> main_o; // useful for "priority" stuff like the main Mixer, main Camera and stuff like that

				_data(std::function<bool(Q*&)> a, std::function<void(Q*&)> b) { load = a; unload = b; }
			};

			static _data<T> data;
			bool is_this_autodie = false;
		public:
			~SuperResource();

			// gets the main resource (the one set to be main via setMain)
			std::shared_ptr<T> getMain();

			/* set a particular resource as main resource (good for main Mixer, Camera, and stuff)
			   WARN: it won't automatically switch if you delete the same from SuperResource and replace with other one.
			   WARN: if it is deleted from SuperResource, it might not get deleted until cleanMain() or a new setMain() is called. (no worries about invalid pointer)*/
			bool setMain(const std::string);

			// removes any main stuff
			void cleanMain();

			void autodie(bool = true);

			void lock();

			size_t size();

			auto begin();

			auto end();

			void unlock();

			bool rename(const std::string, const std::string);

			bool setEnabled(const std::string, const bool);

			size_t rename(const std::function<bool(const std::string)>, const std::function<std::string(const std::string)>);

			size_t setEnabled(const std::function<bool(const std::string)>, const std::function<bool(const std::string)>);

			std::shared_ptr<T> customLoad(const std::string, std::function<bool(T*&)>);

			std::shared_ptr<T> create(std::string);

			std::shared_ptr<T> load(std::string);

			void create(std::vector<std::string>);

			void load(std::vector<std::string>, float* = nullptr);

			bool get(const std::string, std::shared_ptr<T>&, const bool = false);

			bool remove(const std::string);

			bool remove(const std::shared_ptr<T>);

			size_t remove(const std::function<bool(const std::string)>);

			std::vector<std::shared_ptr<T>> getList(const std::function<bool(const std::string)>);

			// swap ptrs
			std::shared_ptr<T> swap(const std::string, std::shared_ptr<T>&);

			// creates new and replace/reset
			std::shared_ptr<T> swap(const std::string);

			// creates new and replace/reset (if none there, creates one)
			std::shared_ptr<T> swapCustomLoad(const std::string, std::function<bool(T*&)>);

			// creates new and replace/reset (if none there, creates one)
			std::shared_ptr<T> swapNew(const std::string);

			void clear();
		};




		template<typename T>
		inline SuperResource<T>::~SuperResource()
		{
			if (is_this_autodie) clear();
		}

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::getMain()
		{
			return data.main_o;
		}

		template<typename T>
		inline bool SuperResource<T>::setMain(const std::string id)
		{
			for (auto& i : data.vec) {
				if (i == id) {
					data.main_o = i.data();
					return true;
				}
			}
			return false;
		}

		template<typename T>
		inline void SuperResource<T>::cleanMain()
		{
			data.main_o.reset();
		}

		template<typename T>
		inline void SuperResource<T>::autodie(bool die) {
			is_this_autodie = die;
		}

		template<typename T>
		inline void SuperResource<T>::lock() {
			data.m.lock();
		}

		template<typename T>
		inline size_t SuperResource<T>::size() {
			return data.vec.size();
		}

		template<typename T>
		inline auto SuperResource<T>::begin() {
			return data.vec.begin();
		}

		template<typename T>
		inline auto SuperResource<T>::end() {
			return data.vec.end();
		}

		template<typename T>
		inline void SuperResource<T>::unlock() {
			data.m.unlock();
		}

		template<typename T>
		inline bool SuperResource<T>::rename(const std::string id, const std::string newid) { // GOOD
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

		template<typename T>
		inline bool SuperResource<T>::setEnabled(const std::string id, const bool val) { // GOOD
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

		template<typename T>
		inline size_t SuperResource<T>::rename(const std::function<bool(const std::string)> f, const std::function<std::string(const std::string)> nf) // GOOD
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

		template<typename T>
		inline size_t SuperResource<T>::setEnabled(const std::function<bool(const std::string)> f, const std::function<bool(const std::string)> nf) { // GOOD
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

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::customLoad(const std::string id, std::function<bool(T*&)> f) {
			std::shared_ptr<T> ret;
			bool r = get(id, ret, true);

			if (!r) {
				//ret = std::make_shared<generic_class<T>>(data.load, data.unload, id);
				std::function<bool( T*&)> _f = [=](T*& v)->bool { return f(v); };
				generic_class<T> ret2 = generic_class<T>(_f, data.unload, id);

				if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

				data.m.lock();
				data.vec.push_back(ret2);
				data.m.unlock();

				ret = ret2.data();
			}
			return ret;
		}

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::create(std::string id)
		{
			return load(id);
		}

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::load(std::string id)
		{
			if (!data.load) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);

			std::shared_ptr<T> ret;
			bool r = get(id, ret, true);

			if (!r) {
				generic_class<T> ret2 = generic_class<T>(data.load, data.unload, id);

				if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

				data.m.lock();
				data.vec.push_back(ret2);
				data.m.unlock();

				ret = ret2.data();
			}
			return ret;
		}

		template<typename T>
		inline void SuperResource<T>::create(std::vector<std::string> n)
		{
			load(n);
		}

		template<typename T>
		inline void SuperResource<T>::load(std::vector<std::string> n, float* perc) // GOOD
		{
			for (size_t p = 0; p < n.size(); p++)
			{
				if (perc) *perc = 1.0f * p / n.size();

				load(n[p]);
			}
			if (perc) *perc = 1.00;
		}

		template<typename T>
		inline bool SuperResource<T>::get(const std::string id, std::shared_ptr<T>& ret, const bool force_enabled) { // GOOD
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

		template<typename T>
		inline bool SuperResource<T>::remove(const std::string id) {
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

		template<typename T>
		inline bool SuperResource<T>::remove(const std::shared_ptr<T> sptr) {
			if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
			data.m.lock();
			for (size_t p = 0; p < data.vec.size(); p++) {
				auto& i = data.vec[p];
				if (i.data() == sptr) {
					data.vec.erase(data.vec.begin() + p);
					data.m.unlock();
					return true;
				}
			}
			data.m.unlock();
			return false;
		}

		template<typename T>
		inline size_t SuperResource<T>::remove(const std::function<bool(const std::string)> f)
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

		template<typename T>
		inline std::vector<std::shared_ptr<T>> SuperResource<T>::getList(const std::function<bool(const std::string)> f)
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

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::swap(const std::string id, std::shared_ptr<T>& p) {
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

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::swap(const std::string id) {
			data.m.lock();
			for (auto& i : data.vec) {
				if (i == id) {
					generic_class<T> ret2 = generic_class<T>(data.load, data.unload, id);

					if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

					i.data().swap(ret2.data());
					data.m.unlock();
					return i.data();
				}
			}
			data.m.unlock();
			return std::shared_ptr<T>();
		}

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::swapCustomLoad(const std::string id, std::function<bool(T*&)> f) {
			data.m.lock();
			for (auto& i : data.vec) {
				if (i == id) {
					std::function<bool(T*&)> _f = [=](T*& v)->bool { return f(v); };
					generic_class<T> ret2 = generic_class<T>(_f, data.unload, id);

					if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

					i.data().swap(ret2.data());
					data.m.unlock();
					return i.data();
				}
			}
			data.m.unlock();
			return create(id);
		}

		template<typename T>
		inline std::shared_ptr<T> SuperResource<T>::swapNew(const std::string id) {
			data.m.lock();
			for (auto& i : data.vec) {
				if (i == id) {
					generic_class<T> ret2 = generic_class<T>(data.load, data.unload, id);

					if (!ret2.data()) throw Abort::Abort(__FUNCSIG__, "Can't load a resource! id=" + id, Abort::abort_level::FATAL_ERROR);

					i.data().swap(ret2.data());
					data.m.unlock();
					return i.data();
				}
			}
			data.m.unlock();
			return create(id);
		}

		template<typename T>
		inline void SuperResource<T>::clear() {
			if (!data.unload) throw Abort::Abort(__FUNCSIG__, "You have to setup load/unload lambdas before using dynamic resource", Abort::abort_level::FATAL_ERROR);
			data.m.lock();
			data.vec.clear();
			data.m.unlock();
		}











		/* PRETTY USEFUL LAMBDA STUFF FOR BIG_TEMPLATES */
		template<typename H> const auto lambda_null_load = [](H*& r) -> bool { return false; };
		template<typename H> const auto lambda_null_unload = [](H*& b) -> void { b = nullptr;  return; };


		template<typename K> const auto lambda_default_load = [](K*& r) -> bool { return (r = new K()); };
		template<typename K> const auto lambda_default_unload = [](K*& b) -> void { if (b) delete b; b = nullptr; };


		//inline const auto lambda_bitmap_load = [](ALLEGRO_BITMAP*& b) -> bool { return (b = al_load_bitmap(p.c_str())); };
		//inline const auto lambda_bitmap_unload = [](ALLEGRO_BITMAP*& b) -> void {if (al_is_system_installed() && b) { al_destroy_bitmap(b); b = nullptr; } };

		// big problems down low here
		/*
		inline const auto lambda_font_load = [](ALLEGRO_FONT*& b) -> bool { return (b = al_load_ttf_font(p.c_str(), 75.0, 0)); };
		inline const auto lambda_font_unload = [](ALLEGRO_FONT*& b) -> void { if (al_is_system_installed() && b) { al_destroy_font(b); b = nullptr; } };

		inline const auto lambda_sample_load = [](ALLEGRO_SAMPLE*& b) -> bool { return (b = al_load_sample(p.c_str())); };
		inline const auto lambda_sample_unload = [](ALLEGRO_SAMPLE*& b) -> void { if (al_is_system_installed() && b) { al_destroy_sample(b); b = nullptr; } };
		*/

		inline const auto lambda_sampleinst_load = [](ALLEGRO_SAMPLE_INSTANCE*& b) -> bool { return (b = al_create_sample_instance(nullptr)); };
		inline const auto lambda_sampleinst_unload = [](ALLEGRO_SAMPLE_INSTANCE*& b) -> void { if (al_is_system_installed() && b) { al_detach_sample_instance(b); al_destroy_sample_instance(b); b = nullptr; } };

		inline const auto lambda_voice_load = [](ALLEGRO_VOICE*& b) -> bool { return (b = al_create_voice(48000, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2)); };
		inline const auto lambda_voice_unload = [](ALLEGRO_VOICE*& b) -> void { if (al_is_system_installed() && b) { al_detach_voice(b); al_destroy_voice(b); b = nullptr; } };

		inline const auto lambda_mixer_load = [](ALLEGRO_MIXER*& b) -> bool { return (b = al_create_mixer(48000, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2)); };
		inline const auto lambda_mixer_unload = [](ALLEGRO_MIXER*& b) -> void { if (al_is_system_installed() && b) { al_detach_mixer(b); al_destroy_mixer(b); b = nullptr; } };


		template<typename T> SuperResource<T>::_data<T> SuperResource<T>::data = { lambda_default_load<T>, lambda_default_unload<T> };

		// type specific
		//template <> SuperResource<ALLEGRO_BITMAP>::_data<ALLEGRO_BITMAP>					SuperResource<ALLEGRO_BITMAP>::data				= { lambda_bitmap_load, lambda_bitmap_unload };
		//template <> SuperResource<ALLEGRO_FONT>::_data<ALLEGRO_FONT>						SuperResource<ALLEGRO_FONT>::data				= { lambda_font_load, lambda_font_unload };
		//template <> SuperResource<ALLEGRO_SAMPLE>::_data<ALLEGRO_SAMPLE>					SuperResource<ALLEGRO_SAMPLE>::data				= { lambda_sample_load, lambda_sample_unload };
		template <> SuperResource<ALLEGRO_SAMPLE_INSTANCE>::_data<ALLEGRO_SAMPLE_INSTANCE>	SuperResource<ALLEGRO_SAMPLE_INSTANCE>::data	= { lambda_sampleinst_load, lambda_sampleinst_unload };
		template <> SuperResource<ALLEGRO_VOICE>::_data<ALLEGRO_VOICE>						SuperResource<ALLEGRO_VOICE>::data				= { lambda_voice_load, lambda_voice_unload };
		template <> SuperResource<ALLEGRO_MIXER>::_data<ALLEGRO_MIXER>						SuperResource<ALLEGRO_MIXER>::data				= { lambda_mixer_load, lambda_mixer_unload };


}
}