#pragma once

// C
#define _CRT_RAND_S
// C++
#include <numeric>
#include <functional>
// Others
#include "..\Lower\font\font.h"
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

			enum class e_cstring_readonly { PROCESSED_STRING };
			enum class e_chronomillis_readonly { LAST_UPDATE_STRING };

			enum class e_cstring { STRING };
			enum class e_double { SHADOW_DISTANCE_X, SHADOW_DISTANCE_Y, TEXT_UPDATE_TIME };
			enum class e_color { SHADOW_COLOR };
			enum class e_integer { STRING_MODE };
			enum class e_sprite_ptr { FOLLOWING };
			enum class e_boolean { USE_FOLLOWING_COLOR_INSTEAD, NO_AUTO_STRING_INTERPRETATION, USE_PER_CHAR_COLORING };

			const SuperMap< std::function<std::chrono::milliseconds(void)>>		e_chronomillis_readonly_defaults = {
				{[]{return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());},	(e_chronomillis_readonly::LAST_UPDATE_STRING),			CHAR_INIT("last_update_string")}
			};
			const SuperMap<std::function<coloured_string(void)>>				e_string_defaults = {
				{[] {return coloured_string(); },																							(e_cstring::STRING),									CHAR_INIT("string")},
				{[] {return coloured_string(); },																							(e_cstring_readonly::PROCESSED_STRING),					CHAR_INIT("processed_string")}
			};
			const SuperMap<std::function<double(void)>>							e_double_defaults = {
				{[] {return 0.0; } ,																										(e_double::SHADOW_DISTANCE_X),							CHAR_INIT("shadow_distance_x")},
				{[] {return 0.0; } ,																										(e_double::SHADOW_DISTANCE_Y),							CHAR_INIT("shadow_distance_y")}
			};
			const SuperMap<std::function<ALLEGRO_COLOR(void)>>					e_color_defaults = {
				{[] {return ALLEGRO_COLOR(); },																								(e_color::SHADOW_COLOR),								CHAR_INIT("shadow_color")}
			};
			const SuperMap<std::function<int(void)>>							e_integer_defaults = {
				{[] {return 0; },																											(e_integer::STRING_MODE),								CHAR_INIT("string_mode")}
			};
			const SuperMap<std::function<std::weak_ptr<Sprite_Base>(void)>>		e_sprite_ptr_defaults = {
				{[] {return std::weak_ptr<Sprite_Base>(); },																				(e_sprite_ptr::FOLLOWING),								CHAR_INIT("following")}
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

			enum class e_custom_tags_tag { CUSTOM_TAG };


			constexpr auto timeout_interpret = std::chrono::milliseconds(500);		// milliseconds!
			constexpr auto timeout_interpret_debugging = std::chrono::seconds(300); // SECONDS!
			constexpr auto default_sharpness_font = 75.0;
			constexpr auto default_delta_t_text_update_delay = std::chrono::seconds(1); // don't need to be fast
		}

		class Text : public Sprite_Base, public Workaround<std::chrono::milliseconds>, public Workaround<coloured_string>, public Workaround<std::weak_ptr<Sprite_Base>> {
			/*struct _text {
				std::shared_ptr<ALLEGRO_FONT> ref;
				std::string source, id;
			};*/

			struct text_data : Sprite_Base::sprite_base_data {
				std::shared_ptr<SuperMap<std::function<std::chrono::milliseconds(void)>>>				chronomillis_readonly_data;
				std::shared_ptr<SuperMap<std::function<coloured_string(void)>>>							cstring_data;
				std::shared_ptr<SuperMap<std::function<std::weak_ptr<Sprite_Base>(void)>>>				sprite_ptr_data;
			};

			//SuperMap<std::function<std::string(void)>>								custom_tags; // has to initialize defaults in constructor to link functions to itself

			//std::shared_ptr<text_data> data_text = std::make_shared<text_data>();

			//_text font;
			std::shared_ptr<Font> fontt;

			// call on Text() build
			void __link_hard_task();

			void interpretTags(coloured_string&);

			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getDirect;
			using Workaround<std::chrono::milliseconds>::set;
			using Workaround<std::chrono::milliseconds>::get;
			using Workaround<std::chrono::milliseconds>::getDirect;
			using Workaround<coloured_string>::set;
			using Workaround<coloured_string>::get;
			using Workaround<coloured_string>::getDirect;
			using Workaround<std::weak_ptr<Sprite_Base>>::set;
			using Workaround<std::weak_ptr<Sprite_Base>>::get;
			using Workaround<std::weak_ptr<Sprite_Base>>::getDirect;

			Text();
			Text(Text&);

			// gets specific font from SuperResource
			void load(const std::string);
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

		};
	}
}	