#include "sprite.h"


namespace LSW {
	namespace v5 {
		namespace Work {

			Sprite_Base::easier_collision_handle::easier_collision_handle(const easier_collision_handle& cpy)
			{
				posx = cpy.posx;
				posy = cpy.posy;
				sizx = cpy.sizx;
				sizy = cpy.sizy;
				dx_max = cpy.dx_max;
				dy_max = cpy.dy_max;
				memcpy_s(&directions_cases, sizeof(directions_cases), &cpy.directions_cases, sizeof(cpy.directions_cases));
				was_col = cpy.was_col;
			}

			bool Sprite_Base::easier_collision_handle::overlap(const easier_collision_handle& ol) // me == P1
			{
				double dist_x = (posx - ol.posx);
				double diff_x = fabs(dist_x) - (sizx + ol.sizx) * 1.0 / 2.0; // if < 0, col
				bool col_x = diff_x < 0.0;
				double dist_y = (posy - ol.posy);
				double diff_y = fabs(dist_y) - (sizy + ol.sizy) * 1.0 / 2.0; // if < 0, col
				bool col_y = diff_y < 0.0;
				bool is_col = col_y && col_x;

				if (is_col) {
					if (fabs(diff_x) < fabs(diff_y)) {
						if (fabs(diff_x) > fabs(dx_max)) dx_max = diff_x;
						
						if (dist_x > 0.0) {
							directions_cases[static_cast<int>(sprite::e_direction_array_version::WEST)]++; // VEM DO WEST
						}
						else {
							directions_cases[static_cast<int>(sprite::e_direction_array_version::EAST)]++; // VEM DO EAST
						}
					}
					else {
						if (fabs(diff_y) > fabs(dy_max)) dy_max = diff_y;
						if (dist_y > 0.0) {
							directions_cases[static_cast<int>(sprite::e_direction_array_version::NORTH)]++; // VEM DO NORTH
						}
						else {
							directions_cases[static_cast<int>(sprite::e_direction_array_version::SOUTH)]++; // VEM DO SOUTH
						}
					}
				}

				/*
				Y:
				P1 - P2 > 0 == col vem do NORTH
				P1 - P2 < 0 == col vem do SOUTH

				X:
				P1 - P2 > 0 == col vem do WEST
				P1 - P2 < 0 == col vem do EAST
				*/

				was_col |= is_col;
				return is_col;
			}

			int Sprite_Base::easier_collision_handle::process_result()
			{
				bool n, s, w, e;
				n = directions_cases[static_cast<int>(sprite::e_direction_array_version::NORTH)] > 0;
				s = directions_cases[static_cast<int>(sprite::e_direction_array_version::SOUTH)] > 0;
				w = directions_cases[static_cast<int>(sprite::e_direction_array_version::WEST)] > 0;
				e = directions_cases[static_cast<int>(sprite::e_direction_array_version::EAST)] > 0;

				const int east = static_cast<int>(sprite::e_direction::EAST);
				const int west = static_cast<int>(sprite::e_direction::WEST);
				const int north = static_cast<int>(sprite::e_direction::NORTH);
				const int south = static_cast<int>(sprite::e_direction::SOUTH);


				if (n) { // north
					if (w && !e) {
						return south | east; // GOTO EAST
					}
					if (e && !w) {
						return south | west; // GOTO WEST
					}
					if (s) {
						if (directions_cases[static_cast<int>(sprite::e_direction_array_version::NORTH)] > directions_cases[static_cast<int>(sprite::e_direction_array_version::SOUTH)]) return south;
						if (directions_cases[static_cast<int>(sprite::e_direction_array_version::SOUTH)] > directions_cases[static_cast<int>(sprite::e_direction_array_version::NORTH)]) return north;
						return 0; // NO GOTO
					}
					return south; // GOTO SOUTH
				}
				else if (s) { //south
					if (w && !e) {
						return north | east; // GOTO EAST
					}
					if (e && !w) {
						return north | west; // GOTO WEST
					}
					return north; // GOTO NORTH
				}
				else if (w) { // west
					if (e) {
						if (directions_cases[static_cast<int>(sprite::e_direction_array_version::EAST)] > directions_cases[static_cast<int>(sprite::e_direction_array_version::WEST)]) return west;
						if (directions_cases[static_cast<int>(sprite::e_direction_array_version::WEST)] > directions_cases[static_cast<int>(sprite::e_direction_array_version::EAST)]) return east;
						return 0;
					}
					return east;
				}
				else if (e) { // east
					return west;
				}
				return 0;
			}

