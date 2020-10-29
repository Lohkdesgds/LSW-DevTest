#include "config.h"

namespace LSW {
	namespace v5 {
		namespace Interface {
			void Config::_set(section& i, const std::string& key, const std::string& val)
			{
				if (key.empty() || val.empty()) return;

				for (auto& j : i.key_and_value) {
					if (j.first == key) {
						j.second = val;
						return;
					}
				}
				i.key_and_value.push_back({ key, val });
				return;
			}
			Config::section& Config::_get_to_set(const std::string& sec, const config::config_section_mode mode)
			{
				for (auto& i : conff) {
					if (i.section_name == sec) {
						return i;
					}
				}
				conff.push_back({ mode, sec });
				return conff.back();
			}
			const Config::section& Config::_get(const std::string& sec) const
			{
				for (auto& i : conff) {
					if (i.section_name == sec) {
						return i;
					}
				}
				throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + "!", Handling::abort::abort_level::GIVEUP);
				return conff.front();
			}
			bool Config::_has(const std::string& sec) const
			{
				for (auto& i : conff) {
					if (i.section_name == sec) return true;
				}
				return false;
			}
			Config::Config(Config&& c) noexcept
			{
				path = std::move(c.path);
				autosave = c.autosave;
				conff = std::move(c.conff);
			}

			Config::~Config()
			{
				if (autosave) flush();
			}

			void Config::auto_save(bool b)
			{
				autosave = b;
			}

			bool Config::load(std::string str)
			{
				if (str.empty()) {
					logg.debug("A configuration could not be loaded because path was null!", E::ERRR);
					//throw Handling::Abort(__FUNCSIG__, "A configuration could not be loaded because path was null!");
					return false;
				}

				Handling::handle_path(str);
				ALLEGRO_CONFIG* conf = nullptr;
				conf = al_load_config_file(str.c_str());
				if (!conf) {
					logg.debug("Failed to load configuration file: " + str, E::ERRR);
					//throw Handling::Abort(__FUNCSIG__, "Failed to open config '" + str + "'!");
					return false;
				}

				path = str;

				ALLEGRO_CONFIG_SECTION* sect = nullptr;
				ALLEGRO_CONFIG_ENTRY* entr = nullptr;
				conff.clear();

				{
					const char* section = nullptr;

					do {
						if (!section) section = al_get_first_config_section(conf, &sect);
						else section = al_get_next_config_section(&sect);
						if (!section) break;

						conff.push_back({ config::config_section_mode::SAVE, section });
						auto& backk = conff.back();

						const char* key = nullptr;
						do {
							if (!entr) key = al_get_first_config_entry(conf, section, &entr);
							else key = al_get_next_config_entry(&entr);
							if (!key) break;

							const char* result = al_get_config_value(conf, section, key);

							logg.debug(std::string(section) + " > " + key + " = " + result);

							backk.key_and_value.push_back({ key, result });

						} while (key);

					} while (section);

				}
				al_destroy_config(conf);
				return true;
			}

			void Config::save_path(std::string str)
			{
				if (str.empty()) {
					logg.debug("Null path is not a valid path at save_path!", E::ERRR);
					throw Handling::Abort(__FUNCSIG__, "Null path is not a valid path at save_path!");
				}

				Handling::handle_path(str);
				path = str;
			}

			void Config::flush()
			{
				if (path.empty()) {
					logg.debug("A configuration could not be saved because path was null!", E::WARN);
					return;
				}

				ALLEGRO_CONFIG* conf = al_create_config();
				if (!conf) {
					logg.debug("Failed to create a buffer to flush config.", E::ERRR);
					throw Handling::Abort(__FUNCSIG__, "Failed to create a buffer to flush config.");
				}

				for (auto& i : conff) {
					if (i.mode != config::config_section_mode::SAVE) continue;
					if (!i.section_comment.empty()) al_add_config_comment(conf, i.section_name.c_str(), i.section_comment.c_str());
					for (auto& j : i.key_and_value) {
						al_set_config_value(conf, i.section_name.c_str(), j.first.c_str(), j.second.c_str());
					}
				}

				if (!al_save_config_file(path.c_str(), conf)) {
					al_destroy_config(conf);
					logg.debug("Failed to save configuration file: " + path, E::ERRR);
					throw Handling::Abort(__FUNCSIG__, "Failed to save config '" + path + "'!");
				}
				al_destroy_config(conf);

				logg.debug("Saved configuration file: " + path);
			}

			void Config::set(const std::string& sec, const config::config_section_mode mode)
			{
				this->_get_to_set(sec, mode);
			}



			/*void Config::set(const std::string& sec, const std::string& key, const std::string& val, config::config_section_mode mode)
			{
				auto& u = _get_to_set(sec, mode);
				_set(u, key, val);
				u.mode = mode;
			}

			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<std::string>& vals, const config::config_section_mode mode)
			{
				std::string res;
				for (auto& i : vals) {
					for (auto& j : i) if (j == ';') throw Handling::Abort(__FUNCSIG__, "Invalid character ';' in multiple values configuration!");
					res += i + ';';
				}
				set(sec, key, "{" + res + "}", mode);
			}*/



			void Config::set(const std::string& sec, const std::string& key, const std::string& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, val);
			}

