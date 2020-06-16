#pragma once

// C++
#include <string>
// Others
#include "..\abort\abort.h"
#include "..\supermap\supermap.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\autocaster\autocaster.h"
#include "..\tools\tools.h"
#include "..\camera\camera.h"
#include "..\shared\shared.h"
#include "..\database\database.h"


namespace LSW {
	namespace v5 {
		namespace sprite {

			/*
			COL_MINDIST_... = the distance it has to move in ... (on a collision tick)			
			*/
			enum class e_double_readonly { SPEED_X, SPEED_Y, LAST_COLLISION_TIME /* related to: COLLISION_COLLIDED */, LAST_DRAW, POSX, POSY, ROTATION, COL_MINDIST_X, COL_MINDIST_Y };
			enum class e_boolean_readonly { COLLISION_MOUSE_ON, COLLISION_MOUSE_CLICK, COLLISION_COLLIDED /* related to: LAST_COLLISION_TIME */ };


			enum class e_string { ID };
			enum class e_double { TARG_POSX, TARG_POSY, SCALE_X, SCALE_Y, SCALE_G, CENTER_X, CENTER_Y, TARG_ROTATION, ACCELERATION_X, ACCELERATION_Y, SPEED_ROTATION, SPEEDXY_LIMIT, ELASTICITY_X, ELASTICITY_Y };
			enum class e_boolean { DRAW, USE_COLOR, AFFECTED_BY_CAM, SHOWDOT, SHOWBOX, RESPECT_CAMERA_LIMITS, FOLLOW_MOUSE, FOLLOW_KEYBOARD };
			enum class e_integer { LAYER, COLLISION_MODE };
			enum class e_color { COLOR };

			enum class e_collision_mode_cast {
				COLLISION_BOTH, // collide and move if colliding
				COLLISION_STATIC, // collide with others and stay static
				COLLISION_INVERSE, // doesn't cause collision on others, but move if someone is colliding with it
				COLLISION_NONE // no collision behaviour at all
			};

			enum class e_direction { NORTH, SOUTH, EAST, WEST, NONE };

			enum class e_tie_functional { COLLISION_MOUSE_ON, COLLISION_MOUSE_CLICK, COLLISION_MOUSE_UNCLICK, COLLISION_COLLIDED_OTHER, COLLISION_NONE };
			constexpr auto tie_functional_size = static_cast<size_t>(e_tie_functional::COLLISION_NONE) + 1;



