#pragma once

#include <numeric>
#include <functional>

#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\database\database.h"
#include "..\tools\tools.h"

#undef max

namespace LSW {
	namespace v5 {

		namespace block {

			enum class e_chronomillis_readonly { LAST_FRAME, LAST_TIE_FRAME_VERIFICATION };
			enum class e_integer { FRAME };
			enum class e_boolean { SET_FRAME_VALUE_READONLY };
			enum class e_double { FRAMES_PER_SECOND, TIE_SIZE_TO_DISPLAY_PROPORTION };

			const SuperMap<std::chrono::milliseconds>		e_chronomillis_readonly_defaults{
				{MILI_NOW,																										(e_chronomillis_readonly::LAST_FRAME),					CHAR_INIT("last_frame")},
				{MILI_NOW,																										(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	CHAR_INIT("last_tie_verification")}
			};
			const SuperMap<std::function<int(void)>>		e_integer_defaults = {
				{[] {return 0;},																								(e_integer::FRAME),										CHAR_INIT("frame")}
			};
			const SuperMap<std::function<bool(void)>>		e_boolean_defaults = {
				{[] {return false;},																							(e_boolean::SET_FRAME_VALUE_READONLY),					CHAR_INIT("set_frame_readonly")}
			};
			const SuperMap<std::function<double(void)>>		e_double_defaults = {
				{[] {return 10.0;},																								(e_double::FRAMES_PER_SECOND),							CHAR_INIT("frames_per_second")},
				{[] {return 0.0;},																								(e_double::TIE_SIZE_TO_DISPLAY_PROPORTION),				CHAR_INIT("tie_size_to_display_proportion")}
			};

			constexpr size_t maximum_bitmap_amount = static_cast<size_t>(std::numeric_limits<int>::max());
			constexpr size_t static_frame = static_cast<size_t>(-1); // static frame X?
			constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast
		}

		class Block : public Sprite_Base {
			struct _bitmap {
				std::shared_ptr<ALLEGRO_BITMAP> ref;
				std::string source, id;
				bool is_sub_bitmap = false; // if true, source is source's id, else source is path
			};

			struct entity_data {
				SuperMap<std::function<int(void)>>			integer_data				= block::e_integer_defaults;
				SuperMap<std::function<double(void)>>		double_data					= block::e_double_defaults;
				SuperMap<std::function<bool(void)>>			boolean_data				= block::e_boolean_defaults;
				SuperMap<std::chrono::milliseconds>			chronomillis_readonly_data	= block::e_chronomillis_readonly_defaults;
			} data;

			std::vector<_bitmap> bitmaps;


			//ALLEGRO_BITMAP* bmp = nullptr;

			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getRef;

			Block();
			Block(Block&);
			//~Block(); // bitmaps are handled by SuperResource, there might be some other people around using the same resource

			// from what id, path (can be "null" if you know the file is indeed loaded as id)
			void load(const std::string, const std::string = "");
			// new id, from what id copy, x, y, w, h
			void loadCut(const std::string, const std::string, const int, const int, const int, const int);
			// removes from its list all matches
			void remove(const std::string);

			void set(const block::e_integer, int);
			void set(const block::e_double, double);
			void set(const block::e_boolean, bool);
			void set(const block::e_integer, std::function<int(void)>);
			void set(const block::e_double, std::function<double(void)>);
			void set(const block::e_boolean, std::function<bool(void)>);
			void set(const std::string, int);
			void set(const std::string, double);
			void set(const std::string, bool);
			void set(const std::string, std::function<int(void)>);
			void set(const std::string, std::function<double(void)>);
			void set(const std::string, std::function<bool(void)>);

			bool get(const block::e_integer, int&);
			bool get(const block::e_double, double&);
			bool get(const block::e_boolean, bool&);
			bool get(const block::e_integer, std::function<int(void)>&);
			bool get(const block::e_double, std::function<double(void)>&);
			bool get(const block::e_boolean, std::function<bool(void)>&);
			bool get(const std::string, int&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, std::function<int(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<bool(void)>&);

			bool get(const block::e_chronomillis_readonly, std::chrono::milliseconds&);

			std::function<int(void)>*			getRef(const block::e_integer);
			std::function<double(void)>*		getRef(const block::e_double);
			std::function<bool(void)>*			getRef(const block::e_boolean);
			const std::chrono::milliseconds*	getRef(const block::e_chronomillis_readonly) const;

		};


