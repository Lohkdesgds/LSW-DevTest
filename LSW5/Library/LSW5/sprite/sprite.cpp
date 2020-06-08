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
			sizx = sx;
			sizy = sy;
			resetDirections();
			was_col = false;
		}


		Sprite_Base::Sprite_Base(Sprite_Base& other)
		{
			auto& oth = other.copyRAW();
			data.double_readonly_data	= oth.double_readonly_data;
			data.boolean_readonly_data	= oth.boolean_readonly_data;
			data.string_data			= oth.string_data;
			data.double_data			= oth.double_data;
			data.boolean_data			= oth.boolean_data;
			data.integer_data			= oth.integer_data;
			data.color_data				= oth.color_data;
		}
		void Sprite_Base::set(const sprite::e_string e, std::string v)
		{
			if (auto* ptr = data.string_data(e); ptr)
				*ptr = v;
		}

		void Sprite_Base::set(const sprite::e_double e, double v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Sprite_Base::set(const sprite::e_boolean e, bool v)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				*ptr = v;
		}

		void Sprite_Base::set(const sprite::e_integer e, int v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}

		void Sprite_Base::set(const sprite::e_color e, ALLEGRO_COLOR v)
		{
			if (auto* ptr = data.color_data(e); ptr)
				*ptr = v;
		}

		void Sprite_Base::set(const std::string e, std::string v)
		{
			auto* ptr = data.string_data(e.c_str(), e.length());
			if (!ptr) data.string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Sprite_Base::set(const std::string e, double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Sprite_Base::set(const std::string e, bool v)
		{
			auto* ptr = data.boolean_data(e.c_str(), e.length());
			if (!ptr) data.boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Sprite_Base::set(const std::string e, int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Sprite_Base::set(const std::string e, ALLEGRO_COLOR v)
		{
			auto* ptr = data.color_data(e.c_str(), e.length());
			if (!ptr) data.color_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Sprite_Base::get(const sprite::e_double_readonly e, double& v)
		{
			if (auto* ptr = data.double_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_boolean_readonly e, bool& v)
		{
			if (auto* ptr = data.boolean_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_string e, std::string& v)
		{
			if (auto* ptr = data.string_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_boolean e, bool& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const sprite::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const std::string e, bool& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const std::string e, std::string& v)
		{
			if (auto* ptr = data.string_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Sprite_Base::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		double* Sprite_Base::getRef(const sprite::e_double_readonly e)
		{
			if (auto* ptr = data.double_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		bool* Sprite_Base::getRef(const sprite::e_boolean_readonly e)
		{
			if (auto* ptr = data.boolean_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::string* Sprite_Base::getRef(const sprite::e_string e)
		{
			if (auto* ptr = data.string_data(e); ptr)
				return ptr;
			return nullptr;
		}

		double* Sprite_Base::getRef(const sprite::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		bool* Sprite_Base::getRef(const sprite::e_boolean e)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		int* Sprite_Base::getRef(const sprite::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		ALLEGRO_COLOR* Sprite_Base::getRef(const sprite::e_color e)
		{
			if (auto* ptr = data.color_data(e); ptr)
				return ptr;
			return nullptr;
		}

		void Sprite_Base::draw(Camera* cam)
		{
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's draw!", Abort::abort_level::GIVEUP);

			if (isEq(sprite::e_boolean::SHOWBOX, true) || isEq(sprite::e_boolean::SHOWDOT, true)) {
				Camera clean_camera;

				double camx, camy, camg;
				if (*data.boolean_data[sprite::e_boolean::AFFECTED_BY_CAM]) {
					cam->get(camera::e_double::SCALE_G, camg);
					cam->get(camera::e_double::SCALE_X, camx);
					cam->get(camera::e_double::SCALE_Y, camy);
					cam->apply();
				}
				else {
					clean_camera.apply();
					camx = camy = camg = 1.0;
				}

				// delta T calculation

				double timee = al_get_time();
				double dt = timee - *data.double_readonly_data[sprite::e_double_readonly::LAST_DRAW];
				*data.double_readonly_data[sprite::e_double_readonly::LAST_DRAW] = timee;

				double perc_run = Shared::game_timing_tps * dt; // ex: 5 per sec * 0.2 (1/5 sec) = 1, so posx = actual posx...
				if (perc_run > 1.0) perc_run = 1.0; // 1.0 is "set value"
				if (perc_run < 1.0 / 10000) perc_run = 1.0 / 10000; // can't be infinitely smooth right? come on

				// new position calculation

				*data.double_readonly_data[sprite::e_double_readonly::ROTATION] = (1.0 - perc_run) * *data.double_readonly_data[sprite::e_double_readonly::ROTATION] + perc_run * *data.double_data[sprite::e_double::TARG_ROTATION];
				*data.double_readonly_data[sprite::e_double_readonly::POSX] = (1.0 - perc_run) * *data.double_readonly_data[sprite::e_double_readonly::POSX] + perc_run * *data.double_data[sprite::e_double::TARG_POSX];
				*data.double_readonly_data[sprite::e_double_readonly::POSY] = (1.0 - perc_run) * *data.double_readonly_data[sprite::e_double_readonly::POSY] + perc_run * *data.double_data[sprite::e_double::TARG_POSY];



				if (isEq(sprite::e_boolean::SHOWBOX, true)) {
					double scale_x, scale_y;

					scale_x = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_X] * (camg * camx);
					scale_y = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_Y] * (camg * camy);

					al_draw_filled_rectangle(
						/* X1: */ *data.double_data[sprite::e_double::TARG_POSX] - scale_x * 0.5,
						/* Y1: */ *data.double_data[sprite::e_double::TARG_POSY] - scale_y * 0.5,
						/* X2: */ *data.double_data[sprite::e_double::TARG_POSX] + scale_x * 0.5,
						/* Y2: */ *data.double_data[sprite::e_double::TARG_POSY] + scale_y * 0.5,
						al_map_rgb(255, easy_collision.was_col ? 128 : 255, easy_collision.was_col ? 128 : 255));

					//al_draw_filled_rectangle(0, 0, 900, 900, al_map_rgb(255, 0, 255));
				}

				if (isEq(sprite::e_boolean::SHOWDOT, true)) {
					al_draw_filled_circle(
						/* X1: */ *data.double_readonly_data[sprite::e_double_readonly::POSX],
						/* X1: */ *data.double_readonly_data[sprite::e_double_readonly::POSY],
						/* SCL */ 0.1f * (fabs(camg * sqrt(camx * camy) * 0.30f) + 0.12f),
						al_map_rgb(127, easy_collision.was_col ? 76 : 127, easy_collision.was_col ? 76 : 127));
				}
			}

			if (custom_draw_task) custom_draw_task();
		}

		void Sprite_Base::collide(Camera* cam, Sprite_Base& oth) // if colliding, do not accept speed entry (keyboard player moving)
		{
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's collision!", Abort::abort_level::GIVEUP);

			if (!oth.isEq(sprite::e_integer::LAYER, *getRef(sprite::e_integer::LAYER))) return; // not same layer
			if (oth.isEq(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE))) return; // no collision

			if (auto* ptr = cam->getLayerConfig(*getRef(sprite::e_integer::LAYER)); ptr) {
				if (!ptr->hasCollision()) return;

				double elasticity = ptr->getElasticity();

				easy_collision.overlap(oth.getCollision());

				/*if (easy_collision.overlap(cpy)) {

				}*/
			}
		}

		void Sprite_Base::update(Camera* cam)// needs to use ACCELERATION_X when collision, ELASTICITY_X to know how much of it goes backwards, TARG_POSX
		{
			if (!cam) throw Abort::Abort(__FUNCSIG__, "Invalid camera at Sprite's update!", Abort::abort_level::GIVEUP);

			if (auto* ptr = cam->getLayerConfig(*getRef(sprite::e_integer::LAYER)); ptr) {
				double roughness = ptr->getRoughness();

				auto nogo = easy_collision.processResult();

				switch (nogo) {
				case sprite::e_direction::SOUTH:
					if (*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] < 0.0) {
						*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] = (-roughness * *data.double_data[sprite::e_double::ELASTICITY_Y] * (*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y]));
						//*data.double_data[sprite::e_double::ACCELERATION_Y] = sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::NORTH:
					if (*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] > 0.0) {
						*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] = (-roughness * *data.double_data[sprite::e_double::ELASTICITY_Y] * (*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y]));
						//*data.double_data[sprite::e_double::ACCELERATION_Y] = -sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::EAST:
					if (*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] < 0.0) {
						*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] = (-roughness * *data.double_data[sprite::e_double::ELASTICITY_X] * (*data.double_readonly_data[sprite::e_double_readonly::SPEED_X]));
						//*data.double_data[sprite::e_double::ACCELERATION_X] = sprite::minimum_sprite_accel_collision;
					}
					break;
				case sprite::e_direction::WEST:
					if (*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] > 0.0) {
						*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] = (-roughness * *data.double_data[sprite::e_double::ELASTICITY_X] * (*data.double_readonly_data[sprite::e_double_readonly::SPEED_X]));
						//*data.double_data[sprite::e_double::ACCELERATION_X] = -sprite::minimum_sprite_accel_collision;
					}
					break;
				}

				*data.double_data[sprite::e_double::TARG_POSX] += *data.double_readonly_data[sprite::e_double_readonly::SPEED_X];
				*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] += *data.double_data[sprite::e_double::ACCELERATION_X];
				*data.double_readonly_data[sprite::e_double_readonly::SPEED_X] *= roughness;

				*data.double_data[sprite::e_double::TARG_POSY] += *data.double_readonly_data[sprite::e_double_readonly::SPEED_Y];
				*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] += *data.double_data[sprite::e_double::ACCELERATION_Y];
				*data.double_readonly_data[sprite::e_double_readonly::SPEED_Y] *= roughness;


				double camx, camy, camg;
				if (*data.boolean_data[sprite::e_boolean::AFFECTED_BY_CAM]) {
					cam->get(camera::e_double::SCALE_G, camg);
					cam->get(camera::e_double::SCALE_X, camx);
					cam->get(camera::e_double::SCALE_Y, camy);
					cam->apply();
				}
				else {
					camx = camy = camg = 1.0;
				}

				double scale_x, scale_y;

				scale_x = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_X] * (camg * camx);
				scale_y = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_Y] * (camg * camy);

				easy_collision.setup(*data.double_data[sprite::e_double::TARG_POSX], *data.double_data[sprite::e_double::TARG_POSY], scale_x, scale_y);
			}
		}

		Sprite_Base::sprite_base_data& Sprite_Base::copyRAW()
		{
			return data;
		}

		Sprite_Base::easier_collision_handle& Sprite_Base::getCollision()
		{
			return easy_collision;
		}
	}
}