			/*
			class __slice {
				std::string n;
				int p;
			public:
				__slice(const std::string w, const int v) { n = w; p = v; }
				const std::string s() const { return n; }
				const int i() const { return p; }
			};
			enum class tags_e {
				T_POSX, T_POSY, T_SCREEN_POSX, T_SCREEN_POSY, T_ISFOLLOWING, T_COLOR_R, T_COLOR_G, T_COLOR_B, T_COLOR_A, T_MODE, T_TIME, T_ISUSINGBUF, T_GB_RESX, T_GB_RESY, T_REFRESHRATE, T_FPS, T_TPS, T_UPS, T_APS, T_I_FPS, T_I_TPS, T_I_UPS, T_I_APS, T_MS_FPS, T_MS_TPS, T_MS_UPS, T_MS_APS,
				T_SPRITE_FRAME, T_CAM_X, T_CAM_Y, T_CAM_ZOOM, T_CAM_ZOOMG, T_CAM_ZOOMX, T_CAM_ZOOMY, T_CURRSTRING, T_LASTSTRING, T_MOUSE_X, T_MOUSE_Y, T_SPRITE_SPEEDX, T_SPRITE_SPEEDY, T_SPRITE_NAME, T_SPRITE_ENTITY_NAME, T_SPRITE_ENTITY_HEALTH, _T_SPRITE_DEBUG, T_TEXTURES_LOADED, T_FONTS_LOADED,
				T_SPRITES_LOADED, T_TEXTS_LOADED, T_TRACKS_LOADED, T_SPRITE_STATE, T_VOLUME, T_VERSION, T_RESOLUTION_PROPORTION, T_CHROMA_FX, T_FPS_CAP, size
			};
			inline const __slice tags[] = {
				__slice("%pos_x%", Cast::a_cast(tags_e::T_POSX)),
				__slice("%pos_y%", Cast::a_cast(tags_e::T_POSY)),
				__slice("%screen_pos_x%", Cast::a_cast(tags_e::T_SCREEN_POSX)),
				__slice("%screen_pos_y%", Cast::a_cast(tags_e::T_SCREEN_POSY)),
				__slice("%is_following%", Cast::a_cast(tags_e::T_ISFOLLOWING)),
				__slice("%color_r%", Cast::a_cast(tags_e::T_COLOR_R)),
				__slice("%color_g%", Cast::a_cast(tags_e::T_COLOR_G)),
				__slice("%color_b%", Cast::a_cast(tags_e::T_COLOR_B)),
				__slice("%color_a%", Cast::a_cast(tags_e::T_COLOR_A)),
				__slice("%mode%", Cast::a_cast(tags_e::T_MODE)),
				__slice("%time%", Cast::a_cast(tags_e::T_TIME)),
				__slice("%is_using_buf%", Cast::a_cast(tags_e::T_ISUSINGBUF)),
				__slice("%curr_res_x%", Cast::a_cast(tags_e::T_GB_RESX)),
				__slice("%curr_res_y%", Cast::a_cast(tags_e::T_GB_RESY)),
				__slice("%curr_refresh_rate%", Cast::a_cast(tags_e::T_REFRESHRATE)),
				__slice("%int_fps%", Cast::a_cast(tags_e::T_FPS)),
				__slice("%int_tps%", Cast::a_cast(tags_e::T_TPS)),
				__slice("%int_ups%", Cast::a_cast(tags_e::T_UPS)),
				__slice("%int_aps%", Cast::a_cast(tags_e::T_APS)),
				__slice("%instant_fps%", Cast::a_cast(tags_e::T_I_FPS)),
				__slice("%instant_tps%", Cast::a_cast(tags_e::T_I_TPS)),
				__slice("%instant_ups%", Cast::a_cast(tags_e::T_I_UPS)),
				__slice("%instant_aps%", Cast::a_cast(tags_e::T_I_APS)),
				__slice("%ms_fps%", Cast::a_cast(tags_e::T_MS_FPS)),
				__slice("%ms_tps%", Cast::a_cast(tags_e::T_MS_TPS)),
				__slice("%ms_ups%", Cast::a_cast(tags_e::T_MS_UPS)),
				__slice("%ms_aps%", Cast::a_cast(tags_e::T_MS_APS)),
				__slice("%sprite_frame%", Cast::a_cast(tags_e::T_SPRITE_FRAME)),
				__slice("%cam_x%", Cast::a_cast(tags_e::T_CAM_X)),
				__slice("%cam_y%", Cast::a_cast(tags_e::T_CAM_Y)),
				__slice("%cam_zoom_combined%", Cast::a_cast(tags_e::T_CAM_ZOOM)),
				__slice("%cam_zoom_g%", Cast::a_cast(tags_e::T_CAM_ZOOMG)),
				__slice("%cam_zoom_x%", Cast::a_cast(tags_e::T_CAM_ZOOMX)),
				__slice("%cam_zoom_y%", Cast::a_cast(tags_e::T_CAM_ZOOMY)),
				__slice("%curr_string%", Cast::a_cast(tags_e::T_CURRSTRING)),
				__slice("%last_string%", Cast::a_cast(tags_e::T_LASTSTRING)),
				__slice("%mouse_x%", Cast::a_cast(tags_e::T_MOUSE_X)),
				__slice("%mouse_y%", Cast::a_cast(tags_e::T_MOUSE_Y)),
				__slice("%sprite_speed_x%", Cast::a_cast(tags_e::T_SPRITE_SPEEDX)),
				__slice("%sprite_speed_y%", Cast::a_cast(tags_e::T_SPRITE_SPEEDY)),
				__slice("%sprite_name%", Cast::a_cast(tags_e::T_SPRITE_NAME)),
				__slice("%entity_name%", Cast::a_cast(tags_e::T_SPRITE_ENTITY_NAME)),
				__slice("%entity_health%", Cast::a_cast(tags_e::T_SPRITE_ENTITY_HEALTH)),
				__slice("%sprite_debug%", Cast::a_cast(tags_e::_T_SPRITE_DEBUG)),
				__slice("%num_images%", Cast::a_cast(tags_e::T_TEXTURES_LOADED)),
				__slice("%num_fonts%", Cast::a_cast(tags_e::T_FONTS_LOADED)),
				__slice("%num_sprites%", Cast::a_cast(tags_e::T_SPRITES_LOADED)),
				__slice("%num_texts%", Cast::a_cast(tags_e::T_TEXTS_LOADED)),
				__slice("%num_tracks%", Cast::a_cast(tags_e::T_TRACKS_LOADED)),
				__slice("%sprite_state%", Cast::a_cast(tags_e::T_SPRITE_STATE)),
				__slice("%volume_perc%", Cast::a_cast(tags_e::T_VOLUME)),
				__slice("%version%", Cast::a_cast(tags_e::T_VERSION)),
				__slice("%screen_buf_proportion%", Cast::a_cast(tags_e::T_RESOLUTION_PROPORTION)),
				__slice("%screen_chroma_fx%", Cast::a_cast(tags_e::T_CHROMA_FX)),
				__slice("%fps_cap%", Cast::a_cast(tags_e::T_FPS_CAP))
			};*/


