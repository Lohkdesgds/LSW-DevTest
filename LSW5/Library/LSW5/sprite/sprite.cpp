#include "sprite.h"

namespace LSW {
	namespace v5 {
		/*Camera* Sprite_Base::checkAndGetCamera() const
		{
			Camera l_cam, *cam;
			if (!(cam = l_cam.getLastCameraApply())) {
				throw Abort::Abort(__FUNCSIG__, "Camera not set! Please setup the camera before drawing.", Abort::abort_level::GIVEUP);
			}
			return cam;
		}*/




		Sprite_Base::easier_collision_handle::easier_collision_handle(const easier_collision_handle& cpy)
		{
			posx = cpy.posx;
			posy = cpy.posy;
			sizx = cpy.sizx;
			sizy = cpy.sizy;
			memcpy_s(&directions_cases, sizeof(directions_cases), &cpy.directions_cases, sizeof(cpy.directions_cases));
			was_col = cpy.was_col;
		}

		bool Sprite_Base::easier_collision_handle::overlap(easier_collision_handle& ol) // me == P1
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
					if (dist_x > 0.0) {
						directions_cases[static_cast<int>(sprite::e_direction::WEST)]++; // VEM DO WEST
					}
					else {
						directions_cases[static_cast<int>(sprite::e_direction::EAST)]++; // VEM DO EAST
					}
				}
				else {
					if (dist_y > 0.0) {
						directions_cases[static_cast<int>(sprite::e_direction::NORTH)]++; // VEM DO NORTH
					}
					else {
						directions_cases[static_cast<int>(sprite::e_direction::SOUTH)]++; // VEM DO SOUTH
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

		sprite::e_direction Sprite_Base::easier_collision_handle::processResult()
		{
			bool n, s, w, e;
			n = directions_cases[static_cast<int>(sprite::e_direction::NORTH)] > 0;
			s = directions_cases[static_cast<int>(sprite::e_direction::SOUTH)] > 0;
			w = directions_cases[static_cast<int>(sprite::e_direction::WEST)] > 0;
			e = directions_cases[static_cast<int>(sprite::e_direction::EAST)] > 0;

			if (n) { // north
				if (w && !e) {
					return sprite::e_direction::EAST; // GOTO EAST
				}
				if (e && !w) {
					return sprite::e_direction::WEST; // GOTO WEST
				}
				if (s) {
					if (directions_cases[static_cast<int>(sprite::e_direction::NORTH)] > directions_cases[static_cast<int>(sprite::e_direction::SOUTH)]) return sprite::e_direction::SOUTH;
					if (directions_cases[static_cast<int>(sprite::e_direction::SOUTH)] > directions_cases[static_cast<int>(sprite::e_direction::NORTH)]) return sprite::e_direction::NORTH;
					return sprite::e_direction::NONE; // NO GOTO
				}
				return sprite::e_direction::SOUTH; // GOTO SOUTH
			}
			else if (s) { //south
				if (w && !e) {
					return sprite::e_direction::EAST; // GOTO EAST
				}
				if (e && !w) {
					return sprite::e_direction::WEST; // GOTO WEST
				}
				return sprite::e_direction::NORTH; // GOTO NORTH
			}
			else if (w) { // west
				if (e) {
					if (directions_cases[static_cast<int>(sprite::e_direction::EAST)] > directions_cases[static_cast<int>(sprite::e_direction::WEST)]) return sprite::e_direction::WEST;
					if (directions_cases[static_cast<int>(sprite::e_direction::WEST)] > directions_cases[static_cast<int>(sprite::e_direction::EAST)]) return sprite::e_direction::EAST;
					return sprite::e_direction::NONE;
				}
				return sprite::e_direction::EAST;
			}
			else if (e) { // east
				return sprite::e_direction::WEST;
			}
			return sprite::e_direction::NONE;
		}

		void Sprite_Base::easier_collision_handle::resetDirections()
		{
			for (auto& i : directions_cases) i = 0;
		}

		void Sprite_Base::easier_collision_handle::setup(const double px, const double py, const double sx, const double sy)
		{
			posx = px;
			posy = py;
			sizx = Shared::game_collision_oversize + sx;
			sizy = Shared::game_collision_oversize + sy;
			resetDirections();
			was_col = false;
		}


		Sprite_Base::Sprite_Base()
		{
			/*if (!data_sprite_base) throw Abort::Abort(__FUNCSIG__, "Failed to create Sprite base's data!");
			data_sprite_base->original_this = (void*)this;*/

			set<double>(sprite::e_double_defaults);
			set<bool>(sprite::e_boolean_defaults);
			set<std::string>(sprite::e_string_defaults);
			set<int>(sprite::e_integer_defaults);
			set<ALLEGRO_COLOR>(sprite::e_color_defaults);
			set(sprite::e_uintptrt::DATA_FROM, (uintptr_t)this);
		}

		Sprite_Base::Sprite_Base(Sprite_Base& other)
		{
			//if (!data_sprite_base) throw Abort::Abort(__FUNCSIG__, "Failed to create Sprite base's data!");
			auto oth = other.getAttributes(); // yes, but it will copy, not "replace"

			set<double>(*oth->double_data);
			set<bool>(*oth->boolean_data);
			set<std::string>(*oth->string_data);
			set<int>(*oth->integer_data);
			set<ALLEGRO_COLOR>(*oth->color_data);
			set<uintptr_t>(*oth->uintptrt_data);

			/*data_sprite_base->double_readonly_data		= oth->double_readonly_data;
			data_sprite_base->boolean_readonly_data		= oth->boolean_readonly_data;
			data_sprite_base->string_data				= oth->string_data;
			data_sprite_base->double_data				= oth->double_data;
			data_sprite_base->boolean_data				= oth->boolean_data;
			data_sprite_base->integer_data				= oth->integer_data;
			data_sprite_base->color_data				= oth->color_data;*/
		}

		void Sprite_Base::hook(const sprite::e_tie_functional e, const std::function<void(void)> f)
		{
			pair_tied[static_cast<size_t>(e)] = f;
		}

		void Sprite_Base::unhook(const sprite::e_tie_functional e)
		{
			hook(e, std::function<void(void)>());
		}

 		void Sprite_Base::twinUpAttributes(const std::shared_ptr<sprite_base_data> oth)
		{
			//data_sprite_base = oth; // now they share

			set<double>(oth->double_data);
			set<bool>(oth->boolean_data);
			set<std::string>(oth->string_data);
			set<int>(oth->integer_data);
			set<ALLEGRO_COLOR>(oth->color_data);
			set<uintptr_t>(oth->uintptrt_data);
		}

		std::shared_ptr<Sprite_Base::sprite_base_data> Sprite_Base::getAttributes()
		{
			std::shared_ptr<Sprite_Base::sprite_base_data> oth = std::make_shared<Sprite_Base::sprite_base_data>();

			oth->double_data = get<double>();
			oth->boolean_data = get<bool>();
			oth->string_data = get<std::string>();
			oth->integer_data = get<int>();
			oth->color_data = get<ALLEGRO_COLOR>();
			oth->uintptrt_data = get<uintptr_t>();
			return oth;
		}

		bool Sprite_Base::draw(Camera* cam, int layer) {
			if (layer == getDirect<int>(sprite::e_integer::LAYER)) return draw(cam, false);
			return false;
		}

		bool Sprite_Base::draw(Camera* cam, bool checkcam)
		{
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's draw!", Abort::abort_level::GIVEUP);

			if (checkcam) {
				if (auto* ptr = cam->getLayerConfig(getDirect<int>(sprite::e_integer::LAYER)); !ptr) return false; // check layer
			}

			if (getDirect<uintptr_t>(sprite::e_uintptrt::DATA_FROM) == (uintptr_t)this) { // only original copy should update parameters
				//if (isEq(sprite::e_boolean::SHOWBOX, true) || isEq(sprite::e_boolean::SHOWDOT, true)) {
					Camera clean_camera;

					const auto affected_by_cam = getDirect<bool>(sprite::e_boolean::AFFECTED_BY_CAM);
					const auto targ_rotation = getDirect<double>(sprite::e_double::TARG_ROTATION);
					const auto targ_posx = getDirect<double>(sprite::e_double::TARG_POSX);
					const auto targ_posy = getDirect<double>(sprite::e_double::TARG_POSY);
					const auto scale_g = getDirect<double>(sprite::e_double::SCALE_G);
					const auto scale_x = getDirect<double>(sprite::e_double::SCALE_X);
					const auto scale_y = getDirect<double>(sprite::e_double::SCALE_Y);
					auto& last_draw = *getDirect<double*>(sprite::e_double_readonly::LAST_DRAW);
					auto& rotation = *getDirect<double*>(sprite::e_double_readonly::ROTATION);
					auto& posx = *getDirect<double*>(sprite::e_double_readonly::POSX);
					auto& posy = *getDirect<double*>(sprite::e_double_readonly::POSY);
					const auto last_draw_v = last_draw();
					const auto rotation_v = rotation();
					const auto posx_v = posx();
					const auto posy_v = posy();


					if (affected_by_cam) {
						cam->apply();
					}
					else {
						clean_camera.apply();
					}

					// delta T calculation

					double timee = al_get_time();
					double dt = timee - last_draw();
					last_draw = [=] {return timee; };

					double perc_run = Shared::game_timing_tps * dt;		// ex: 5 per sec * 0.2 (1/5 sec) = 1, so posx = actual posx...
					if (perc_run > 1.0) perc_run = 1.0;					// 1.0 is "set value"
					if (perc_run < 1.0 / 10000) perc_run = 1.0 / 10000; // can't be infinitely smooth right? come on

					// new position calculation

					rotation = [=] {return (1.0 - perc_run) * rotation_v + perc_run * targ_rotation; };
					posx = [=] {return (1.0 - perc_run) * posx_v + perc_run * targ_posx; };
					posy = [=] {return (1.0 - perc_run) * posy_v + perc_run * targ_posy; };

					if (isEq(sprite::e_boolean::SHOWBOX, true)) {

						const double calculated_scale_x = scale_g * scale_x;
						const double calculated_scale_y = scale_g * scale_y;

						al_draw_filled_rectangle(
							/* X1: */ targ_posx - calculated_scale_x * 0.5,
							/* Y1: */ targ_posy - calculated_scale_y * 0.5,
							/* X2: */ targ_posx + calculated_scale_x * 0.5,
							/* Y2: */ targ_posy + calculated_scale_y * 0.5,
							al_map_rgba(180, easy_collision.was_col ? 90 : 180, easy_collision.was_col ? 90 : 180, 180));
					}

					if (isEq(sprite::e_boolean::SHOWDOT, true)) {
						al_draw_filled_circle(
							/* X1: */ posx(),
							/* X1: */ posy(),
							/* SCL */ 0.1f * (fabs(0.30f) + 0.12f),
							al_map_rgba(90, easy_collision.was_col ? 45 : 90, easy_collision.was_col ? 45 : 90, 90));
					}
				//}
			}
			// custom_draw_task has nothing to do with clone attributes, so this is fine! (I guess so)
			if (custom_draw_task) custom_draw_task();

			return true;
		}

		void Sprite_Base::collide(Camera* cam, Sprite_Base& oth) // if colliding, do not accept speed entry (keyboard player moving)
		{
			if (getDirect<uintptr_t>(sprite::e_uintptrt::DATA_FROM) == (uintptr_t)this) return; // this is a copy following attrbutes from somewhere else, no duplication in collision
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's collision!", Abort::abort_level::GIVEUP);

			if (!oth.isEq(sprite::e_integer::LAYER, getDirect<int>(sprite::e_integer::LAYER))) return; // not same layer
			if (oth.isEq(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE))) return; // no collision
			if (isEq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true)) return; // no collision (readonly)
			if (oth.isEq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true)) return; // no collision (readonly)