		namespace text {

			enum class e_string_readonly { PROCESSED_STRING };
			enum class e_chronomillis_readonly { LAST_UPDATE_STRING };

			enum class e_cstring { STRING };
			enum class e_double { SHADOW_DISTANCE_X, SHADOW_DISTANCE_Y, TEXT_UPDATE_TIME };
			enum class e_color { SHADOW_COLOR };
			enum class e_integer { STRING_MODE };
			enum class e_sprite_ptr { FOLLOWING };
			enum class e_boolean { USE_FOLLOWING_COLOR_INSTEAD, NO_AUTO_STRING_INTERPRETATION, USE_PER_CHAR_COLORING };

			const SuperMap<coloured_string>							e_string_readonly_defaults = {
				{"",																														(e_string_readonly::PROCESSED_STRING),					CHAR_INIT("processed_string")}
			};
			const SuperMap<std::chrono::milliseconds>				e_chronomillis_readonly_defaults = {
				{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()),				(e_chronomillis_readonly::LAST_UPDATE_STRING),			CHAR_INIT("last_update_string")}
			};
			const SuperMap<std::function<coloured_string(void)>>	e_string_defaults = {
				{[] {return coloured_string();},																							(e_cstring::STRING),									CHAR_INIT("string")}
			};
			const SuperMap<std::function<double(void)>>				e_double_defaults = {
				{[] {return 0.0;} ,																											(e_double::SHADOW_DISTANCE_X),							CHAR_INIT("shadow_distance_x")},
				{[] {return 0.0;} ,																											(e_double::SHADOW_DISTANCE_Y),							CHAR_INIT("shadow_distance_y")}
			};
			const SuperMap<std::function<ALLEGRO_COLOR(void)>>		e_color_defaults = {
				{[] {return ALLEGRO_COLOR();},																								(e_color::SHADOW_COLOR),								CHAR_INIT("shadow_color")}
			};
			const SuperMap<std::function<int(void)>>				e_integer_defaults = {
				{[] {return 0;},																											(e_integer::STRING_MODE),								CHAR_INIT("string_mode")}
			};
			const SuperMap<std::function<Sprite_Base*(void)>>		e_sprite_ptr_defaults = {
				{[] {return nullptr;},																										(e_sprite_ptr::FOLLOWING),								CHAR_INIT("following")}
			};
			const SuperMap<std::function<bool(void)>>				e_boolean_defaults = {
				{[] {return false;},																										(e_boolean::USE_FOLLOWING_COLOR_INSTEAD),				CHAR_INIT("use_following_color_instead")},
				{[] {return false;},																										(e_boolean::NO_AUTO_STRING_INTERPRETATION),				CHAR_INIT("no_auto_string_interpretation")},
				{[] {return false;},																										(e_boolean::USE_PER_CHAR_COLORING),						CHAR_INIT("use_per_char_coloring")}
			};

			enum class e_text_modes { LEFT, CENTER, RIGHT };


