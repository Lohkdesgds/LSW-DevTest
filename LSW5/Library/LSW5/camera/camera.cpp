#include "camera.h"

namespace LSW {
	namespace v5 {

		Camera::layer_each::layer_each(const int a, const double b, const double c) {
			layer = a;
			elasticity = b;
			roughness = c;
		}
		Camera::layer_each::layer_each(const layer_each & o) {
			layer = o.getLayerID();
			elasticity = o.getElasticity();
			roughness = o.getRoughness();
		}
		int Camera::layer_each::getLayerID() const {
			return layer;
		}
		bool Camera::layer_each::hasCollision() const {
			return elasticity < 0.0;
		}
		double Camera::layer_each::getElasticity() const {
			return elasticity;
		}
		double Camera::layer_each::getRoughness() const {
			return roughness;
		}
		void Camera::layer_each::setLayerID(const int a) {
			layer = a;
		}
		void Camera::layer_each::setElasticity(const double a) {
			if (a < 0.0) elasticity = -1.0;
			else elasticity = a; 
		}
		void Camera::layer_each::setRoughness(const double a) {
			if (a < 0.0) roughness = 0.0; 
			else if (a > 1.0) roughness = 1.0; 
			else elasticity = a; 
		}




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

		void Camera::addLayerConfig(const layer_each v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getLayerID() == v.getLayerID()) {
					//data.layers[p].setElasticity(v.getElasticity());
					data.layers[p] = v;
					return;
				}
			}
			data.layers.push_back(v);
		}

		void Camera::delLayerConfig(const int v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getLayerID() == v) {
					data.layers.erase(data.layers.begin() + p);
					return;
				}
			}
		}

		Camera::layer_each* Camera::getLayerConfig(const int v)
		{
			for (size_t p = 0; p < data.layers.size(); p++) {
				if (data.layers[p].getLayerID() == v) return &data.layers[p];
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

			// return is optional here
			transf(d,
				*Tools::assert_cast(data.double_data[camera::e_double::OFFSET_X]), *Tools::assert_cast(data.double_data[camera::e_double::OFFSET_Y]),
				*Tools::assert_cast(data.double_data[camera::e_double::SCALE_X]) * *Tools::assert_cast(data.double_data[camera::e_double::SCALE_G]),
				*Tools::assert_cast(data.double_data[camera::e_double::SCALE_Y]) * *Tools::assert_cast(data.double_data[camera::e_double::SCALE_G]),
				*Tools::assert_cast(data.double_data[camera::e_double::ROTATION_RAD]));
		}

		void Camera::apply()
		{
			al_use_transform(&data.transformation);
		}

		ALLEGRO_TRANSFORM Camera::getTransform()
		{
			return data.transformation;
		}

		void Camera::matrix_debug()
		{
			double m[2] = { 0.0 };
			double limits[4] = { 0.0 };
			//Database db;
			Camera* psf = this;
			Camera  org;

			//db.get(Constants::ro__db_mouse_double::RAW_MOUSE_X, m[0]);
			//db.get(Constants::ro__db_mouse_double::RAW_MOUSE_Y, m[1]);

			psf->get(camera::e_double::LIMIT_MIN_X, limits[0]);
			psf->get(camera::e_double::LIMIT_MIN_Y, limits[1]);
			psf->get(camera::e_double::LIMIT_MAX_X, limits[2]);
			psf->get(camera::e_double::LIMIT_MAX_Y, limits[3]);

			psf->refresh();

			float mt[2];
			for (short n = 0; n < 2; n++) mt[n] = +m[n];

			ALLEGRO_TRANSFORM untransf = psf->getTransform();
			al_invert_transform(&untransf);
			al_transform_coordinates(&untransf, &mt[0], &mt[1]);

			m[0] = +mt[0];
			m[1] = +mt[1];

			ALLEGRO_COLOR co = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
			ALLEGRO_COLOR cp = al_map_rgba_f(0.5, 0.25, 0.0, 0.5);
			ALLEGRO_COLOR cp2 = al_map_rgba_f(0.3, 0.08, 0.0, 0.3);

			org.apply();

			// +
			al_draw_line(-5, 0, 5, 0, co, 0.007);
			al_draw_line(0, -5, 0, 5, co, 0.007);

			// |---|
			al_draw_line(-1, 0.1, -1, -0.1, co, 0.007);
			al_draw_line(1, 0.1, 1, -0.1, co, 0.007);
			al_draw_line(0.1, -1, -0.1, -1, co, 0.007);
			al_draw_line(0.1, 1, -0.1, 1, co, 0.007);

			// mouse
			al_draw_filled_circle(m[0], m[1], 0.018, al_map_rgba_f(0.75, 0, 0, 0.75)); // mouse is different color

			psf->apply();

			// +
			al_draw_line(-5, 0, 5, 0, cp, 0.01);
			al_draw_line(0, -5, 0, 5, cp, 0.01);

			// x
			al_draw_line(-0.1, 0.1, 0.1, -0.1, cp, 0.008);
			al_draw_line(-0.1, -0.1, 0.1, 0.1, cp, 0.008);

			// |---|
			al_draw_line(-1, 0.1, -1, -0.1, cp, 0.005);
			al_draw_line(1, 0.1, 1, -0.1, cp, 0.005);
			al_draw_line(0.1, -1, -0.1, -1, cp, 0.005);
			al_draw_line(0.1, 1, -0.1, 1, cp, 0.005);

			// limits
			if (psf->isEq(camera::e_boolean::RESPECT_LIMITS, true)) al_draw_rectangle(limits[0], limits[1], limits[2], limits[3], cp2, 0.010);
		}

		Camera::Camera_configuration& Camera::copyRAW()
		{
			return data;
		}

}
}