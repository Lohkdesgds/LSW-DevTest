#pragma once

// C
#define _CRT_RAND_S
// C++
#include <numeric>
#include <functional>
// Others
#include "..\lower\bitmap\bitmap.h"
#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\database\database.h"
#include "..\tools\tools.h"
#include "..\shared\shared.h"

#undef max

namespace LSW {
	namespace v5 {

		namespace text {

			enum class e_string_readonly { PROCESSED_STRING };
			enum class e_chronomillis_readonly { LAST_UPDATE_STRING };

			enum class e_cstring { STRING };
			enum class e_double { SHADOW_DISTANCE_X, SHADOW_DISTANCE_Y, TEXT_UPDATE_TIME };
			enum class e_color { SHADOW_COLOR };
			enum class e_integer { STRING_MODE };
			enum class e_sprite_ptr { FOLLOWING };
			enum class e_boolean { USE_FOLLOWING_COLOR_INSTEAD, NO_AUTO_STRING_INTERPRETATION, USE_PER_CHAR_COLORING };

			const SuperMap<coloured_string>										e_string_readonly_defaults = {
				{"",																														(e_string_readonly::PROCESSED_STRING),					CHAR_INIT("processed_string")}
			};
			const SuperMap<std::chrono::milliseconds>							e_chronomillis_readonly_defaults = {
				{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()),				(e_chronomillis_readonly::LAST_UPDATE_STRING),			CHAR_INIT("last_update_string")}
			};
			const SuperMap<std::function<coloured_string(void)>>				e_string_defaults = {
				{[] {return coloured_string(); },																							(e_cstring::STRING),									CHAR_INIT("string")}
			};
			const SuperMap<std::function<double(void)>>							e_double_defaults = {
				{[] {return 0.0; } ,																											(e_double::SHADOW_DISTANCE_X),							CHAR_INIT("shadow_distance_x")},
				{[] {return 0.0; } ,																											(e_double::SHADOW_DISTANCE_Y),							CHAR_INIT("shadow_distance_y")}
			};
			const SuperMap<std::function<ALLEGRO_COLOR(void)>>					e_color_defaults = {
				{[] {return ALLEGRO_COLOR(); },																								(e_color::SHADOW_COLOR),								CHAR_INIT("shadow_color")}
			};
			const SuperMap<std::function<int(void)>>							e_integer_defaults = {
				{[] {return 0; },																											(e_integer::STRING_MODE),								CHAR_INIT("string_mode")}
			};
			const SuperMap<std::function<std::weak_ptr<Sprite_Base>(void)>>		e_sprite_ptr_defaults = {
				{[] {return std::weak_ptr<Sprite_Base>(); },																					(e_sprite_ptr::FOLLOWING),								CHAR_INIT("following")}
			};
			const SuperMap<std::function<bool(void)>>							e_boolean_defaults = {
				{[] {return false; },																										(e_boolean::USE_FOLLOWING_COLOR_INSTEAD),				CHAR_INIT("use_following_color_instead")},
				{[] {return false; },																										(e_boolean::NO_AUTO_STRING_INTERPRETATION),				CHAR_INIT("no_auto_string_interpretation")},
				{[] {return false; },																										(e_boolean::USE_PER_CHAR_COLORING),						CHAR_INIT("use_per_char_coloring")}
			};

			enum class e_text_modes { LEFT, CENTER, RIGHT };

			// ADVANCED TAGGING REPLACEMENT

			enum class e_custom_tags {
				T_POSX, T_POSY, T_SCREEN_POSX, T_SCREEN_POSY, T_ISFOLLOWING, T_COLOR_R, T_COLOR_G, T_COLOR_B, T_COLOR_A, T_MODE, T_TIME, T_ISUSINGBUF, T_GB_RESX, T_GB_RESY, T_REFRESHRATE, T_FPS, T_TPS, T_UPS, T_APS, T_MPS, T_I_FPS, T_I_TPS, T_I_UPS, T_I_APS, T_I_MPS, T_MS_FPS, T_MS_TPS, T_MS_UPS, T_MS_APS, T_MS_MPS,
				/*T_SPRITE_FRAME,*/ T_CAM_X, T_CAM_Y, T_CAM_ZOOM, T_CAM_ZOOMG, T_CAM_ZOOMX, T_CAM_ZOOMY, T_CURRSTRING, T_LASTSTRING, T_MOUSE_X, T_MOUSE_Y, T_SPRITE_SPEEDX, T_SPRITE_SPEEDY, T_SPRITE_NAME, T_SPRITE_ENTITY_NAME, T_SPRITE_ENTITY_HEALTH, _T_SPRITE_DEBUG, T_TEXTURES_LOADED,
				T_FONTS_LOADED, T_SPRITES_LOADED,/* T_TEXTS_LOADED,*/ T_TRACKS_LOADED,/* T_SPRITE_STATE,*/ T_VOLUME, T_VERSION, T_RESOLUTION_PROPORTION, T_CHROMA_FX, T_FPS_CAP, size
			};


			constexpr auto timeout_interpret = std::chrono::milliseconds(500);		// milliseconds!
			constexpr auto timeout_interpret_debugging = std::chrono::seconds(300); // SECONDS!
			constexpr auto default_sharpness_font = 75.0;
			constexpr auto default_delta_t_text_update_delay = std::chrono::seconds(1); // don't need to be fast
		}

