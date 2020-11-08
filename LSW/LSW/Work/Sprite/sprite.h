#pragma once

/*
 * SPRITE_BASE WILL ASSUME GAMECORE MOUSE CONFIG FORMAT, EVEN THOUGH IT'S NOT DEPENDENT.
 * 
 * This basically means it will only work 10/10 if you set config with:
 * 
 * FORMAT: '[ SECTION, KEY ]: description; type'
 * 
 * ## MOUSE CONFIGURATION ##
 * - [ "mouse", "x" ]:				Position X; float
 * - [ "mouse", "y" ]:				Position Y; float
 * - [ "mouse", "press_count" ]:	Pressed buttons count; unsigned
 * - [ "mouse", "down_latest" ]:	Latest event, from what key, starting with 1 for button 0, negative for release; int
 * - [ "mouse", "b#" ]:				Button # (placeholder) latest state, if not set, never had one; bool
*/

#include "../../Handling/Abort/abort.h"
#include "../../Tools/SuperMap/SuperMap.h"
#include "../../Tools/SuperFunctionalMap/superfunctionalmap.h"
#include "../../Tools/SuperMutex/supermutex.h"
#include "../../Interface/Camera/camera.h"
#include "../../Interface/Config/config.h"
#include "../../Interface/Color/color.h"

namespace LSW {
	namespace v5 {
		namespace Work {

			namespace sprite {
				/*
				COL_MINDIST_... = the distance it has to move in ... (on a collision tick)
				...UPDATE... = automatic smoothing based on collision calls
				*/
				enum class e_double_readonly { SPEED_X, SPEED_Y, LAST_COLLISION_TIME /* related to: COLLISION_COLLIDED */, LAST_DRAW, LAST_UPDATE, UPDATE_DELTA, POSX, POSY, ROTATION, COL_MINDIST_X, COL_MINDIST_Y };
				enum class e_boolean_readonly { COLLISION_MOUSE_ON, COLLISION_MOUSE_CLICK, COLLISION_COLLIDED /* related to: LAST_COLLISION_TIME */ };
				enum class e_tief_readonly { LAST_STATE };

				enum class e_string { ID };
				enum class e_double { TARG_POSX, TARG_POSY, SCALE_X, SCALE_Y, SCALE_G, CENTER_X, CENTER_Y, TARG_ROTATION, ACCELERATION_X, ACCELERATION_Y, SPEEDXY_LIMIT, ELASTICITY_X, ELASTICITY_Y, ROUGHNESS };
				enum class e_boolean { DRAW, USE_COLOR, AFFECTED_BY_CAM, SHOWDOT, SHOWBOX, RESPECT_CAMERA_LIMITS, SET_TARG_POS_VALUE_READONLY /*Readonly means no collision or acceleration, just f()*/ };
				enum class e_integer { COLLISION_MODE };
				enum class e_color { COLOR };
				enum class e_uintptrt { DATA_FROM };

				enum class e_collision_mode_cast {
					COLLISION_BOTH, // collide and move if colliding
					COLLISION_STATIC, // collide with others and stay static
					COLLISION_INVERSE, // doesn't cause collision on others, but move if someone is colliding with it
					COLLISION_NONE // no collision behaviour at all
				};

				enum class e_direction { NORTH, SOUTH, EAST, WEST, NONE };

				enum class e_tie_functional { DELAYED_WORK_AUTODEL, COLLISION_MOUSE_ON, COLLISION_MOUSE_CLICK, COLLISION_MOUSE_UNCLICK, COLLISION_COLLIDED_OTHER, COLLISION_NONE };
				constexpr auto tie_functional_size = static_cast<size_t>(e_tie_functional::COLLISION_NONE) + 1;

				using functional = std::function<void(void)>;

				constexpr double minimum_sprite_accel_collision = 1e-6;
				constexpr double game_collision_oversize = 1e-3;
				constexpr double maximum_time_between_collisions = 1.0; // sec

				const Tools::SuperMap<std::function<std::string(void)>> e_string_defaults = {
					{[] {return "";},															(e_string::ID),										("id")}
				};