			class __slice {
				std::string n;
				int p;
			public:
				template<typename T>
				__slice(const std::string w, const T v) { n = w; p = static_cast<int>(v); }
				const std::string s() const { return n; }
				const int i() const { return p; }
			};
			enum class tags_e {
				T_POSX, T_POSY, T_SCREEN_POSX, T_SCREEN_POSY, T_ISFOLLOWING, T_COLOR_R, T_COLOR_G, T_COLOR_B, T_COLOR_A, T_MODE, T_TIME, T_ISUSINGBUF, T_GB_RESX, T_GB_RESY, T_REFRESHRATE, T_FPS, T_TPS, T_UPS, T_APS, T_I_FPS, T_I_TPS, T_I_UPS, T_I_APS, T_MS_FPS, T_MS_TPS, T_MS_UPS, T_MS_APS,
				/*T_SPRITE_FRAME,*/ T_CAM_X, T_CAM_Y, T_CAM_ZOOM, T_CAM_ZOOMG, T_CAM_ZOOMX, T_CAM_ZOOMY, T_CURRSTRING, T_LASTSTRING, T_MOUSE_X, T_MOUSE_Y, T_SPRITE_SPEEDX, T_SPRITE_SPEEDY, T_SPRITE_NAME, T_SPRITE_ENTITY_NAME, T_SPRITE_ENTITY_HEALTH, _T_SPRITE_DEBUG, T_TEXTURES_LOADED, T_FONTS_LOADED,
				T_SPRITES_LOADED,/* T_TEXTS_LOADED,*/ T_TRACKS_LOADED,/* T_SPRITE_STATE,*/ T_VOLUME, T_VERSION, T_RESOLUTION_PROPORTION, T_CHROMA_FX, T_FPS_CAP, size
			};
			inline const __slice tags[] = {
__slice("%pos_x%", tags_e::T_POSX),  __slice("%pos_y%", tags_e::T_POSY), __slice("%screen_pos_x%", tags_e::T_SCREEN_POSX), __slice("%screen_pos_y%", tags_e::T_SCREEN_POSY), __slice("%is_following%", tags_e::T_ISFOLLOWING),
__slice("%color_r%", tags_e::T_COLOR_R), __slice("%color_g%", tags_e::T_COLOR_G), __slice("%color_b%", tags_e::T_COLOR_B), __slice("%color_a%", tags_e::T_COLOR_A), __slice("%mode%", tags_e::T_MODE), __slice("%time%", tags_e::T_TIME),
__slice("%is_using_buf%", tags_e::T_ISUSINGBUF), __slice("%curr_res_x%", tags_e::T_GB_RESX), __slice("%curr_res_y%", tags_e::T_GB_RESY), __slice("%curr_refresh_rate%", tags_e::T_REFRESHRATE), __slice("%int_fps%", tags_e::T_FPS),
__slice("%int_tps%", tags_e::T_TPS), __slice("%int_ups%", tags_e::T_UPS), __slice("%int_aps%", tags_e::T_APS), __slice("%instant_fps%", tags_e::T_I_FPS), __slice("%instant_tps%", tags_e::T_I_TPS), __slice("%instant_ups%", tags_e::T_I_UPS),
__slice("%instant_aps%", tags_e::T_I_APS), __slice("%ms_fps%", tags_e::T_MS_FPS), __slice("%ms_tps%", tags_e::T_MS_TPS), __slice("%ms_ups%", tags_e::T_MS_UPS), __slice("%ms_aps%", tags_e::T_MS_APS), /*__slice("%sprite_frame%", tags_e::T_SPRITE_FRAME),*/
__slice("%cam_x%", tags_e::T_CAM_X), __slice("%cam_y%", tags_e::T_CAM_Y), __slice("%cam_zoom_combined%", tags_e::T_CAM_ZOOM), __slice("%cam_zoom_g%", tags_e::T_CAM_ZOOMG), __slice("%cam_zoom_x%", tags_e::T_CAM_ZOOMX),
__slice("%cam_zoom_y%",tags_e::T_CAM_ZOOMY),__slice("%curr_string%", tags_e::T_CURRSTRING), __slice("%last_string%", tags_e::T_LASTSTRING), __slice("%mouse_x%", tags_e::T_MOUSE_X), __slice("%mouse_y%", tags_e::T_MOUSE_Y),
__slice("%sprite_speed_x%", tags_e::T_SPRITE_SPEEDX),__slice("%sprite_speed_y%", tags_e::T_SPRITE_SPEEDY), __slice("%sprite_name%", tags_e::T_SPRITE_NAME), __slice("%entity_name%", tags_e::T_SPRITE_ENTITY_NAME),
__slice("%entity_health%", tags_e::T_SPRITE_ENTITY_HEALTH), __slice("%sprite_debug%", tags_e::_T_SPRITE_DEBUG), __slice("%num_images%", tags_e::T_TEXTURES_LOADED), __slice("%num_fonts%", tags_e::T_FONTS_LOADED),
__slice("%num_sprites%", tags_e::T_SPRITES_LOADED)/*, __slice("%num_texts%", tags_e::T_TEXTS_LOADED)*/, __slice("%num_tracks%", tags_e::T_TRACKS_LOADED)/*, __slice("%sprite_state%", tags_e::T_SPRITE_STATE)*/,__slice("%volume_perc%", tags_e::T_VOLUME),
__slice("%version%", tags_e::T_VERSION),__slice("%screen_buf_proportion%", tags_e::T_RESOLUTION_PROPORTION),__slice("%screen_chroma_fx%", tags_e::T_CHROMA_FX), __slice("%fps_cap%", tags_e::T_FPS_CAP) };


			constexpr auto timeout_interpret = std::chrono::milliseconds(500);
			constexpr auto default_sharpness_font = 75.0;
			constexpr auto default_delta_t_text_update_delay = std::chrono::seconds(1); // don't need to be fast
		}


