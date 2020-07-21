#pragma once

// C++
#include <memory>
#include <string>
#include <functional>

// Others
#include "..\abort\abort.h"
#include "..\superresource\superresource.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\mixer\mixer.h"
#include "..\Lower\sample\sample.h"
#include "..\supermap\supermap.h"
#include "..\tools\tools.h"

namespace LSW {
	namespace v5 {

		namespace track {
			enum class e_boolean {PLAYING};
			enum class e_double {VOLUME, SPEED};
			enum class e_integer {PLAY_MODE};

			const SuperMap<std::function<bool(void)>>		e_boolean_defaults = {
				{[] {return false; },										(e_boolean::PLAYING),						CHAR_INIT("playing")}
			};
			const SuperMap<std::function<double(void)>>		e_double_defaults = {
				{[] {return 0.5; },											(e_double::VOLUME),							CHAR_INIT("volume")},
				{[] {return 1.0; },											(e_double::SPEED),							CHAR_INIT("speed")}
			};
			const SuperMap<std::function<int(void)>>		e_integer_defaults = {
				{[] {return ALLEGRO_PLAYMODE_ONCE; },						(e_integer::PLAY_MODE),						CHAR_INIT("play_mode")}
			};

			enum class e_track_modes { ONCE = ALLEGRO_PLAYMODE_ONCE, LOOP = ALLEGRO_PLAYMODE_LOOP, BIDIR = ALLEGRO_PLAYMODE_BIDIR };
		}

		class Track {
			struct track_data {
				SuperMap<std::function<bool(void)>>		boolean_data		= track::e_boolean_defaults;
				SuperMap<std::function<double(void)>>	double_data			= track::e_double_defaults;
				SuperMap<std::function<int(void)>>		integer_data		= track::e_integer_defaults;
			};

			std::shared_ptr<track_data> data_track = std::make_shared<track_data>();


			std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> playing;
		public:
			Track();
			~Track();

			// id, sample id to load
			bool load(const std::shared_ptr<Sample>);
			// id, sample name
			bool load(const std::string);

			// attaches to a Mixer
			bool attach(const std::shared_ptr<Mixer>);

			// auto attach to main Mixer (see SuperResource.setMain(...))
			bool autoAttach();

			// set to blindly use this comrade's attributes as theirs (this must be Block)
			void twinUpAttributes(const std::shared_ptr<track_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<track_data> getAttributes();

			void set(const track::e_boolean, bool);
			void set(const track::e_double, double);
			void set(const track::e_integer, int);
			void set(const std::string, bool);
			void set(const std::string, double);
			void set(const std::string, int);

			void set(const track::e_boolean, std::function<bool(void)>);
			void set(const track::e_double, std::function<double(void)>);
			void set(const track::e_integer, std::function<int(void)>);
			void set(const std::string, std::function<bool(void)>);
			void set(const std::string, std::function<double(void)>);
			void set(const std::string, std::function<int(void)>);

			bool get(const track::e_boolean, bool&);
			bool get(const track::e_double, double&);
			bool get(const track::e_integer, int&);
			bool get(const std::string, bool&);
			bool get(const std::string, double&);
			bool get(const std::string, int&);

			bool get(const track::e_boolean, std::function<bool(void)>&);
			bool get(const track::e_double, std::function<double(void)>&);
			bool get(const track::e_integer, std::function<int(void)>&);
			bool get(const std::string, std::function<bool(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<int(void)>&);

			std::function<bool(void)>*		getDirect(const track::e_boolean);
			std::function<double(void)>*	getDirect(const track::e_double);
			std::function<int(void)>*		getDirect(const track::e_integer);


			std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> getInstance();

			// updates data set to it like volume and stuff
			void update();
		};

	}
}