		class Text : public Sprite_Base {
			/*struct _text {
				std::shared_ptr<ALLEGRO_FONT> ref;
				std::string source, id;
			};*/

			struct text_data {
				SuperMap<coloured_string>												string_readonly_data = text::e_string_readonly_defaults;
				SuperMap<std::chrono::milliseconds>										chronomillis_readonly_data = text::e_chronomillis_readonly_defaults;
				SuperMap<std::function<coloured_string(void)>>							string_data = text::e_string_defaults;
				SuperMap<std::function<double(void)>>									double_data = text::e_double_defaults;
				SuperMap<std::function<ALLEGRO_COLOR(void)>>							color_data = text::e_color_defaults;
				SuperMap<std::function<int(void)>>										integer_data = text::e_integer_defaults;
				SuperMap<std::function<std::weak_ptr<Sprite_Base>(void)>>				sprite_ptr_data = text::e_sprite_ptr_defaults;
				SuperMap<std::function<bool(void)>>										boolean_data = text::e_boolean_defaults;

				SuperMap<std::function<std::string(void)>>								custom_tags; // has to initialize defaults in constructor to link functions to itself
			};

			std::shared_ptr<text_data> data_text = std::make_shared<text_data>();

			//_text font;
			std::shared_ptr<ALLEGRO_FONT> fontt;

			// call on Text() build
			void __link_hard_task();

			void interpretTags(coloured_string&);
			ALLEGRO_COLOR hex(const int);

			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getRef;

			Text();
			Text(Text&);

			// from what id, path (can be "null" if you know the file is indeed loaded as id)
			void load(const std::string, const std::string = "");
			// gets from main font (SuperResource.setMain(...))
			void load();
			// removes font
			void remove();

			// set to blindly use this comrade's attributes as theirs (this must be Block)
			void twinUpAttributes(const std::shared_ptr<text_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<text_data> getAttributes();

			void addNewEntry(const std::string, std::function<std::string(void)>);
			void removeEntry(const std::string);

			void set(const text::e_cstring, coloured_string);
			void set(const text::e_double, double);
			void set(const text::e_color, ALLEGRO_COLOR);
			void set(const text::e_integer, int);
			void set(const text::e_sprite_ptr, std::shared_ptr<Sprite_Base>);
			void set(const text::e_boolean, bool);
			void set(const std::string, coloured_string);
			void set(const std::string, double);
			void set(const std::string, ALLEGRO_COLOR);
			void set(const std::string, int);
			void set(const std::string, std::shared_ptr<Sprite_Base>);
			void set(const std::string, bool);

			void set(const text::e_cstring, std::function<coloured_string(void)>);
			void set(const text::e_double, std::function<double(void)>);
			void set(const text::e_color, std::function<ALLEGRO_COLOR(void)>);
			void set(const text::e_integer, std::function<int(void)>);
			void set(const text::e_sprite_ptr, std::function<std::shared_ptr<Sprite_Base>(void)>);
			void set(const text::e_boolean, std::function<bool(void)>);
			void set(const std::string, std::function<coloured_string(void)>);
			void set(const std::string, std::function<double(void)>);
			void set(const std::string, std::function<ALLEGRO_COLOR(void)>);
			void set(const std::string, std::function<int(void)>);
			void set(const std::string, std::function<std::shared_ptr<Sprite_Base>(void)>);
			void set(const std::string, std::function<bool(void)>);


			bool get(const text::e_string_readonly, coloured_string&);
			bool get(const text::e_cstring, coloured_string&);
			bool get(const text::e_double, double&);
			bool get(const text::e_color, ALLEGRO_COLOR&);
			bool get(const text::e_integer, int&);
			bool get(const text::e_sprite_ptr, std::shared_ptr<Sprite_Base>&);
			bool get(const text::e_boolean, bool&);
			bool get(const std::string, coloured_string&);
			bool get(const std::string, double&);
			bool get(const std::string, ALLEGRO_COLOR&);
			bool get(const std::string, int&);
			bool get(const std::string, std::shared_ptr<Sprite_Base>&);
			bool get(const std::string, bool&);

			bool get(const text::e_cstring, std::function<coloured_string(void)>&);
			bool get(const text::e_double, std::function<double(void)>&);
			bool get(const text::e_color, std::function<ALLEGRO_COLOR(void)>&);
			bool get(const text::e_integer, std::function<int(void)>&);
			bool get(const text::e_sprite_ptr, std::function<std::weak_ptr<Sprite_Base>(void)>&);
			bool get(const text::e_boolean, std::function<bool(void)>&);
			bool get(const std::string, std::function<coloured_string(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<ALLEGRO_COLOR(void)>&);
			bool get(const std::string, std::function<int(void)>&);
			bool get(const std::string, std::function<std::weak_ptr<Sprite_Base>(void)>&);
			bool get(const std::string, std::function<bool(void)>&);

			std::function<coloured_string(void)>* getRef(const text::e_cstring);
			std::function<double(void)>* getRef(const text::e_double);
			std::function<ALLEGRO_COLOR(void)>* getRef(const text::e_color);
			std::function<int(void)>* getRef(const text::e_integer);
			std::function<std::weak_ptr<Sprite_Base>(void)>* getRef(const text::e_sprite_ptr);
			std::function<bool(void)>* getRef(const text::e_boolean);
			const coloured_string* getRef(const text::e_string_readonly) const;
			const std::chrono::milliseconds* getRef(const text::e_chronomillis_readonly) const;

		};
	}
}	