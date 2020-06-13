#pragma once

//#define LSW_NO_AUTOSAVE_DEBUG

// C++
#include <string>
// Others
#include "..\Abort\abort.h"
#include "..\supermap\supermap.h"
#include "..\Logger\logger.h"
#include "..\tools\tools.h"
#include "..\supermutex\supermutex.h"

namespace LSW {
	namespace v5 {
		namespace database {

			// new
			constexpr int d_mode = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL;
			constexpr int delay_flush = 3000; // 5 min (this val / 10)

			enum class e_double {
				LAST_VOLUME, RESOLUTION_BUFFER_PROPORTION, FX_AMOUNT, // on file
				MOUSE_X, MOUSE_Y, RAW_MOUSE_X, RAW_MOUSE_Y, INSTANT_FRAMESPERSECOND, INSTANT_COLLISIONSPERSECOND, INSTANT_USEREVENTSPERSECOND, INSTANT_ADVANCEDFUNCSPERSECOND }; // not on file

			enum class e_boolean { 
				WAS_OSD_ON, ENABLE_SECOND_DEBUGGING_SCREEN, ULTRADEBUG, DOUBLE_BUFFERING, HIDEMOUSE, // on file
				SAVING_STRING_INPUT, MOUSE_0, MOUSE_1, MOUSE_2, MOUSE_3, MOUSE_4, MOUSE_5, MOUSE_6, MOUSE_7 }; // not on file

			enum class e_integer {
				SCREEN_X, SCREEN_Y, SCREEN_FLAGS, SCREEN_PREF_HZ, LIMIT_FPS }; // on file

			enum class e_sizet {
				TIMES_LIT, // on file
				MAXIMUM_STRING_INPUT_LEN, FRAMESPERSECOND, COLLISIONSPERSECOND, USEREVENTSPERSECOND, ADVANCEDFUNCSPERSECOND }; // not on file

			enum class e_string {
				LAST_VERSION, LAST_PLAYERNAME, PRINT_PATH, // on file
				LAST_STRING, CURRENT_STRING, DATA_PATH, KEY_ADD }; // not on file
			
			enum class e_color {
				LAST_PLAYER_COLOR // on file
			};

			enum class e_voidfunctional {
				MOUSE_KEY, KEYBOARD_KEY, MOUSE_LEFT, KEYBOARD_LEFT }; // not on file

			enum class e_commandonly { // no need for arguments, not on file
				KEY_SET, KEY_POP, KEY_ERASE = KEY_POP/*old*/
			};


			const std::string e_true = "true"; // if not 'true', false.
			enum class customtriggers { SAVE_ON_FILE };