			void Sprite_Base::easier_collision_handle::reset_directions()
			{
				for (auto& i : directions_cases) i = 0;
				dx_max = dy_max = 0.0;
			}

			void Sprite_Base::easier_collision_handle::setup(const double px, const double py, const double sx, const double sy)
			{
				posx = px;
				posy = py;
				sizx = sprite::game_collision_oversize + sx;
				sizy = sprite::game_collision_oversize + sy;
				reset_directions();
				was_col = false;
			}


			Sprite_Base::Sprite_Base()
			{
				/*if (!data_sprite_base) throw Handling::Abort(__FUNCSIG__, "Failed to create Sprite base's data!");
				data_sprite_base->original_this = (void*)this;*/
				Handling::init_basic();
				Handling::init_graphics();

				set<sprite::functional>(sprite::e_functional_defaults);
				set<double>(sprite::e_double_defaults);
				set<bool>(sprite::e_boolean_defaults);
				set<std::string>(sprite::e_string_defaults);
				set<int>(sprite::e_integer_defaults);
				set<Interface::Color>(sprite::e_color_defaults);
				set(sprite::e_uintptrt::DATA_FROM, (uintptr_t)this);
			}

			Sprite_Base::Sprite_Base(const Sprite_Base& other) // copy
			{
				*this = other;
			}

			Sprite_Base::Sprite_Base(Sprite_Base&& other)
			{
				*this = std::move(other);
			}

			/*void Sprite_Base::hook(const sprite::e_tie_functional e, const std::function<void(void)> f)
			{
				pair_tied[static_cast<size_t>(e)] = f;
			}

			void Sprite_Base::unhook(const sprite::e_tie_functional e)
			{
				hook(e, std::function<void(void)>());
			}*/

			void Sprite_Base::clone(const Sprite_Base& other) // reference
			{
				set<sprite::functional>(*other.get<sprite::functional>());
				set<sprite::e_tie_functional>(*other.get<sprite::e_tie_functional>());
				set<double>(*other.get<double>());
				set<bool>(*other.get<bool>());
				set<std::string>(*other.get<std::string>());
				set<int>(*other.get<int>());
				set<Interface::Color>(*other.get<Interface::Color>());
				set<uintptr_t>(*other.get<uintptr_t>());
			}

			void Sprite_Base::operator=(const Sprite_Base& oth)
			{
				set<sprite::functional>(oth.get<sprite::functional>());
				set<sprite::e_tie_functional>(oth.get<sprite::e_tie_functional>());
				set<double>(oth.get<double>());
				set<bool>(oth.get<bool>());
				set<std::string>(oth.get<std::string>());
				set<int>(oth.get<int>());
				set<Interface::Color>(oth.get<Interface::Color>());
				set<uintptr_t>(oth.get<uintptr_t>());
			}

			void Sprite_Base::operator=(Sprite_Base&& other)
			{
				set<sprite::functional>(std::move(other.get<sprite::functional>()));
				set<sprite::e_tie_functional>(std::move(other.get<sprite::e_tie_functional>()));
				set<double>(std::move(other.get<double>()));
				set<bool>(std::move(other.get<bool>()));
				set<std::string>(std::move(other.get<std::string>()));
				set<int>(std::move(other.get<int>()));
				set<Interface::Color>(std::move(other.get<Interface::Color>()));
				set<uintptr_t>(std::move(other.get<uintptr_t>()));
			}

