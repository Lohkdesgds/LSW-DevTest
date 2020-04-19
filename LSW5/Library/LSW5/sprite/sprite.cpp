#include "sprite.h"

namespace LSW {
	namespace v5 {
		Camera* Sprite_Base::checkAndGetCamera() const
		{
			Camera l_cam, *cam;
			if (!(cam = l_cam.getLastCameraApply())) {
				throw Abort::Abort(__FUNCSIG__, "Camera not set! Please setup the camera before drawing.", Abort::abort_level::GIVEUP);
			}
			return cam;
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

		void Sprite_Base::draw()
		{
			if (isEq(sprite::e_boolean::SHOWBOX, true) || isEq(sprite::e_boolean::SHOWDOT, true)) {
				Camera* cam = checkAndGetCamera();
				Camera clean_camera;

				double camx, camy, camg;
				if (*data.boolean_data[sprite::e_boolean::AFFECTED_BY_CAM]) {
					cam->get(camera::e_double::SCALE_G, camg);
					cam->get(camera::e_double::SCALE_X, camx);
					cam->get(camera::e_double::SCALE_Y, camy);
				}
				else {
					clean_camera.apply();
					camx = camy = camg = 1.0;
				}

				if (isEq(sprite::e_boolean::SHOWBOX, true)) {
					double scale_x, scale_y;

					scale_x = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_X] * (camg * camx);
					scale_y = *data.double_data[sprite::e_double::SCALE_G] * *data.double_data[sprite::e_double::SCALE_Y] * (camg * camy);

					al_draw_filled_rectangle(
						/* X1: */ *data.double_data[sprite::e_double::TARG_POSX] - scale_x,
						/* Y1: */ *data.double_data[sprite::e_double::TARG_POSY] - scale_y,
						/* X2: */ *data.double_data[sprite::e_double::TARG_POSX] + scale_x,
						/* Y2: */ *data.double_data[sprite::e_double::TARG_POSY] + scale_y,
						al_map_rgb(255, 255, 255));
				}

				if (isEq(sprite::e_boolean::SHOWDOT, true)) {
					al_draw_filled_circle(
						/* X1: */ *data.double_data[sprite::e_double::TARG_POSX],
						/* X1: */ *data.double_data[sprite::e_double::TARG_POSY],
						/* SCL */ 0.1f * (fabs(camg * sqrt(camx * camy) * 0.30f) + 0.12f),
						al_map_rgb(127,127,127));
				}
			}

			if (custom_draw_task) custom_draw_task();
		}

		void Sprite_Base::collide(Sprite_Base& oth) // if colliding, do not accept speed entry (keyboard player moving)
		{
			Camera* cam = checkAndGetCamera();

			if (auto* ptr = cam->getLayer(*getRef(sprite::e_integer::LAYER)); ptr) {
				if (!ptr->hasCollision()) return;

				double elasticity = ptr->getElasticity();


			}
		}

		Sprite_Base::sprite_base_data& Sprite_Base::copyRAW()
		{
			return data;
		}

	}
}