				const Tools::SuperMap<std::function<double(void)>> e_double_defaults = {
					{[] {return 0.0;},															(e_double_readonly::SPEED_X),						("speed_x")},
					{[] {return 0.0;},															(e_double_readonly::SPEED_Y),						("speed_y")},
					{[] {return 0.0;},															(e_double_readonly::LAST_COLLISION_TIME),			("last_collision_time")},
					{[] {return 0.0;},															(e_double_readonly::LAST_DRAW),						("last_draw")},
					{[] {return 0.0;},															(e_double_readonly::LAST_UPDATE),					("last_update")},
					{[] {return 0.0;},															(e_double_readonly::UPDATE_DELTA),					("update_delta")},
					{[] {return 0.0;},															(e_double_readonly::POSX),							("pos_x")}, // drawing POSX
					{[] {return 0.0;},															(e_double_readonly::POSY),							("pos_y")}, // drawing POSY
					{[] {return 0.0;},															(e_double_readonly::ROTATION),						("rotation")}, // drawing ROTATION
					{[] {return 0.0;},															(e_double_readonly::COL_MINDIST_X),					("collision_min_distance_x")},
					{[] {return 0.0;},															(e_double_readonly::COL_MINDIST_Y),					("collision_min_distance_y")},

					{[] {return 0.0;},															(e_double::TARG_POSX),								("target_pos_x")},
					{[] {return 0.0;},															(e_double::TARG_POSY),								("target_pos_y")},
					{[] {return 1.0;},															(e_double::SCALE_X),								("scale_x")},
					{[] {return 1.0;},															(e_double::SCALE_Y),								("scale_y")},
					{[] {return 1.0;},															(e_double::SCALE_G),								("scale_g")},
					{[] {return 0.0;},															(e_double::CENTER_X),								("center_x")},
					{[] {return 0.0;},															(e_double::CENTER_Y),								("center_y")},
					{[] {return 0.0;},															(e_double::TARG_ROTATION),							("target_rotation")},
					{[] {return 0.0;},															(e_double::ACCELERATION_X),							("acceleration_x")},
					{[] {return 0.0;},															(e_double::ACCELERATION_Y),							("acceleration_y")},
					{[] {return 0.3;},															(e_double::SPEEDXY_LIMIT),							("speed_limit")},
					{[] {return 0.85;},															(e_double::ELASTICITY_X),							("elasticity_x")},
					{[] {return 0.85;},															(e_double::ELASTICITY_Y),							("elasticity_y")},
					{[] {return 0.9999;},														(e_double::ROUGHNESS),								("roughness")}
				};

				const Tools::SuperMap<std::function<bool(void)>> e_boolean_defaults = {
					{[] {return false;},														(e_boolean_readonly::COLLISION_MOUSE_ON),			("collision_mouse_on")},
					{[] {return false;},														(e_boolean_readonly::COLLISION_MOUSE_CLICK),		("collision_mouse_click")},
					{[] {return false;},														(e_boolean_readonly::COLLISION_COLLIDED),			("collision_collided")},
					{[] {return true;},															(e_boolean::DRAW),									("draw")},
					{[] {return false;},														(e_boolean::USE_COLOR),								("use_color")},
					{[] {return true;},															(e_boolean::AFFECTED_BY_CAM),						("affected_by_camera")},
					{[] {return false;},														(e_boolean::SHOWDOT),								("show_dot")}, // shows dot where it will be drawn
					{[] {return false;},														(e_boolean::SHOWBOX),								("show_box")}, // shows rectangle where collision updated (latest update)
					{[] {return true;},															(e_boolean::RESPECT_CAMERA_LIMITS),					("respect_camera_limits")},
					{[] {return false;},														(e_boolean::SET_TARG_POS_VALUE_READONLY),			("set_targ_pos_value_readonly")}
				};

				const Tools::SuperMap<std::function<int(void)>> e_integer_defaults = {
					{[] {return static_cast<int>(e_collision_mode_cast::COLLISION_BOTH);},		(e_integer::COLLISION_MODE),						("collision_mode")}
				};

				const Tools::SuperMap<std::function<Interface::Color(void)>> e_color_defaults = {
					{[] {return Interface::Color();},											(e_color::COLOR),									("color")}
				};

				const Tools::SuperMap<std::function<uintptr_t(void)>> e_uintptrt_defaults = {
					{[] {return (uintptr_t)0;},													(e_uintptrt::DATA_FROM)}
				};

				const Tools::SuperMap<std::function<e_tie_functional(void)>> e_tief_defaults = {
					{[] {return e_tie_functional::COLLISION_NONE;},								(e_tief_readonly::LAST_STATE)}
				};

				const Tools::SuperMap<std::function<functional(void)>> e_functional_defaults = {
					{[] {return functional(); },												(e_tie_functional::DELAYED_WORK_AUTODEL)},
					{[] {return functional(); },												(e_tie_functional::COLLISION_MOUSE_ON)},
					{[] {return functional(); },												(e_tie_functional::COLLISION_MOUSE_CLICK)},
					{[] {return functional(); },												(e_tie_functional::COLLISION_MOUSE_UNCLICK)},
					{[] {return functional(); },												(e_tie_functional::COLLISION_COLLIDED_OTHER)},
					{[] {return functional(); },												(e_tie_functional::COLLISION_NONE)}
				};
			}