			bool Sprite_Base::draw(const Interface::Camera& cam)
			{

				if (get_direct<uintptr_t>(sprite::e_uintptrt::DATA_FROM) == (uintptr_t)this) { // only original copy should update parameters
					//if (is_eq(sprite::e_boolean::SHOWBOX, true) || is_eq(sprite::e_boolean::SHOWDOT, true)) {

					const auto affected_by_cam = get_direct<bool>(sprite::e_boolean::AFFECTED_BY_CAM);
					const auto targ_rotation = get_direct<double>(sprite::e_double::TARG_ROTATION);
					const auto real_targ_posx = get_direct<double>(sprite::e_double::TARG_POSX);
					const auto real_targ_posy = get_direct<double>(sprite::e_double::TARG_POSY);
					const auto targ_posx = get_direct<double>(sprite::e_double_readonly::PROJECTED_POSX);
					const auto targ_posy = get_direct<double>(sprite::e_double_readonly::PROJECTED_POSY);
					const auto scale_g = get_direct<double>(sprite::e_double::SCALE_G);
					const auto scale_x = get_direct<double>(sprite::e_double::SCALE_X);
					const auto scale_y = get_direct<double>(sprite::e_double::SCALE_Y);
					const auto update_delta = get_direct<double>(sprite::e_double_readonly::UPDATE_DELTA);
					const auto last_draw_v = get_direct<double>(sprite::e_double_readonly::LAST_DRAW);
					const auto rotation_v = get_direct<double>(sprite::e_double_readonly::ROTATION);
					const auto posx_v = get_direct<double>(sprite::e_double_readonly::POSX);
					const auto posy_v = get_direct<double>(sprite::e_double_readonly::POSY);					


					if (affected_by_cam) {
						cam.apply();
					}
					else {
						Interface::Camera clean_camera = cam;
						clean_camera.classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);
						clean_camera.apply();
					}

					// delta T calculation

					double timee = al_get_time();
					double dt = timee - last_draw_v;
					set(sprite::e_double_readonly::LAST_DRAW, timee);

					double perc_run = (update_delta > 0.0 ? (0.5 / update_delta) : 0.1) * pow(dt, 0.86);		// ex: 5 per sec * 0.2 (1/5 sec) = 1, so posx = actual posx...
					if (perc_run > 1.0) perc_run = 1.0;					// 1.0 is "set value"
					if (perc_run < 1.0 / 10000) perc_run = 1.0 / 10000; // can't be infinitely smooth right? come on

					// new position calculation

					set(sprite::e_double_readonly::ROTATION, (1.0 - perc_run) * rotation_v + perc_run * targ_rotation);
					set(sprite::e_double_readonly::POSX, (1.0 - perc_run) * posx_v + perc_run * targ_posx);
					set(sprite::e_double_readonly::POSY, (1.0 - perc_run) * posy_v + perc_run * targ_posy);

					//rotation = [=] {return (1.0 - perc_run) * rotation_v + perc_run * targ_rotation; };
					//posx = [=] {return (1.0 - perc_run) * posx_v + perc_run * targ_posx; };
					//posy = [=] {return (1.0 - perc_run) * posy_v + perc_run * targ_posy; };

					if (is_eq(sprite::e_boolean::SHOWBOX, true)) {

						const double calculated_scale_x = scale_g * scale_x;
						const double calculated_scale_y = scale_g * scale_y;

						al_draw_filled_rectangle(
							/* X1: */ real_targ_posx - calculated_scale_x * 0.5,
							/* Y1: */ real_targ_posy - calculated_scale_y * 0.5,
							/* X2: */ real_targ_posx + calculated_scale_x * 0.5,
							/* Y2: */ real_targ_posy + calculated_scale_y * 0.5,
							al_map_rgba(90, easy_collision.was_col ? 45 : 90, easy_collision.was_col ? 45 : 90, 90));
						al_draw_filled_rectangle(
							/* X1: */ real_targ_posx - calculated_scale_x * 0.515,
							/* Y1: */ real_targ_posy - calculated_scale_y * 0.515,
							/* X2: */ real_targ_posx + calculated_scale_x * 0.515,
							/* Y2: */ real_targ_posy + calculated_scale_y * 0.515,
							al_map_rgba(180, easy_collision.was_col ? 90 : 180, easy_collision.was_col ? 90 : 180, 180));
					}

					if (is_eq(sprite::e_boolean::SHOWDOT, true)) {
						al_draw_filled_circle(
							/* X1: */ posx_v,
							/* X1: */ posy_v,
							/* SCL */ 0.05f,
							al_map_rgba(90, easy_collision.was_col ? 45 : 90, easy_collision.was_col ? 45 : 90, 90));
					}
					//}
				}
				// custom_draw_task has nothing to do with clone attributes, so this is fine! (I guess so)
				if (custom_draw_task) custom_draw_task();

				return true;
			}

			void Sprite_Base::collide(const Sprite_Base& oth, const bool run_anyway) // if colliding, do not accept speed entry (keyboard player moving)
			{
				if (!run_anyway && get_direct<uintptr_t>(sprite::e_uintptrt::DATA_FROM) != (uintptr_t)this) return; // this is a copy following attrbutes from somewhere else, no duplication in collision

				if (oth.is_eq(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE))) return; // no collision
				if (is_eq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true)) return; // no collision (readonly)
				//if (oth.is_eq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true)) return; // no collision (readonly)

