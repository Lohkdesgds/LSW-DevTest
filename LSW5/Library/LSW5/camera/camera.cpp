#include "camera.h"

namespace LSW {
	namespace v5 {

		Camera* Camera::last_camera_applied = nullptr;

		ALLEGRO_TRANSFORM Camera::transf(ALLEGRO_BITMAP* d, const float x, const float y, const float sx, const float sy, const float r)
		{
			ALLEGRO_TRANSFORM& t = data.transformation;

			al_identity_transform(&t);

			al_rotate_transform(&t, r);

			al_scale_transform(&t,
				al_get_bitmap_width(d) * 0.5 * sx,
				al_get_bitmap_height(d) * 0.5 * sy);

			al_translate_transform(&t,
				al_get_bitmap_width(d) * 0.5 - x * (al_get_bitmap_width(d) * 0.5 * sx),
				al_get_bitmap_height(d) * 0.5 - y * (al_get_bitmap_height(d) * 0.5 * sy));

			return t;
		}
		Camera::Camera()
		{
			lsw_al_init();

			reset(); // refresh(); auto
		}

		Camera::Camera(Camera& other)
		{
			auto& j = other.copyRAW();

			data.double_data = j.double_data;
			data.boolean_data = j.boolean_data;
			data.integer_data = j.integer_data;
			data.layers = j.layers;
			data.transformation = j.transformation;
		}

		void Camera::reset()
		{
			data.double_data  = camera::e_double_defaults;
			data.boolean_data = camera::e_boolean_defaults;
			data.integer_data = camera::e_integer_defaults;
			data.layers.clear();

			al_identity_transform(&data.transformation);
			refresh();
		}

		void Camera::setLayer(const Camera::camera_data::layer_config v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getID() == v.getID()) {
					data.layers[p].setElasticity(v.getElasticity());
					return;
				}
			}
			data.layers.push_back(v);
		}

		void Camera::delLayer(const int v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getID() == v) {
					data.layers.erase(data.layers.begin() + p);
					return;
				}
			}
		}

		Camera::camera_data::layer_config* Camera::getLayer(const int v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getID() == v) return &data.layers[p];
			}
			return nullptr;
		}

		void Camera::set(const camera::e_double e, double v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Camera::set(const camera::e_boolean e, bool v)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				*ptr = v;
		}

		void Camera::set(const camera::e_integer e, int v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}

		void Camera::set(const std::string e, double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Camera::set(const std::string e, bool v)
		{
			auto* ptr = data.boolean_data(e.c_str(), e.length());
			if (!ptr) data.boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Camera::set(const std::string e, int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Camera::get(const camera::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Camera::get(const camera::e_boolean e, bool& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr) { 
				v = *ptr; 
				return true;
			}
			return false;
		}

		bool Camera::get(const camera::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr) { 
				v = *ptr; 
				return true;
			}
			return false;
		}

		bool Camera::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) { 
				v = *ptr;
				return true; 
			}
			return false;
		}

		bool Camera::get(const std::string e, bool& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Camera::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		double* Camera::getRef(const camera::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		bool* Camera::getRef(const camera::e_boolean e)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		int* Camera::getRef(const camera::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		void Camera::refresh()
		{
			ALLEGRO_BITMAP* d = al_get_target_bitmap();

			if (!d) return;

			transf(d,
				*Tools::assert_cast(data.double_data[camera::e_double::OFFSET_X]), *Tools::assert_cast(data.double_data[camera::e_double::OFFSET_Y]),
				*Tools::assert_cast(data.double_data[camera::e_double::SCALE_X]) * *Tools::assert_cast(data.double_data[camera::e_double::SCALE_G]),
				*Tools::assert_cast(data.double_data[camera::e_double::SCALE_Y]) * *Tools::assert_cast(data.double_data[camera::e_double::SCALE_G]),
				*Tools::assert_cast(data.double_data[camera::e_double::ROTATION_RAD]));
		}

		void Camera::apply()
		{
			al_use_transform(&data.transformation);
			last_camera_applied = this;
		}

		Camera* Camera::getLastCameraApply() const
		{
			return last_camera_applied;
		}

		Camera::camera_data& Camera::copyRAW()
		{
			return data;
		}

	}
}