			if (auto* ptr = cam->getLayerConfig(getDirect<int>(sprite::e_integer::LAYER)); ptr) {
				if (!ptr->hasCollision()) return;

				double elasticity = ptr->getElasticity();

				easy_collision.overlap(oth.getCollision());

				/*if (easy_collision.overlap(cpy)) {

				}*/
			}
		}

		void Sprite_Base::update(Camera* cam)// needs to use ACCELERATION_X when collision, ELASTICITY_X to know how much of it goes backwards, TARG_POSX
		{
			// delayed work to do (auto-clean)
			if (auto& f = pair_tied[static_cast<size_t>(sprite::e_tie_functional::DELAYED_WORK)]; f) {
				f();
				f = std::function<void(void)>();
			}

			if (getDirect<uintptr_t>(sprite::e_uintptrt::DATA_FROM) == (uintptr_t)this) return; // this is a copy following attrbutes from somewhere else, no duplication in update
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's update!", Abort::abort_level::GIVEUP);


			if (auto* ptr = cam->getLayerConfig(getDirect<int>(sprite::e_integer::LAYER)); ptr && isEq(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, false)) {

				// task to be done (no auto-clean, internal, only if layer (as above))
				if (custom_think_task) custom_think_task();

				double roughness = ptr->getRoughness();

				auto nogo = easy_collision.processResult();

				switch (nogo) {
				case sprite::e_direction::SOUTH:
					if (getDirect<double>(sprite::e_double_readonly::SPEED_Y) < 0.0) {
						set(sprite::e_double_readonly::SPEED_Y, -roughness * getDirect<double>(sprite::e_double::ELASTICITY_Y) * getDirect<double>(sprite::e_double_readonly::SPEED_Y));
						//getDirect<double>(sprite::e_double::ACCELERATION_Y] = sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::NORTH:
					if (getDirect<double>(sprite::e_double_readonly::SPEED_Y) > 0.0) {
						set(sprite::e_double_readonly::SPEED_Y, -roughness * getDirect<double>(sprite::e_double::ELASTICITY_Y) * getDirect<double>(sprite::e_double_readonly::SPEED_Y));
						//getDirect<double>(sprite::e_double::ACCELERATION_Y] = -sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::EAST:
					if (getDirect<double>(sprite::e_double_readonly::SPEED_X) < 0.0) {
						set(sprite::e_double_readonly::SPEED_X, -roughness * getDirect<double>(sprite::e_double::ELASTICITY_X) * getDirect<double>(sprite::e_double_readonly::SPEED_X));
						//getDirect<double>(sprite::e_double::ACCELERATION_X] = sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::WEST:
					if (getDirect<double>(sprite::e_double_readonly::SPEED_X) > 0.0) {
						set(sprite::e_double_readonly::SPEED_X, -roughness * getDirect<double>(sprite::e_double::ELASTICITY_X) * getDirect<double>(sprite::e_double_readonly::SPEED_X));
						//getDirect<double>(sprite::e_double::ACCELERATION_X] = -sprite::minimum_sprite_accel_collision;
					}
					break;
				}
				
				const auto targ_posx_c = getDirect<double>(sprite::e_double::TARG_POSX);
				const auto targ_posy_c = getDirect<double>(sprite::e_double::TARG_POSY);
				const auto speed_x_c   = getDirect<double>(sprite::e_double_readonly::SPEED_X);
				const auto speed_y_c   = getDirect<double>(sprite::e_double_readonly::SPEED_Y);
			

				set<double>(sprite::e_double::TARG_POSX, [targ_posx_c,speed_x_c] { return targ_posx_c + speed_x_c; });
				set(sprite::e_double_readonly::SPEED_X, (getDirect<double>(sprite::e_double_readonly::SPEED_X) + getDirect<double>(sprite::e_double::ACCELERATION_X)) * roughness);

				set<double>(sprite::e_double::TARG_POSY, [targ_posy_c, speed_y_c] { return targ_posy_c + speed_y_c; });
				set(sprite::e_double_readonly::SPEED_Y, (getDirect<double>(sprite::e_double_readonly::SPEED_Y) + getDirect<double>(sprite::e_double::ACCELERATION_Y)) * roughness);

				/*if (auto spr = (getDirect<double>(sprite::e_double::SPEED_ROTATION])(); spr != 0.0) {
					double act = (getDirect<double>(sprite::e_double::TARG_ROTATION])();
					getDirect<double>(sprite::e_double::TARG_ROTATION] = [spr,act] {return act + spr; };
				}*/
			}


			if (!getDirect<bool>(sprite::e_boolean::FOLLOW_MOUSE)) {
				double m[2] = { 0.0 };
				bool is_mouse_pressed = false;

				Database db;

				db.get(database::e_double::MOUSE_X, m[0]);
				db.get(database::e_double::MOUSE_Y, m[1]);

				for (size_t u = static_cast<size_t>(database::e_boolean::MOUSE_0); !is_mouse_pressed && u <= static_cast<size_t>(database::e_boolean::MOUSE_7); u++)
					is_mouse_pressed |= db.isEq(static_cast<database::e_boolean>(u), true);

				const double dist_x = getDirect<double>(sprite::e_double::TARG_POSX) - m[0];
				const double dist_y = getDirect<double>(sprite::e_double::TARG_POSY) - m[1];

				bool mouse_collide = (
					(fabs(dist_x) < 0.5 * getDirect<double>(sprite::e_double::SCALE_X) * getDirect<double>(sprite::e_double::SCALE_G)) &&
					(fabs(dist_y) < 0.5 * getDirect<double>(sprite::e_double::SCALE_X) * getDirect<double>(sprite::e_double::SCALE_G))
					);


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
			}			

			if (new_state != last_state) {
				last_state = new_state;

				if (pair_tied[static_cast<size_t>(last_state)]) pair_tied[static_cast<size_t>(last_state)]();
			}

			double scale_x, scale_y;

			scale_x = getDirect<double>(sprite::e_double::SCALE_G) * getDirect<double>(sprite::e_double::SCALE_X);
			scale_y = getDirect<double>(sprite::e_double::SCALE_G) * getDirect<double>(sprite::e_double::SCALE_Y);

			easy_collision.setup(getDirect<double>(sprite::e_double::TARG_POSX), getDirect<double>(sprite::e_double::TARG_POSY), scale_x, scale_y);
		}


		Sprite_Base::easier_collision_handle& Sprite_Base::getCollision()
		{
			return easy_collision;
		}
	}
}