			constexpr double minimum_sprite_accel_collision = 1e-6;

			const SuperMap<double> e_double_readonly_defaults = {
				{0.0,				(e_double_readonly::SPEED_X),						CHAR_INIT("speed_x")},
				{0.0,				(e_double_readonly::SPEED_Y),						CHAR_INIT("speed_y")},
				{0.0,				(e_double_readonly::LAST_COLLISION_TIME),			CHAR_INIT("last_collision_time")},
				{0.0,				(e_double_readonly::LAST_DRAW),						CHAR_INIT("last_draw")},
				{0.0,				(e_double_readonly::POSX),							CHAR_INIT("pos_x")}, // drawing POSX
				{0.0,				(e_double_readonly::POSY),							CHAR_INIT("pos_y")}, // drawing POSY
				{0.0,				(e_double_readonly::ROTATION),						CHAR_INIT("rotation")}, // drawing ROTATION
				{0.0,				(e_double_readonly::COL_MINDIST_X),					CHAR_INIT("collision_min_distance_x")},
				{0.0,				(e_double_readonly::COL_MINDIST_Y),					CHAR_INIT("collision_min_distance_y")}
			};

			const SuperMap<bool> e_boolean_readonly_defaults = {
				{false,				(e_boolean_readonly::COLLISION_MOUSE_ON),			CHAR_INIT("collision_mouse_on")},
				{false,				(e_boolean_readonly::COLLISION_MOUSE_CLICK),		CHAR_INIT("collision_mouse_click")},
				{false,				(e_boolean_readonly::COLLISION_COLLIDED),			CHAR_INIT("collision_collided")}
			};

			const SuperMap<std::string> e_string_defaults = {
				{"",				(e_string::ID),										CHAR_INIT("id")}
			};

			const SuperMap<double> e_double_defaults = {
				{0.0,				(e_double::TARG_POSX),								CHAR_INIT("target_pos_x")},
				{0.0,				(e_double::TARG_POSY),								CHAR_INIT("target_pos_y")},
				{1.0,				(e_double::SCALE_X),								CHAR_INIT("scale_x")},
				{1.0,				(e_double::SCALE_Y),								CHAR_INIT("scale_y")},
				{1.0,				(e_double::SCALE_G),								CHAR_INIT("scale_g")},
				{0.0,				(e_double::CENTER_X),								CHAR_INIT("center_x")},
				{0.0,				(e_double::CENTER_Y),								CHAR_INIT("center_y")},
				{0.0,				(e_double::TARG_ROTATION),							CHAR_INIT("target_rotation")},
				{0.0,				(e_double::ACCELERATION_X),							CHAR_INIT("acceleration_x")},
				{0.0,				(e_double::ACCELERATION_Y),							CHAR_INIT("acceleration_y")},
				{0.0,				(e_double::SPEED_ROTATION),							CHAR_INIT("speed_rotation")},
				{0.3,				(e_double::SPEEDXY_LIMIT),							CHAR_INIT("speed_limit")},
				{0.85,				(e_double::ELASTICITY_X),							CHAR_INIT("elasticity_x")},
				{0.85,				(e_double::ELASTICITY_Y),							CHAR_INIT("elasticity_y")} 
			};

			const SuperMap<bool> e_boolean_defaults = {
				{true,				(e_boolean::DRAW),									CHAR_INIT("draw")},
				{false,				(e_boolean::USE_COLOR),								CHAR_INIT("use_color")},
				{true,				(e_boolean::AFFECTED_BY_CAM),						CHAR_INIT("affected_by_camera")},
				{false,				(e_boolean::SHOWDOT),								CHAR_INIT("show_dot")}, // shows dot where it will be drawn
				{false,				(e_boolean::SHOWBOX),								CHAR_INIT("show_box")}, // shows rectangle where collision updated (latest update)
				{true,				(e_boolean::RESPECT_CAMERA_LIMITS),					CHAR_INIT("respect_camera_limits")},
				{false,				(e_boolean::FOLLOW_MOUSE),							CHAR_INIT("follow_mouse")},
				{false,				(e_boolean::FOLLOW_KEYBOARD),						CHAR_INIT("follow_keyboard")}
			};

			const SuperMap<int> e_integer_defaults = {
				{0,															(e_integer::LAYER),					CHAR_INIT("layer")},
				{static_cast<int>(e_collision_mode_cast::COLLISION_BOTH),	(e_integer::COLLISION_MODE),		CHAR_INIT("collision_mode")}
			};