			const SuperMap<double>			e_double_defaults   = {
				{0.5,															(e_double::LAST_VOLUME),											CHAR_INIT("last_volume"),						customtriggers::SAVE_ON_FILE},
				{1.0, 															(e_double::RESOLUTION_BUFFER_PROPORTION),							CHAR_INIT("resolution_proportion"),				customtriggers::SAVE_ON_FILE},
				{1.0, 															(e_double::FX_AMOUNT),												CHAR_INIT("fx_amount"),							customtriggers::SAVE_ON_FILE},
				{0.0, 															(e_double::MOUSE_X),												CHAR_INIT("mouse_x")},
				{0.0, 															(e_double::MOUSE_Y),												CHAR_INIT("mouse_y")},
				{0.0,			 												(e_double::RAW_MOUSE_X),											CHAR_INIT("raw_mouse_x")},
				{0.0, 															(e_double::RAW_MOUSE_Y),											CHAR_INIT("raw_mouse_y")},
				{0.0, 															(e_double::INSTANT_FRAMESPERSECOND),								CHAR_INIT("instant_frames_per_second_ms")},
				{0.0, 															(e_double::INSTANT_COLLISIONSPERSECOND),							CHAR_INIT("instant_collisions_per_second_ms")},
				{0.0, 															(e_double::INSTANT_USEREVENTSPERSECOND),							CHAR_INIT("instant_events_per_second_ms")},
				{0.0,			 												(e_double::INSTANT_ADVANCEDFUNCSPERSECOND),							CHAR_INIT("instant_functions_per_second_ms")}
			};
			const SuperMap<bool>			e_boolean_defaults  = {
				{false, 														(e_boolean::WAS_OSD_ON),											CHAR_INIT("was_osd_on"),						customtriggers::SAVE_ON_FILE},
				{false, 														(e_boolean::ENABLE_SECOND_DEBUGGING_SCREEN),						CHAR_INIT("second_screen_debug"),				customtriggers::SAVE_ON_FILE},
				{false,  														(e_boolean::ULTRADEBUG),											CHAR_INIT("ultradebug"),						customtriggers::SAVE_ON_FILE},
				{false,  														(e_boolean::DOUBLE_BUFFERING),										CHAR_INIT("double_buffering_screen"),			customtriggers::SAVE_ON_FILE},
				{true,   														(e_boolean::HIDEMOUSE),												CHAR_INIT("hidemouse"),							customtriggers::SAVE_ON_FILE},
				{false,  														(e_boolean::SAVING_STRING_INPUT),									CHAR_INIT("saving_string_input")},
				{false,  														(e_boolean::MOUSE_0),												CHAR_INIT("mouse_button_0")},
				{false,  														(e_boolean::MOUSE_1), 												CHAR_INIT("mouse_button_1")},
				{false,  														(e_boolean::MOUSE_2), 												CHAR_INIT("mouse_button_2")},
				{false, 														(e_boolean::MOUSE_3), 												CHAR_INIT("mouse_button_3")},
				{false, 														(e_boolean::MOUSE_4), 												CHAR_INIT("mouse_button_4")},
				{false, 														(e_boolean::MOUSE_5), 												CHAR_INIT("mouse_button_5")},
				{false,  														(e_boolean::MOUSE_6), 												CHAR_INIT("mouse_button_6")},
				{false,  														(e_boolean::MOUSE_7), 												CHAR_INIT("mouse_button_7")}
			};
			const SuperMap<int>				e_integer_defaults  = {
				{0,  															(e_integer::SCREEN_X), 												CHAR_INIT("screen_width"),						customtriggers::SAVE_ON_FILE},
				{0,  															(e_integer::SCREEN_Y), 												CHAR_INIT("screen_height"),						customtriggers::SAVE_ON_FILE},
				{(d_mode),														(e_integer::SCREEN_FLAGS),											CHAR_INIT("last_display_flags"),				customtriggers::SAVE_ON_FILE},
				{0,  															(e_integer::SCREEN_PREF_HZ),										CHAR_INIT("pref_refresh_rate"),					customtriggers::SAVE_ON_FILE},
				{0,  															(e_integer::LIMIT_FPS),												CHAR_INIT("limit_framerate_to"),				customtriggers::SAVE_ON_FILE}
			};
			const SuperMap<size_t>			e_sizet_defaults    = {
				{0ULL,  														(e_sizet::TIMES_LIT), 												CHAR_INIT("times_open"),						customtriggers::SAVE_ON_FILE},
				{0ULL,  														(e_sizet::MAXIMUM_STRING_INPUT_LEN),								CHAR_INIT("maximum_string_input_length")},
				{0ULL,  														(e_sizet::FRAMESPERSECOND),											CHAR_INIT("frames_per_second")},
				{0ULL,  														(e_sizet::COLLISIONSPERSECOND),										CHAR_INIT("collisions_per_second")},
				{0ULL,  														(e_sizet::USEREVENTSPERSECOND), 									CHAR_INIT("events_per_second")},
				{0ULL,  														(e_sizet::ADVANCEDFUNCSPERSECOND),									CHAR_INIT("functions_per_second")}
			};
			const SuperMap<std::string>		e_string_defaults	= {
				{std::string("V1.0.0"),											(e_string::LAST_VERSION),											CHAR_INIT("last_version"),						customtriggers::SAVE_ON_FILE},
				{std::string("Player"),											(e_string::LAST_PLAYERNAME),										CHAR_INIT("playername"),						customtriggers::SAVE_ON_FILE},
				{std::string("%win_photos_path%/Your Game Screenshots/"),		(e_string::PRINT_PATH),												CHAR_INIT("prints_path"),						customtriggers::SAVE_ON_FILE},
				{std::string(""),												(e_string::LAST_STRING),											CHAR_INIT("last_string")},
				{std::string(""),												(e_string::CURRENT_STRING),											CHAR_INIT("current_string")},
				{std::string(""),												(e_string::DATA_PATH),												CHAR_INIT("data_path")},
				{std::string(""), 												(e_string::KEY_ADD),												CHAR_INIT("key_add")} // translates to SuperChar (single multiple char char)
			};
			const SuperMap<ALLEGRO_COLOR>	e_color_defaults = {
				{al_map_rgb(255,255,255),										(e_color::LAST_PLAYER_COLOR),										CHAR_INIT("playercolor"),						customtriggers::SAVE_ON_FILE}
			};
		}

