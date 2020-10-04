#pragma once

#include <memory>

#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Handling\Path\path.h"
#include "..\..\Tools\Common\common.h"

namespace LSW {
	namespace v5 {
		namespace Interface {


			class Config {
				struct config_data {
					std::string path;
					ALLEGRO_CONFIG* conf = nullptr;

					config_data(const std::string& = "");
					~config_data();

					void save();
				};

				std::shared_ptr<config_data> conf;
			public:
				Config() = default;
				Config(const Config&) = delete;
				Config(Config&&) noexcept;

				bool load(std::string);
				void create();

				// update path to save
				void save_at(std::string);

				// destroy, but with save if has path saved by load(...) [does flush()]
				void close();
				
				// save
				void flush();

				// set a value to a key in section ( [section]\nkey=value )
				void set(const std::string&, const std::string&, const std::string&);
				// comment a section (don't use # or breaklines, they'll be changed to % and ' ' respectively)
				void comment(const std::string&, std::string);

				// get value in section -> key
				std::string get(const std::string&, const std::string&);

				void operator=(const Config&) = delete;
				void operator=(Config&&) noexcept;

				// merges what the other has inside this one (overwrites if exists already). It doesn't copy comments!
				Config& operator+(const Config&);
			};


		}
	}
}
