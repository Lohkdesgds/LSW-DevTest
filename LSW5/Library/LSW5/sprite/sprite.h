#pragma once

// C
#define _CRT_RAND_S
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
			enum class e_double { TARG_POSX, TARG_POSY, SCALE_X, SCALE_Y, SCALE_G, CENTER_X, CENTER_Y, TARG_ROTATION, ACCELERATION_X, ACCELERATION_Y, SPEEDXY_LIMIT, ELASTICITY_X, ELASTICITY_Y };
			enum class e_boolean { DRAW, USE_COLOR, AFFECTED_BY_CAM, SHOWDOT, SHOWBOX, RESPECT_CAMERA_LIMITS, FOLLOW_MOUSE, FOLLOW_KEYBOARD, SET_TARG_POS_VALUE_READONLY /*Readonly means no collision or acceleration, just f()*/ };
			enum class e_integer { LAYER, COLLISION_MODE };
			enum class e_color { COLOR };

			enum class e_collision_mode_cast {
				COLLISION_BOTH, // collide and move if colliding
				COLLISION_STATIC, // collide with others and stay static
				COLLISION_INVERSE, // doesn't cause collision on others, but move if someone is colliding with it
				COLLISION_NONE // no collision behaviour at all
			};

			enum class e_direction { NORTH, SOUTH, EAST, WEST, NONE };

			enum class e_tie_functional { DELAYED_WORK, COLLISION_MOUSE_ON, COLLISION_MOUSE_CLICK, COLLISION_MOUSE_UNCLICK, COLLISION_COLLIDED_OTHER, COLLISION_NONE };
			constexpr auto tie_functional_size = static_cast<size_t>(e_tie_functional::COLLISION_NONE) + 1;


			constexpr double minimum_sprite_accel_collision = 1e-6;

			const SuperMap<double> e_double_readonly_defaults = {
				{0.0,																		(e_double_readonly::SPEED_X),						CHAR_INIT("speed_x")},
				{0.0,																		(e_double_readonly::SPEED_Y),						CHAR_INIT("speed_y")},
				{0.0,																		(e_double_readonly::LAST_COLLISION_TIME),			CHAR_INIT("last_collision_time")},
				{0.0,																		(e_double_readonly::LAST_DRAW),						CHAR_INIT("last_draw")},
				{0.0,																		(e_double_readonly::POSX),							CHAR_INIT("pos_x")}, // drawing POSX
				{0.0,																		(e_double_readonly::POSY),							CHAR_INIT("pos_y")}, // drawing POSY
				{0.0,																		(e_double_readonly::ROTATION),						CHAR_INIT("rotation")}, // drawing ROTATION
				{0.0,																		(e_double_readonly::COL_MINDIST_X),					CHAR_INIT("collision_min_distance_x")},
				{0.0,																		(e_double_readonly::COL_MINDIST_Y),					CHAR_INIT("collision_min_distance_y")}
			};

			const SuperMap<bool> e_boolean_readonly_defaults = {
				{false,																		(e_boolean_readonly::COLLISION_MOUSE_ON),			CHAR_INIT("collision_mouse_on")},
				{false,																		(e_boolean_readonly::COLLISION_MOUSE_CLICK),		CHAR_INIT("collision_mouse_click")},
				{false,																		(e_boolean_readonly::COLLISION_COLLIDED),			CHAR_INIT("collision_collided")}
			};

			const SuperMap<std::function<std::string(void)>> e_string_defaults = {
				{[] {return "";},															(e_string::ID),										CHAR_INIT("id")}
			};

			const SuperMap<std::function<double(void)>> e_double_defaults = {
				{[] {return 0.0;},															(e_double::TARG_POSX),								CHAR_INIT("target_pos_x")},
				{[] {return 0.0;},															(e_double::TARG_POSY),								CHAR_INIT("target_pos_y")},
				{[] {return 1.0;},															(e_double::SCALE_X),								CHAR_INIT("scale_x")},
				{[] {return 1.0;},															(e_double::SCALE_Y),								CHAR_INIT("scale_y")},
				{[] {return 1.0;},															(e_double::SCALE_G),								CHAR_INIT("scale_g")},
				{[] {return 0.0;},															(e_double::CENTER_X),								CHAR_INIT("center_x")},
				{[] {return 0.0;},															(e_double::CENTER_Y),								CHAR_INIT("center_y")},
				{[] {return 0.0;},															(e_double::TARG_ROTATION),							CHAR_INIT("target_rotation")},
				{[] {return 0.0;},															(e_double::ACCELERATION_X),							CHAR_INIT("acceleration_x")},
				{[] {return 0.0;},															(e_double::ACCELERATION_Y),							CHAR_INIT("acceleration_y")},
				{[] {return 0.3;},															(e_double::SPEEDXY_LIMIT),							CHAR_INIT("speed_limit")},
				{[] {return 0.85;},															(e_double::ELASTICITY_X),							CHAR_INIT("elasticity_x")},
				{[] {return 0.85;},															(e_double::ELASTICITY_Y),							CHAR_INIT("elasticity_y")}
			};

			const SuperMap<std::function<bool(void)>> e_boolean_defaults = {
				{[] {return true;},															(e_boolean::DRAW),									CHAR_INIT("draw")},
				{[] {return false;},														(e_boolean::USE_COLOR),								CHAR_INIT("use_color")},
				{[] {return true;},															(e_boolean::AFFECTED_BY_CAM),						CHAR_INIT("affected_by_camera")},
				{[] {return false;},														(e_boolean::SHOWDOT),								CHAR_INIT("show_dot")}, // shows dot where it will be drawn
				{[] {return false;},														(e_boolean::SHOWBOX),								CHAR_INIT("show_box")}, // shows rectangle where collision updated (latest update)
				{[] {return true;},															(e_boolean::RESPECT_CAMERA_LIMITS),					CHAR_INIT("respect_camera_limits")},
				{[] {return false;},														(e_boolean::FOLLOW_MOUSE),							CHAR_INIT("follow_mouse")},
				{[] {return false;},														(e_boolean::FOLLOW_KEYBOARD),						CHAR_INIT("follow_keyboard")},
				{[] {return false;},														(e_boolean::SET_TARG_POS_VALUE_READONLY),			CHAR_INIT("set_targ_pos_value_readonly")}
			};

			const SuperMap<std::function<int(void)>> e_integer_defaults = {
				{[] {return 0;},															(e_integer::LAYER),									CHAR_INIT("layer")},
				{[] {return static_cast<int>(e_collision_mode_cast::COLLISION_BOTH);},		(e_integer::COLLISION_MODE),						CHAR_INIT("collision_mode")}
			};

			const SuperMap<std::function<ALLEGRO_COLOR(void)>> e_color_defaults = {
				{[] {return ALLEGRO_COLOR();},												(e_color::COLOR),									CHAR_INIT("color")}
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
				SuperMap<double>								double_readonly_data = sprite::e_double_readonly_defaults;
				SuperMap<bool>									boolean_readonly_data = sprite::e_boolean_readonly_defaults;
				SuperMap<std::function<std::string(void)>>		string_data = sprite::e_string_defaults;
				SuperMap<std::function<double(void)>>			double_data = sprite::e_double_defaults;
				SuperMap<std::function<bool(void)>>				boolean_data = sprite::e_boolean_defaults;
				SuperMap<std::function<int(void)>>				integer_data = sprite::e_integer_defaults;
				SuperMap<std::function<ALLEGRO_COLOR(void)>>	color_data = sprite::e_color_defaults;
				void* original_this = nullptr;
			};

			std::shared_ptr<sprite_base_data> data_sprite_base = std::make_shared<sprite_base_data>();

			std::function<void(void)> pair_tied[sprite::tie_functional_size];
			sprite::e_tie_functional new_state = sprite::e_tie_functional::COLLISION_NONE;
			sprite::e_tie_functional last_state = sprite::e_tie_functional::COLLISION_NONE;

		protected:
			std::function<void(void)> custom_draw_task; // set this as draw() of new children (so the draw() calls this if exists for further drawing scheme)
			std::function<void(void)> custom_think_task; // set this as a collision / every tick function (so update() calls this if exists for further tasking)
		public:
			Sprite_Base();
			Sprite_Base(Sprite_Base&);

			void hook(const sprite::e_tie_functional, const std::function<void(void)>);
			void unhook(const sprite::e_tie_functional);

			// set to blindly use this comrade's attributes as theirs
			void twinUpAttributes(const std::shared_ptr<sprite_base_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<sprite_base_data> getAttributes();

			void set(const sprite::e_string, std::string);
			void set(const sprite::e_double, double);
			void set(const sprite::e_boolean, bool);
			void set(const sprite::e_integer, int);
			void set(const sprite::e_color, ALLEGRO_COLOR);
			void set(const sprite::e_string, std::function<std::string(void)>);
			void set(const sprite::e_double, std::function<double(void)>);
			void set(const sprite::e_boolean, std::function<bool(void)>);
			void set(const sprite::e_integer, std::function<int(void)>);
			void set(const sprite::e_color, std::function<ALLEGRO_COLOR(void)>);
			void set(const std::string, std::string);
			void set(const std::string, double);
			void set(const std::string, bool);
			void set(const std::string, int);
			void set(const std::string, ALLEGRO_COLOR);
			void set(const std::string, std::function<std::string(void)>);
			void set(const std::string, std::function< double(void)>);
			void set(const std::string, std::function<bool(void)>);
			void set(const std::string, std::function<int(void)>);
			void set(const std::string, std::function<ALLEGRO_COLOR(void)>);

			bool get(const sprite::e_double_readonly, double&);
			bool get(const sprite::e_boolean_readonly, bool&);
			bool get(const sprite::e_string, std::string&);
			bool get(const sprite::e_double, double&);
			bool get(const sprite::e_boolean, bool&);
			bool get(const sprite::e_integer, int&);
			bool get(const sprite::e_color, ALLEGRO_COLOR&);
			bool get(const sprite::e_string, std::function<std::string(void)>&);
			bool get(const sprite::e_double, std::function<double(void)>&);
			bool get(const sprite::e_boolean, std::function<bool(void)>&);
			bool get(const sprite::e_integer, std::function<int(void)>&);
			bool get(const sprite::e_color, std::function<ALLEGRO_COLOR(void)>&);
			bool get(const std::string, std::string&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, int&);
			bool get(const std::string, ALLEGRO_COLOR&);
			bool get(const std::string, std::function<std::string(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<bool(void)>&);
			bool get(const std::string, std::function<int(void)>&);
			bool get(const std::string, std::function<ALLEGRO_COLOR(void)>&);

			const double*								getRef(const sprite::e_double_readonly) const;
			const bool*									getRef(const sprite::e_boolean_readonly) const;
			std::function<std::string(void)>*			getRef(const sprite::e_string);
			std::function<double(void)>*				getRef(const sprite::e_double);
			std::function<bool(void)>*					getRef(const sprite::e_boolean);
			std::function<int(void)>*					getRef(const sprite::e_integer);
			std::function<ALLEGRO_COLOR(void)>*			getRef(const sprite::e_color);

			template<typename T, typename V> inline bool isEq(const T e, const std::function<V(void)> v) { V k; if (get(e, k)) return v() == k; return false; }
			template<typename T, typename V> inline bool isEq(const T e, const V v) { V k; if (get(e, k)) return v == k; return false; }

			// ALWAYS CALL NATIVE DRAW FROM SPRITE_BASE! | Camera is useful to make it consistent (has to be in layer size_t)
			bool draw(Camera*, int);
			// ALWAYS CALL NATIVE DRAW FROM SPRITE_BASE! | Camera is useful to make it consistent (bool is check if it is on any camera layer)
			bool draw(Camera*, bool = true);
			
			// camera is useful for consistent run
			void collide(Camera*, Sprite_Base&);
			// camera is useful for consistent run, also updates easier_collision_handle for further collide()
			void update(Camera*);

			// YOU SHALL update() BEFORE TESTING COLLISION!
			easier_collision_handle& getCollision();
		};

	}
}