		class Text : public Sprite_Base {
			struct _text {
				std::shared_ptr<ALLEGRO_FONT> ref;
				std::string source, id;
			};

			struct text_data {
				SuperMap<coloured_string>								string_readonly_data		= text::e_string_readonly_defaults;
				SuperMap<std::chrono::milliseconds>						chronomillis_readonly_data	= text::e_chronomillis_readonly_defaults;
				SuperMap<std::function<coloured_string(void)>>			string_data					= text::e_string_defaults;
				SuperMap<std::function<double(void)>>					double_data					= text::e_double_defaults;
				SuperMap<std::function<ALLEGRO_COLOR(void)>>			color_data					= text::e_color_defaults;
				SuperMap<std::function<int(void)>>						integer_data				= text::e_integer_defaults;
				SuperMap<std::function<Sprite_Base*(void)>>				sprite_ptr_data				= text::e_sprite_ptr_defaults;
				SuperMap<std::function<bool(void)>>						boolean_data				= text::e_boolean_defaults;
			} data;


			_text font;

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
			// removes font
			void remove();

			void set(const text::e_cstring, coloured_string);
			void set(const text::e_double, double);
			void set(const text::e_color, ALLEGRO_COLOR);
			void set(const text::e_integer, int);
			void set(const text::e_sprite_ptr, Sprite_Base*);
			void set(const text::e_boolean, bool);
			void set(const std::string, coloured_string);
			void set(const std::string, double);
			void set(const std::string, ALLEGRO_COLOR);
			void set(const std::string, int);
			void set(const std::string, Sprite_Base*);
			void set(const std::string, bool);

			void set(const text::e_cstring, std::function<coloured_string(void)>);
			void set(const text::e_double, std::function<double(void)>);
			void set(const text::e_color, std::function<ALLEGRO_COLOR(void)>);
			void set(const text::e_integer, std::function<int(void)>);
			void set(const text::e_sprite_ptr, std::function<Sprite_Base*(void)>);
			void set(const text::e_boolean, std::function<bool(void)>);
			void set(const std::string, std::function<coloured_string(void)>);
			void set(const std::string, std::function<double(void)>);
			void set(const std::string, std::function<ALLEGRO_COLOR(void)>);
			void set(const std::string, std::function<int(void)>);
			void set(const std::string, std::function<Sprite_Base*(void)>);
			void set(const std::string, std::function<bool(void)>);


			bool get(const text::e_string_readonly, coloured_string&);
			bool get(const text::e_cstring, coloured_string&);
			bool get(const text::e_double, double&);
			bool get(const text::e_color, ALLEGRO_COLOR&);
			bool get(const text::e_integer, int&);
			bool get(const text::e_sprite_ptr, Sprite_Base*&);
			bool get(const text::e_boolean, bool&);
			bool get(const std::string, coloured_string&);
			bool get(const std::string, double&);
			bool get(const std::string, ALLEGRO_COLOR&);
			bool get(const std::string, int&);
			bool get(const std::string, Sprite_Base*&);
			bool get(const std::string, bool&);

			bool get(const text::e_cstring, std::function<coloured_string(void)>&);
			bool get(const text::e_double, std::function<double(void)>&);
			bool get(const text::e_color, std::function<ALLEGRO_COLOR(void)>&);
			bool get(const text::e_integer, std::function<int(void)>&);
			bool get(const text::e_sprite_ptr, std::function<Sprite_Base*(void)>&);
			bool get(const text::e_boolean, std::function<bool(void)>&);
			bool get(const std::string, std::function<coloured_string(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<ALLEGRO_COLOR(void)>&);
			bool get(const std::string, std::function<int(void)>&);
			bool get(const std::string, std::function<Sprite_Base*(void)>&);
			bool get(const std::string, std::function<bool(void)>&);

			std::function<coloured_string(void)>*	getRef(const text::e_cstring);
			std::function<double(void)>*			getRef(const text::e_double);
			std::function<ALLEGRO_COLOR(void)>*		getRef(const text::e_color);
			std::function<int(void)>*				getRef(const text::e_integer);
			std::function<Sprite_Base*(void)>*		getRef(const text::e_sprite_ptr);
			std::function<bool(void)>*				getRef(const text::e_boolean);
			const coloured_string*					getRef(const text::e_string_readonly) const;
			const std::chrono::milliseconds*		getRef(const text::e_chronomillis_readonly) const;

		};
	}
}