			void Config::set(const std::string& sec, const std::string& key, const bool& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, val ? "true" : "false");
			}

			void Config::set(const std::string& sec, const std::string& key, const char& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const int& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const float& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const double& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const unsigned& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const long& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const long long& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const size_t& val)
			{
				auto& u = _get_to_set(sec);
				_set(u, key, std::to_string(val));
			}

			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<std::string>& vals)
			{
				std::string res;
				for (auto& i : vals) {
					if (i.find(';')) throw Handling::Abort(__FUNCSIG__, "Invalid character ';' in multiple values configuration!");
					res += i + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}

			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<bool>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::string(i ? "true" : "false") + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<char>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<int>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<float>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<double>& vals)
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<unsigned>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<long>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<long long>& vals)
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}
			void Config::set(const std::string& sec, const std::string& key, const std::initializer_list<size_t>& vals) 
			{
				std::string res;
				for (auto& i : vals) {
					res += std::to_string(i) + ';';
				}
				if (res.length() > 1) res.pop_back();
				set(sec, key, "{" + res + "}");
			}


			void Config::comment(const std::string& sec, std::string comment)
			{
				for (auto& i : comment) {
					if (i == '\n') i = ' ';
					else if (i == '#') i = '%';
				}

				for (auto& i : conff) {
					if (i.section_name == sec) {
						i.section_comment = comment;
						return;
					}
				}
				conff.push_back({ config::config_section_mode::SAVE, sec });
				auto& backk = conff.back();
				backk.section_comment = comment;
			}

			std::string Config::get(const std::string& sec, const std::string& key) const
			{
				for (auto& i : conff) {
					if (sec == i.section_name) {
						for (auto& j : i.key_and_value) {
							if (j.first == key) return j.second;
						}
					}
				}
				return "";
			}

			/*std::vector<std::string> Config::get_array(const std::string& sec, const std::string& key)
			{
				std::vector<std::string> found;
				for (auto& i : conff) {
					if (sec == i.section_name) {
						for (auto& j : i.key_and_value) {
							if (j.first == key) {
								auto& str = j.second;
								if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config. It is not an array or it is invalid.");

								str.erase(str.begin());
								str.pop_back();

								std::stringstream ss(str);
								std::string _buf;

								while (std::getline(ss, _buf, ';')) {
									found.push_back(_buf);
								}

								return std::move(found);
							}
						}
					}
				}
				return found;
			}*/

			void Config::operator=(Config&& c) noexcept
			{
				path = std::move(c.path);
				autosave = c.autosave;
				conff = std::move(c.conff);
			}


			/*
			Config::config_data::config_data(const std::string& str)
			{
				Handling::init_basic();
				path = str;
				if (!path.empty()) {
					if (!(conf = al_load_config_file(str.c_str()))) throw Handling::Abort(__FUNCSIG__, "Cannot open config file!");
				}
				else {
					if (!(conf = al_create_config())) throw Handling::Abort(__FUNCSIG__, "Cannot create config file!");
				}
			}

			Config::config_data::~config_data()
			{
				save();
				if (conf){
					al_destroy_config(conf);
					conf = nullptr;
				}
			}

			void Config::config_data::save()
			{
				if (conf && !path.empty()) {
					al_save_config_file(path.c_str(), conf);
				}
			}

			Config::Config(Config&& c) noexcept
			{
				conf = std::move(c.conf);
			}

			bool Config::load(std::string pth)
			{
				Handling::handle_path(pth);
				try {
					conf = std::make_shared<config_data>(pth);
				}
				catch (Handling::Abort a)
				{
					conf = std::make_shared<config_data>();
					save_at(pth);
					flush();
				}
				return conf && conf->conf;
			}

			void Config::create()
			{
				conf = std::make_shared<config_data>();
			}

			void Config::save_at(std::string newpath)
			{
				if (conf) {
					Handling::handle_path(newpath);
					conf->path = newpath;
				}
			}

			void Config::close()
			{
				flush();
				conf.reset();
			}

			void Config::flush()
			{
				conf->save();
			}

			void Config::set(const std::string& section, const std::string& key, const std::string& value)
			{
				if (!conf || !conf->conf) throw Handling::Abort(__FUNCSIG__, "Config is invalid");

				al_set_config_value(conf->conf, section.c_str(), key.c_str(), value.c_str());
			}

			void Config::comment(const std::string& section, std::string comment)
			{
				if (!conf || !conf->conf) throw Handling::Abort(__FUNCSIG__, "Config is invalid");

				for (auto& i : comment) {
					if (i == '\n') i = ' ';
					else if (i == '#') i = '%';
				}
				// do your thing

				al_add_config_comment(conf->conf, section.c_str(), comment.c_str());
			}
			std::string Config::get(const std::string& section, const std::string& key)
			{
				if (!conf || !conf->conf) throw Handling::Abort(__FUNCSIG__, "Config is invalid");

				return al_get_config_value(conf->conf, section.c_str(), key.c_str());
			}
			void Config::operator=(Config&& c) noexcept
			{
				conf = std::move(c.conf);
			}
			Config& Config::operator+(const Config& c)
			{
				if (!conf || !conf->conf || !c.conf || !c.conf->conf) throw Handling::Abort(__FUNCSIG__, "Config is invalid");

				al_merge_config_into(conf->conf, c.conf->conf);

				return *this;
			}
			*/
		}
	}
}