		class Database {
			struct database_data {
				// actual data
				SuperMap<double>		double_data	 = database::e_double_defaults;
				SuperMap<bool>			boolean_data = database::e_boolean_defaults;
				SuperMap<int>			integer_data = database::e_integer_defaults;
				SuperMap<size_t>		sizet_data	 = database::e_sizet_defaults;
				SuperMap<std::string>	string_data	 = database::e_string_defaults;
				SuperMap<ALLEGRO_COLOR>	color_data	 = database::e_color_defaults;
				ALLEGRO_CONFIG*			config = nullptr;
				std::string				config_path;

				// secure access
				SuperMutex mute;

				// additional data
				std::thread* autosave_thread = nullptr;
				bool keep_autosaving = false;
			};
			static database_data data;

			void autosave_run();
#ifndef LSW_NO_AUTOSAVE_DEBUG
			void flush(const bool); // print save
#endif
		public:
#ifdef LSW_NO_AUTOSAVE_DEBUG
			void flush(const bool = false);
#endif

			Database();
			Database(const std::string);

			void close();

			void set(const database::e_double, const double);
			void set(const database::e_boolean, const bool);
			void set(const database::e_integer, const int);
			void set(const database::e_sizet, const size_t);
			void set(const database::e_string, const std::string);
			void set(const database::e_color, const ALLEGRO_COLOR);
			void set(const std::string, const double);
			void set(const std::string, const bool);
			void set(const std::string, const int);
			void set(const std::string, const size_t);
			void set(const std::string, const std::string);
			void set(const std::string, const ALLEGRO_COLOR);

			bool get(const database::e_double, double&);
			bool get(const database::e_boolean, bool&);
			bool get(const database::e_integer, int&);
			bool get(const database::e_sizet, size_t&);
			bool get(const database::e_string, std::string&);
			bool get(const database::e_color, ALLEGRO_COLOR&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, int&);
			bool get(const std::string, size_t&);
			bool get(const std::string, std::string&);
			bool get(const std::string, ALLEGRO_COLOR&);

			double*			getRef(const database::e_double);	// Direct reference to the value
			bool*			getRef(const database::e_boolean);	// Direct reference to the value
			int*			getRef(const database::e_integer);	// Direct reference to the value
			size_t*			getRef(const database::e_sizet);	// Direct reference to the value
			std::string*	getRef(const database::e_string);	// Direct reference to the value
			ALLEGRO_COLOR* 	getRef(const database::e_color);	// Direct reference to the value
			
			template<typename T, typename V> inline bool g(const T a, V& b) { return get(a, b); };
			template<typename T, typename V> inline bool isEq(const T e, const V v) { V k; if (get(e, k)) return v == k; return false; }
			template<typename T, typename V> inline bool ie(const T e, const V v) { return isEq(e, v); };
		};
	}
}