				if (easy_collision.overlap(oth.easy_collision)) {
					//collision_list.push_back(oth.get_direct<std::string>(sprite::e_string::ID));
				}

			}

			void Sprite_Base::update_and_clear(const Interface::Config& conf, const bool run_anyway)// needs to use ACCELERATION_X when collision, ELASTICITY_X to know how much of it goes backwards, TARG_POSX
			{
				if (!run_anyway && get_direct<uintptr_t>(sprite::e_uintptrt::DATA_FROM) != (uintptr_t)this) return; // this is a copy following attrbutes from somewhere else, no duplication in update

				{
					double update_now = al_get_time();
					double last_update = get_direct<double>(sprite::e_double_readonly::LAST_UPDATE);
					if (update_now - last_update > sprite::maximum_time_between_collisions) last_update = update_now - 1.0;
					set(sprite::e_double_readonly::LAST_UPDATE, update_now);
					set(sprite::e_double_readonly::UPDATE_DELTA, (update_now - last_update));
				}

				const auto roughness = get_direct<double>(sprite::e_double::ROUGHNESS);


				// delayed work to do (auto-clean)
				if (auto f = get_direct<sprite::functional>(sprite::e_tie_functional::DELAYED_WORK_AUTODEL); f) {
					f();
					set<sprite::functional>(sprite::e_tie_functional::DELAYED_WORK_AUTODEL, std::function<void(void)>());
				}


				if (is_eq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, false)) {

					// task to be done (no auto-clean, internal, only if layer (as above))
					if (custom_think_task) custom_think_task();

					auto nowgo = easy_collision.process_result();
					double sum_dx = 0.0;
					double sum_dy = 0.0;

					if (nowgo & static_cast<int>(sprite::e_direction::SOUTH)) {
						if (get_direct<double>(sprite::e_double_readonly::SPEED_Y) < 0.0) {
							set(sprite::e_double_readonly::SPEED_Y, -roughness * get_direct<double>(sprite::e_double::ELASTICITY_Y) * get_direct<double>(sprite::e_double_readonly::SPEED_Y));
							//set(sprite::e_double::ACCELERATION_Y, sprite::minimum_sprite_accel_collision);
						}
						sum_dy = fabs(easy_collision.dy_max);
					}
					else if (nowgo & static_cast<int>(sprite::e_direction::NORTH)) {
						if (get_direct<double>(sprite::e_double_readonly::SPEED_Y) > 0.0) {
							set(sprite::e_double_readonly::SPEED_Y, -roughness * get_direct<double>(sprite::e_double::ELASTICITY_Y) * get_direct<double>(sprite::e_double_readonly::SPEED_Y));
							//set(sprite::e_double::ACCELERATION_Y, -sprite::minimum_sprite_accel_collision);
						}
						sum_dy = -fabs(easy_collision.dy_max);
					}

					if (nowgo & static_cast<int>(sprite::e_direction::EAST)) {
						if (get_direct<double>(sprite::e_double_readonly::SPEED_X) < 0.0) {
							set(sprite::e_double_readonly::SPEED_X, -roughness * get_direct<double>(sprite::e_double::ELASTICITY_X) * get_direct<double>(sprite::e_double_readonly::SPEED_X));
							//set(sprite::e_double::ACCELERATION_X, sprite::minimum_sprite_accel_collision);
						}
						sum_dx = fabs(easy_collision.dx_max);
					}
					else if (nowgo & static_cast<int>(sprite::e_direction::WEST)) {
						if (get_direct<double>(sprite::e_double_readonly::SPEED_X) > 0.0) {
							set(sprite::e_double_readonly::SPEED_X, -roughness * get_direct<double>(sprite::e_double::ELASTICITY_X) * get_direct<double>(sprite::e_double_readonly::SPEED_X));
							//set(sprite::e_double::ACCELERATION_X, -sprite::minimum_sprite_accel_collision);
						}
						sum_dx = -fabs(easy_collision.dx_max);
					}

					const auto targ_posx_c = get_direct<double>(sprite::e_double::TARG_POSX);
					const auto targ_posy_c = get_direct<double>(sprite::e_double::TARG_POSY);
					const auto speed_x_c = get_direct<double>(sprite::e_double_readonly::SPEED_X);
					const auto speed_y_c = get_direct<double>(sprite::e_double_readonly::SPEED_Y);


					set<double>(sprite::e_double::TARG_POSX, targ_posx_c + speed_x_c + sum_dx);
					set<double>(sprite::e_double_readonly::PROJECTED_POSX, targ_posx_c + speed_x_c + 2.0 * (sum_dx));
					set(sprite::e_double_readonly::SPEED_X, (get_direct<double>(sprite::e_double_readonly::SPEED_X) + get_direct<double>(sprite::e_double::ACCELERATION_X)) * roughness);

					set<double>(sprite::e_double::TARG_POSY, targ_posy_c + speed_y_c + sum_dy);
					set<double>(sprite::e_double_readonly::PROJECTED_POSY, targ_posy_c + speed_y_c + 2.0 * (sum_dy));
					set(sprite::e_double_readonly::SPEED_Y, (get_direct<double>(sprite::e_double_readonly::SPEED_Y) + get_direct<double>(sprite::e_double::ACCELERATION_Y)) * roughness);

					/*if (auto spr = (get_direct<double>(sprite::e_double::SPEED_ROTATION])(); spr != 0.0) {
						double act = (get_direct<double>(sprite::e_double::TARG_ROTATION])();
						get_direct<double>(sprite::e_double::TARG_ROTATION] = [spr,act] {return act + spr; };
					}*/
				}

				else { // read only
					set<double>(sprite::e_double_readonly::PROJECTED_POSX, get_direct<double>(sprite::e_double::TARG_POSX));
					set<double>(sprite::e_double_readonly::PROJECTED_POSY, get_direct<double>(sprite::e_double::TARG_POSY));
				}


				float m[2] = { 0.0f };
				bool is_mouse_pressed = false;
				bool mouse_collide = false;

				if (conf.has("mouse", "press_count")) {

					is_mouse_pressed = conf.get_as<unsigned>("mouse", "press_count") > 0;
					m[0] = conf.get_as<float>("mouse", "x");
					m[1] = conf.get_as<float>("mouse", "y");

					const double dist_x = get_direct<double>(sprite::e_double::TARG_POSX) - m[0];
					const double dist_y = get_direct<double>(sprite::e_double::TARG_POSY) - m[1];

					mouse_collide = (
						(fabs(dist_x) < 0.5 * get_direct<double>(sprite::e_double::SCALE_X) * get_direct<double>(sprite::e_double::SCALE_G)) &&
						(fabs(dist_y) < 0.5 * get_direct<double>(sprite::e_double::SCALE_X) * get_direct<double>(sprite::e_double::SCALE_G))
					);
				}

				{
					sprite::e_tie_functional last_state = get_direct<sprite::e_tie_functional>(sprite::e_tief_readonly::LAST_STATE);
					sprite::e_tie_functional new_state{};

					if (mouse_collide) {
						new_state = is_mouse_pressed ? sprite::e_tie_functional::COLLISION_MOUSE_CLICK : sprite::e_tie_functional::COLLISION_MOUSE_ON;
					}
					else if (last_state == sprite::e_tie_functional::COLLISION_MOUSE_CLICK) {
						new_state = sprite::e_tie_functional::COLLISION_MOUSE_UNCLICK;
					}
					else if (easy_collision.was_col) {
						new_state = sprite::e_tie_functional::COLLISION_COLLIDED_OTHER;
					}
					else {
						new_state = sprite::e_tie_functional::COLLISION_NONE;
					}

					if (new_state != last_state) {
						set<sprite::e_tie_functional>(sprite::e_tief_readonly::LAST_STATE, new_state);

						if (auto f = get_direct<sprite::functional>(new_state); f) f();
					}
				}

				double scale_x, scale_y;

				scale_x = get_direct<double>(sprite::e_double::SCALE_G) * get_direct<double>(sprite::e_double::SCALE_X);
				scale_y = get_direct<double>(sprite::e_double::SCALE_G) * get_direct<double>(sprite::e_double::SCALE_Y);

				easy_collision.setup(get_direct<double>(sprite::e_double::TARG_POSX), get_direct<double>(sprite::e_double::TARG_POSY), scale_x, scale_y);
			}

			/*
			Sprite_Base::easier_collision_handle& Sprite_Base::get_collision()
			{
				return easy_collision;
			}
			const std::vector<std::string> Sprite_Base::get_collided_with_list()
			{
				Tools::AutoLock luck(copy_final_m);
				return copy_final;
			}*/

		}
	}
}