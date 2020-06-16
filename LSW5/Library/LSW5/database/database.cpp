#include "database.h"


namespace LSW {
	namespace v5 {
		Database::database_data Database::data;


		void Database::autosave_run()
		{
			data.keep_autosaving = true; // "I initialized!"
			while (data.keep_autosaving)
			{
				for (unsigned t = 0; t < database::delay_flush && data.keep_autosaving; t++) Sleep(100); // 5 sec with test
				flush(!data.keep_autosaving);
			}
		}

		void Database::flush(const bool print)
		{
			data.mute.lock();
			Logger logg;

			/*char lmao[512];
			char* transformed_lmao = (lmao);*/

			// DOUBLE
			{
				auto* cpy = data.double_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save

					al_set_config_value(data.config, "config", transformed_lmao, std::to_string(*Tools::assert_cast(data.double_data[transformed_lmao])).c_str());
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << std::to_string(*Tools::assert_cast(data.double_data[transformed_lmao])) << "&2'" << L::EL;
				}
			}
			// BOOL
			{
				auto* cpy = data.boolean_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save


					al_set_config_value(data.config, "config", transformed_lmao, (*Tools::assert_cast(data.boolean_data[transformed_lmao]) ? "true" : "false"));
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << (*Tools::assert_cast(data.boolean_data[transformed_lmao]) ? "true" : "false") << "&2'" << L::EL;
				}
			}
			// INT
			{
				auto* cpy = data.integer_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save

					al_set_config_value(data.config, "config", transformed_lmao, std::to_string(*Tools::assert_cast(data.integer_data[transformed_lmao])).c_str());
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << std::to_string(*Tools::assert_cast(data.integer_data[transformed_lmao])) << "&2'" << L::EL;
				}
			}
			// SIZET
			{
				auto* cpy = data.sizet_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save

					al_set_config_value(data.config, "config", transformed_lmao, std::to_string(*Tools::assert_cast(data.sizet_data[transformed_lmao])).c_str());
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << std::to_string(*Tools::assert_cast(data.sizet_data[transformed_lmao])) << "&2'" << L::EL;
				}
			}
			// STD::STRING
			{
				auto* cpy = data.string_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save

					al_set_config_value(data.config, "config", transformed_lmao, (*Tools::assert_cast(data.string_data[transformed_lmao])).c_str());
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << *Tools::assert_cast(data.string_data[transformed_lmao]) << "&2'" << L::EL;
				}
			}
			// COLOR STUFF
			{
				auto* cpy = data.color_data.__getcopy();
				for (size_t pp = 0; cpy && pp < cpy->size(); pp++) {
					auto& i = (*cpy)[pp];
					{
						database::customtriggers ct = database::customtriggers::SAVE_ON_FILE;
						if (!i.hasType<database::customtriggers>()) continue;
					}
					if (!i.hasType<char*>()) continue; // no string aka no config save

					size_t len_ = 0;
					char transformed_lmao[512] = { 0 };

					i.getType<>(transformed_lmao);

					if (Tools::strlen_s(transformed_lmao) == 0) continue; // no string aka no config save

					ALLEGRO_COLOR clr = *Tools::assert_cast(data.color_data[transformed_lmao]);
					char savin[512];
					sprintf_s(savin, "{%.4f;%.4f;%.4f}", clr.r, clr.g, clr.b);
					al_set_config_value(data.config, "config", transformed_lmao, savin);
					if (print) logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9Saved config entry &2'&a" << transformed_lmao << "&2' &9as &2'&a" << savin << "&2'" << L::EL;
				}
			}

			logg << L::SL << fsr(__FUNCSIG__, E::DEBUG) << "&9AutoSave: Config " << (al_save_config_file(data.config_path.c_str(), data.config) ? "&2'&asaved successfully&2'" : "&4'&cfailed to save&4'") << L::EL;

			data.mute.unlock();
		}

		Database::Database()
		{
			if (!data.autosave_thread) {
				throw Abort::Abort(__FUNCSIG__, "You have to initialize at least ONCE the Database with a path to load/save the config!");
			}
		}

		Database::Database(const std::string path)
		{
			data.mute.lock();
			if (!data.autosave_thread) // haven't started yet (so load config and stuff please)
			{
				data.config_path = path;
				Tools::handlePath(data.config_path);

				if (!(data.config = al_load_config_file(data.config_path.c_str()))) {
					data.config = al_create_config();
					if (!al_save_config_file(data.config_path.c_str(), data.config)) {
						throw Abort::Abort(__FUNCSIG__, "Invalid path to load/save config!");
					}
				}
				else { // load data from config before saving what it has as default

					Logger logg;

					// DOUBLE STUFF
					for (auto& i : database::e_double_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						double v = 0.0;
						size_t div = 0;
						bool nowdiv = false;
						for (auto& j : std::string(result)) {
							if (j >= '0' && j <= '9') {
								v *= 10.0;
								v += static_cast<double>(j) - static_cast<double>('0');
								if (nowdiv) div++;
							}
							else {
								nowdiv = true;
							}
						}
						v /= (pow(10.0, div));

						if (auto* ptr = data.double_data(transformed_lmao); ptr) *ptr = v;
						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e" << *Tools::assert_cast(data.double_data(static_cast<char*>(transformed_lmao), len_)) << L::ELF;
					}

					// BOOL STUFF
					for (auto& i : database::e_boolean_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						if (auto* ptr = data.boolean_data(transformed_lmao); ptr)  *ptr = ((result == database::e_true));
						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e" << (*Tools::assert_cast(data.boolean_data(static_cast<char*>(transformed_lmao), len_)) ? "true" : "false") << L::ELF;
					}

					//  STUFF
					for (auto& i : database::e_integer_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						int v;
						sscanf_s(result, "%d", &v);

						if (auto* ptr = data.integer_data(transformed_lmao); ptr) *ptr = v;
						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e" << *Tools::assert_cast(data.integer_data(static_cast<char*>(transformed_lmao), len_)) << L::ELF;
					}

					// SIZET STUFF
					for (auto& i : database::e_sizet_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						size_t v;
						sscanf_s(result, "%zu", &v);

						if (auto* ptr = data.sizet_data(transformed_lmao); ptr) *ptr = v;
						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e" << *Tools::assert_cast(data.sizet_data(static_cast<char*>(transformed_lmao), len_)) << L::ELF;
					}

					// STD::STRING STUFF
					for (auto& i : database::e_string_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						if (auto* ptr = data.string_data(transformed_lmao); ptr) *ptr = (std::string(result));

						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e" << *Tools::assert_cast(data.string_data(static_cast<char*>(transformed_lmao), len_)) << L::ELF;
					}

					// COLOR STUFF
					for (auto& i : database::e_color_defaults) {
						if (!i.hasType<char*>()) continue; // no string aka no config save

						size_t len_ = 0;
						char transformed_lmao[512] = { 0 };

						i.getType<>(transformed_lmao);

						if ((len_ = Tools::strlen_s(transformed_lmao)) == 0) continue; // no string aka no config save

						auto result = al_get_config_value(data.config, "config", transformed_lmao);
						if (!result) continue;

						ALLEGRO_COLOR clr;
						sscanf_s(result, "{%f;%f;%f}", &clr.r, &clr.g, &clr.b);
						clr.a = 1.0;

						if (auto* ptr = data.color_data(transformed_lmao); ptr) *ptr = clr;
						logg << L::SLF << fsr(__FUNCSIG__) << "&2Loading config &8| &3" << transformed_lmao << "&6: &e"
							<< clr.r << "," << clr.g << "," << clr.b << L::ELF;
					}



				}
				// if it didn't exist, it will actually set default settings on autosave_run() thread
#ifndef LSW_NO_AUTOSAVE_DEBUG
				data.autosave_thread = new std::thread([&]() {autosave_run(); });
				while (!data.keep_autosaving) Sleep(5);
#endif
			}
			data.mute.unlock();
		}

		void Database::close()
		{
			data.keep_autosaving = false;
			data.autosave_thread->join();

			data.mute.lock();
			delete data.autosave_thread;
			data.autosave_thread = nullptr;
			data.mute.unlock();

			data.double_data.clear();
			data.boolean_data.clear();
			data.integer_data.clear();
			data.sizet_data.clear();
			data.string_data.clear();
			data.color_data.clear();
		}

		void Database::set(const database::e_double e, const double v)
		{
			if (auto* ptr = data.double_data(e); ptr) *ptr = v;
		}

		void Database::set(const database::e_boolean e, const bool v)
		{
			if (auto* ptr = data.boolean_data(e); ptr) *ptr = v;
		}

		void Database::set(const database::e_integer e, const int v)
		{
			if (auto* ptr = data.integer_data(e); ptr) *ptr = v;
		}

		void Database::set(const database::e_sizet e, const size_t v)
		{
			if (auto* ptr = data.sizet_data(e); ptr) *ptr = v;
		}

		void Database::set(const database::e_string e, const std::string v)
		{
			if (auto* ptr = data.string_data(e); ptr) *ptr = v;
		}

		void Database::set(const database::e_color e, const ALLEGRO_COLOR v)
		{
			if (auto* ptr = data.color_data(e); ptr) *ptr = v;
		}

		void Database::set(const std::string e, const double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Database::set(const std::string e, const bool v)
		{
			auto* ptr = data.boolean_data(e.c_str(), e.length());
			if (!ptr) data.boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Database::set(const std::string e, const int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Database::set(const std::string e, const size_t v)
		{
			auto* ptr = data.sizet_data(e.c_str(), e.length());
			if (!ptr) data.sizet_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Database::set(const std::string e, const std::string v)
		{
			auto* ptr = data.string_data(e.c_str(), e.length());
			if (!ptr) data.string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Database::set(const std::string e, const ALLEGRO_COLOR v)
		{
			auto* ptr = data.color_data(e.c_str(), e.length());
			if (!ptr) data.color_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Database::get(const database::e_chronomillis_readonly e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data.chronomillis_readonly_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_boolean e, bool& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_sizet e, size_t& v)
		{
			if (auto* ptr = data.sizet_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_string e, std::string& v)
		{
			if (auto* ptr = data.string_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const database::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data[e]; ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data.chronomillis_readonly_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, bool& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, size_t& v)
		{
			if (auto* ptr = data.sizet_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, std::string& v)
		{
			if (auto* ptr = data.string_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		bool Database::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data(e.c_str(), e.length()); ptr) { v = *ptr; return true; }
			return false;
		}

		const std::chrono::milliseconds* Database::getRef(const database::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data.chronomillis_readonly_data(e); ptr) return ptr;
			return nullptr;
		}

		double* Database::getRef(const database::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr) return ptr;
			return nullptr;
		}

		bool* Database::getRef(const database::e_boolean e)
		{
			if (auto* ptr = data.boolean_data(e); ptr) return ptr;
			return nullptr;
		}

		int* Database::getRef(const database::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr) return ptr;
			return nullptr;
		}

		size_t* Database::getRef(const database::e_sizet e)
		{
			if (auto* ptr = data.sizet_data(e); ptr) return ptr;
			return nullptr;;
		}

		std::string* Database::getRef(const database::e_string e)
		{
			if (auto* ptr = data.string_data(e); ptr) return ptr;
			return nullptr;
		}

		ALLEGRO_COLOR* Database::getRef(const database::e_color e)
		{
			if (auto* ptr = data.color_data(e); ptr) return ptr;
			return nullptr;
		}

		bool& Database::key(const int u)
		{
			if (u < 0 || u >= ALLEGRO_KEY_MAX) throw Abort::Abort(__FUNCSIG__, "Out of range key!");
			return data.keys[u];
		}

}
}