			const SuperMap<ALLEGRO_COLOR> e_color_defaults = {
				{ALLEGRO_COLOR(),	(e_color::COLOR),									CHAR_INIT("color")}
			};
		}

		/*
		Sprite_Base:
		- Base pra qualquer coisa (imagem, texto...)
		- Texto pode ter um tamanho definido como uma imagem, mas desenhar como texto mesmo assim ( [ área sem nada ] + texto desenhado normalmente sem problemas )
		*/

		class Sprite_Base {
			struct easier_collision_handle {
				double	posx = 0.0,
						posy = 0.0,
						sizx = 0.0, // full size
						sizy = 0.0; // full size

				int directions_cases[4] = { 0 };
				bool was_col = false;

				easier_collision_handle() = default;
				easier_collision_handle(const easier_collision_handle&);

				// test overlap between this and someone else
				bool overlap(easier_collision_handle&);
				// Where should I go?
				sprite::e_direction processResult();
				// Resets directions history
				void resetDirections();
				// X, Y, SX, SY
				void setup(const double, const double, const double, const double);
			} easy_collision;

			struct sprite_base_data {
				SuperMap<double>		double_readonly_data = sprite::e_double_readonly_defaults;
				SuperMap<bool>			boolean_readonly_data = sprite::e_boolean_readonly_defaults;
				SuperMap<std::string>	string_data = sprite::e_string_defaults;
				SuperMap<double>		double_data = sprite::e_double_defaults;
				SuperMap<bool>			boolean_data = sprite::e_boolean_defaults;
				SuperMap<int>			integer_data = sprite::e_integer_defaults;
				SuperMap<ALLEGRO_COLOR> color_data = sprite::e_color_defaults;
			} sprite_data;

			std::function<void(void)> pair_tied[sprite::tie_functional_size];
			sprite::e_tie_functional new_state = sprite::e_tie_functional::COLLISION_NONE;
			sprite::e_tie_functional last_state = sprite::e_tie_functional::COLLISION_NONE;

		protected:
			std::function<void(void)> custom_draw_task; // set this as draw() of new children (so the draw() calls this if exists for further drawing scheme)
		public:
			Sprite_Base() = default;
			Sprite_Base(Sprite_Base&);

			void hook(const sprite::e_tie_functional, const std::function<void(void)>);
			void unhook(const sprite::e_tie_functional);

			void set(const sprite::e_string, std::string);
			void set(const sprite::e_double, double);
			void set(const sprite::e_boolean, bool);
			void set(const sprite::e_integer, int);
			void set(const sprite::e_color, ALLEGRO_COLOR);
			void set(const std::string, std::string);
			void set(const std::string, double);
			void set(const std::string, bool);
			void set(const std::string, int);
			void set(const std::string, ALLEGRO_COLOR);

			bool get(const sprite::e_double_readonly, double&);
			bool get(const sprite::e_boolean_readonly, bool&);
			bool get(const sprite::e_string, std::string&);
			bool get(const sprite::e_double, double&);
			bool get(const sprite::e_boolean, bool&);
			bool get(const sprite::e_integer, int&);
			bool get(const sprite::e_color, ALLEGRO_COLOR&);
			bool get(const std::string, std::string&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, int&);
			bool get(const std::string, ALLEGRO_COLOR&);

			const double*			getRef(const sprite::e_double_readonly) const;
			const bool*				getRef(const sprite::e_boolean_readonly) const;
			std::string*			getRef(const sprite::e_string);
			double*					getRef(const sprite::e_double);
			bool*					getRef(const sprite::e_boolean);
			int*					getRef(const sprite::e_integer);
			ALLEGRO_COLOR*			getRef(const sprite::e_color);

			template<typename T, typename V> inline bool g(const T a, V& b) { return get(a, b); };
			template<typename T, typename V> inline bool s(const T a, V& b) { return set(a, b); };
			template<typename T, typename V> inline bool isEq(const T e, const V v) { V k; if (get(e, k)) return v == k; return false; }
			template<typename T, typename V> inline bool ie(const T e, const V v) { return isEq(e, v); };

			// ALWAYS CALL NATIVE DRAW FROM SPRITE_BASE! | Camera is useful to make it consistent
			void draw(Camera*);
			
			// camera is useful for consistent run
			void collide(Camera*, Sprite_Base&);
			// camera is useful for consistent run, also updates easier_collision_handle for further collide()
			void update(Camera*);

			sprite_base_data& copyRAW();
			// YOU SHALL update() BEFORE TESTING COLLISION!
			easier_collision_handle& getCollision();
		};

	}
}