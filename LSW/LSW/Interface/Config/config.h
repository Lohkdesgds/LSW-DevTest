#pragma once

#include <memory>

#include "../../Handling/Abort/abort.h"
#include "../../Handling/Initialize/initialize.h"
#include "../../Handling/Path/path.h"
#include "../../Tools/Common/common.h"
#include "../../Interface/Logger/logger.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace config {
				enum class config_section_mode{ SAVE, MEMORY_ONLY };
			}

			/// <summary>
			/// <para>Config handles both saved configurations and temporary (while running) settings.</para>
			/// <para>The type of data is set by section.</para>
			/// </summary>
			class Config {
				struct section {
					config::config_section_mode mode;
					std::string section_name;
					std::string section_comment;
					std::vector<std::pair<std::string, std::string>> key_and_value;
				};

				Logger logg;
				std::string path; // save path
				bool autosave = true;
				std::vector<section> conff;

				// section itself, key and value. Won't change if empty (one of them). Adds if not existant
				void _set(section&, const std::string&, const std::string&);
				// by section id. if not found, create section. Mode is used if it has to create a new one.
				section& _get_to_set(const std::string&, const config::config_section_mode = config::config_section_mode::SAVE);
				// by section id. if not found, throw
				const section& _get(const std::string&) const;
				// checks existance of a section
				bool _has(const std::string&) const;
			public:
				Config() = default;
				Config(const Config&) = delete;

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{Config}">Config to move from.</param>
				Config(Config&&) noexcept;
				~Config();

				/// <summary>
				/// <para>Sets if it should save when destroyed or not (defaults: true).</para>
				/// </summary>
				/// <param name="{bool}">Autosave?</param>
				void auto_save(bool);

				/// <summary>
				/// <para>Loads a config file (automatically will save to this path later).</para>
				/// <para>It does interpret/create the path.</para>
				/// </summary>
				/// <param name="{std::string}">Path.</param>
				/// <returns>{bool} True if success.</returns>
				bool load(std::string);

				/// <summary>
				/// <para>Sets a path to save at.</para>
				/// </summary>
				/// <param name="{std::string}">Path.</param>
				void save_path(std::string);

				/// <summary>
				/// <para>Flushes the config to file.</para>
				/// </summary>
				void flush();

				/// <summary>
				/// <para>Change a existing section's mode.</para>
				/// </summary>
				/// <param name="{std::string}">Section name.</param>
				/// <param name="{config::config_section_mode}">The new mode.</param>
				void set(const std::string&, const config::config_section_mode);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::string}">Value.</param>
				void set(const std::string&, const std::string&, const std::string&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{bool}">Value.</param>
				void set(const std::string&, const std::string&, const bool&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{char}">Value.</param>
				void set(const std::string&, const std::string&, const char&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{int}">Value.</param>
				void set(const std::string&, const std::string&, const int&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{float}">Value.</param>
				void set(const std::string&, const std::string&, const float&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{double}">Value.</param>
				void set(const std::string&, const std::string&, const double&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{unsigned}">Value.</param>
				void set(const std::string&, const std::string&, const unsigned&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{long}">Value.</param>
				void set(const std::string&, const std::string&, const long&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{long long}">Value.</param>
				void set(const std::string&, const std::string&, const long long&);

				/// <summary>
				/// <para>Set a config value.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{size_t}">Value.</param>
				void set(const std::string&, const std::string&, const size_t&);


				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<std::string>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<bool>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<char>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<int>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<float>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<double>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<unsigned>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<long>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<long long>&);

				/// <summary>
				/// <para>Set values into a config key at once.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <param name="{std::initializer_list}">Values.</param>
				void set(const std::string&, const std::string&, const std::initializer_list<size_t>&);

				/// <summary>
				/// <para>Adds a comment to the config file.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Comment.</param>
				void comment(const std::string&, std::string);

				/// <summary>
				/// <para>Get a value from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::string} Value.</returns>
				std::string get(const std::string&, const std::string&) const;

				template<typename T> T get_as(const std::string&, const std::string&) const = delete;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{bool} Value.</returns>
				template<> bool get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{char} Value.</returns>
				template<> char get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{int} Value.</returns>
				template<> int get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{float} Value.</returns>
				template<> float get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{double} Value.</returns>
				template<> double get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{unsigned} Value.</returns>
				template<> unsigned get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{long} Value.</returns>
				template<> long get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{long long} Value.</returns>
				template<> long long get_as(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a value from config as a type T.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{size_t} Value.</returns>
				template<> size_t get_as(const std::string&, const std::string&) const;

				// if you know (you probably know) that the config has been set with initializer list, this will break down the list to a std::vector<std::string>
				template<typename T> std::vector<T> get_array(const std::string&, const std::string&) const = delete;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<std::string> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<bool> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<char> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<int> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<float> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<double> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<unsigned> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<long> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<long long> get_array(const std::string&, const std::string&) const;

				/// <summary>
				/// <para>Get a vector of values from config.</para>
				/// </summary>
				/// <param name="{std::string}">Section.</param>
				/// <param name="{std::string}">Key.</param>
				/// <returns>{std::vector} Vector of values.</returns>
				template<> std::vector<size_t> get_array(const std::string&, const std::string&) const;

				void operator=(const Config&) = delete;

				/// <summary>
				/// <para>Move operator.</para>
				/// </summary>
				/// <param name="{Config}">Config to move from.</param>
				void operator=(Config&&) noexcept;
			};




			// // // // // // // // // // // // // \ TEMPLATE IMPLEMENTATION \ // // // // // // // // // // // // //



			template<> bool Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				return res == "true";
			}
			template<> char Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				if (res.length() > 1) throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover get_as config.");
				return res[0];
			}
			template<> int Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				return std::atoi(res.c_str());
			}
			template<> float Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				return std::atof(res.c_str());
			}
			template<> double Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				double varr;
				if (!sscanf_s(res.c_str(), "%lf", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover get_as config.");
				return varr;
			}
			template<> unsigned Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				unsigned varr;
				if (!sscanf_s(res.c_str(), "%u", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover get_as config.");
				return varr;
			}
			template<> long Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				long varr;
				if (!sscanf_s(res.c_str(), "%ld", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover get_as config.");
				return varr;
			}
			template<> long long Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				long long varr;
				if (!sscanf_s(res.c_str(), "%lld", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover get_as config.");
				return varr;
			}
			template<> size_t Config::get_as(const std::string& sec, const std::string& key) const
			{
				auto res = get(sec, key);
				size_t varr;
				if (!sscanf_s(res.c_str(), "%zu", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover get_as config.");
				return varr;
			}




			template<> std::vector<std::string> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<std::string> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							vec.push_back(_buf);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<bool> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<bool> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							vec.push_back(_buf == "true");
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<char> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<char> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							if (_buf.length() > 1) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config.");
							vec.push_back(_buf[0]);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<int> Config::get_array(const std::string& sec, const std::string& key) const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<int> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							vec.push_back(std::atoi(_buf.c_str()));
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<float> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<float> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							vec.push_back(std::atof(_buf.c_str()));
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<double> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<double> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							double varr;
							if (!sscanf_s(_buf.c_str(), "%lf", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config.");
							vec.push_back(varr);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<unsigned> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<unsigned> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							unsigned varr;
							if (!sscanf_s(_buf.c_str(), "%u", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config.");
							vec.push_back(varr);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<long> Config::get_array(const std::string& sec, const std::string& key) const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<long> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							long varr;
							if (!sscanf_s(_buf.c_str(), "%ld", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config.");
							vec.push_back(varr);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<long long> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<long long> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							long long varr;
							if (!sscanf_s(_buf.c_str(), "%lld", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config.");
							vec.push_back(varr);
						}

						return std::move(vec);
					}
				}
				return vec;
			}
			template<> std::vector<size_t> Config::get_array(const std::string& sec, const std::string& key)  const
			{
				if (!_has(sec)) throw Handling::Abort(__FUNCSIG__, "There is no section called " + sec + " in this config file (" + path + ")");
				std::vector<size_t> vec;
				auto& s = _get(sec);
				for (auto& i : s.key_and_value) {
					if (i.first == key) {
						auto str = i.second;
						if (str.empty() || str.front() != '{' || str.back() != '}') throw Handling::Abort(__FUNCSIG__, "Fatal error trying to recover array-type config.");

						str.erase(str.begin());
						str.pop_back();

						std::stringstream ss(str);
						std::string _buf;

						while (std::getline(ss, _buf, ';')) {
							size_t varr;
							if (!sscanf_s(_buf.c_str(), "%zu", &varr)) throw Handling::Abort(__FUNCSIG__, "Invalid key while trying to recover array-type config");
							vec.push_back(varr);
						}

						return std::move(vec);
					}
				}
				return vec;
			}






			// Direct ALLEGRO way. I want options. Discarded this. But it is cool to have a backup anyway.
			/*
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
			*/

		}
	}
}
