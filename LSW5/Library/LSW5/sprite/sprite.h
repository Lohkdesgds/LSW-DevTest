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