			/*
			Sprite_Base:
			- Base pra qualquer coisa (imagem, texto...)
			- Texto pode ter um tamanho definido como uma imagem, mas desenhar como texto mesmo assim ( [ área sem nada ] + texto desenhado normalmente sem problemas )
			*/

			// HAS: double, bool, std::string, int, Interface::Color, uintptr_t
			class Sprite_Base :
				public Tools::SuperFunctionalMap<double>,public Tools::SuperFunctionalMap<bool>,public Tools::SuperFunctionalMap<std::string>,public Tools::SuperFunctionalMap<int>,public Tools::SuperFunctionalMap<Interface::Color>,public Tools::SuperFunctionalMap<uintptr_t>,public Tools::SuperFunctionalMap<sprite::functional>,public Tools::SuperFunctionalMap<sprite::e_tie_functional>
			{
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
					bool overlap(const easier_collision_handle&);
					// Where should I go?
					sprite::e_direction process_result();
					// Resets directions history
					void reset_directions();
					// X, Y, SX, SY
					void setup(const double, const double, const double, const double);
				} easy_collision;

			protected:
				std::vector<std::string> collision_list, copy_final;
				Tools::SuperMutex copy_final_m;

				std::function<void(void)> custom_draw_task; // set this as draw() of new children (so the draw() calls this if exists for further drawing scheme)
				std::function<void(void)> custom_think_task; // set this as a collision / every tick function (so update() calls this if exists for further tasking)
			public:
				using Tools::SuperFunctionalMap<double>::set;
				using Tools::SuperFunctionalMap<double>::get;
				using Tools::SuperFunctionalMap<double>::get_direct;
				using Tools::SuperFunctionalMap<int>::set;
				using Tools::SuperFunctionalMap<int>::get;
				using Tools::SuperFunctionalMap<int>::get_direct;
				using Tools::SuperFunctionalMap<Interface::Color>::set;
				using Tools::SuperFunctionalMap<Interface::Color>::get;
				using Tools::SuperFunctionalMap<Interface::Color>::get_direct;
				using Tools::SuperFunctionalMap<std::string>::set;
				using Tools::SuperFunctionalMap<std::string>::get;
				using Tools::SuperFunctionalMap<std::string>::get_direct;
				using Tools::SuperFunctionalMap<bool>::set;
				using Tools::SuperFunctionalMap<bool>::get;
				using Tools::SuperFunctionalMap<bool>::get_direct;
				using Tools::SuperFunctionalMap<uintptr_t>::set;
				using Tools::SuperFunctionalMap<uintptr_t>::get;
				using Tools::SuperFunctionalMap<uintptr_t>::get_direct;
				using Tools::SuperFunctionalMap<sprite::functional>::set;
				using Tools::SuperFunctionalMap<sprite::functional>::get;
				using Tools::SuperFunctionalMap<sprite::functional>::get_direct;
				using Tools::SuperFunctionalMap<sprite::e_tie_functional>::set;
				using Tools::SuperFunctionalMap<sprite::e_tie_functional>::get;
				using Tools::SuperFunctionalMap<sprite::e_tie_functional>::get_direct;

				Sprite_Base();
				Sprite_Base(const Sprite_Base&);
				Sprite_Base(Sprite_Base&&);

				//void hook(const sprite::e_tie_functional, const std::function<void(void)>);
				//void unhook(const sprite::e_tie_functional);

				// set to blindly use this comrade's attributes as theirs
				void twin_up_attributes(const Sprite_Base&);

				void operator=(const Sprite_Base&);
				void operator=(Sprite_Base&&);


				//template<typename T, typename V> inline bool is_eq(const T& e, const std::function<V(void)>& v) const { auto* i = get_direct<V*>(e); return *i == v || (*i)() == v(); }
				template<typename T, typename V> inline bool is_eq(const T& e, const V& v) const { return get_direct<V>(e) == v; }
				template<typename V, typename T> inline bool is_eq(const T& e, const Sprite_Base& s) const { return get_direct<V>(e) == s.get_direct<V>(e); }

				// ALWAYS CALL NATIVE DRAW FROM SPRITE_BASE! | Camera is useful to make it consistent
				bool draw(const Interface::Camera&);
			
				// camera is useful for consistent run, bool to run even though it's a reference to someone?
				void collide(const Sprite_Base&, const bool = false);
			
				// chech header information if you're not using GameCore (for config), bool to run even though it's a reference to someone?
				void update_and_clear(const Interface::Config&, const bool = false);

				// YOU SHALL update() BEFORE TESTING COLLISION!
				//easier_collision_handle& get_collision();

				// list of IDs (internal IDs)
				const std::vector<std::string> get_collided_with_list();
			};

		}
	}
}