#include "config.h"

namespace LSW {
	namespace v5 {
		namespace Interface {
			

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
		}
	}
}