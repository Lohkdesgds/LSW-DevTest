#pragma once

#include <allegro5/allegro.h>

#include <map>
#include <vector>

#include "..\supermap\supermap.h"
#include "..\tools\tools.h"
#include "..\allegroinitialization\allegroinitialization.h"

namespace LSW {
	namespace v5 {
		namespace camera {
			enum class e_double { SCALE_X, SCALE_Y, SCALE_G, OFFSET_X, OFFSET_Y, ROTATION_RAD, LIMIT_MIN_X, LIMIT_MIN_Y, LIMIT_MAX_X, LIMIT_MAX_Y };
			enum class e_boolean { RESPECT_LIMITS, READONLY_NOW };
			enum class e_integer { ID };


			const SuperMap<double> e_double_defaults = {
				{1.0,			(e_double::SCALE_X),						CHAR_INIT("scale_x")},
				{1.0,			(e_double::SCALE_Y),						CHAR_INIT("scale_y")},
				{1.0,			(e_double::SCALE_G),						CHAR_INIT("scale_g")},
				{0.0,			(e_double::OFFSET_X), 						CHAR_INIT("offset_x")},
				{0.0,			(e_double::OFFSET_Y), 						CHAR_INIT("offset_y")},
				{0.0,			(e_double::ROTATION_RAD),					CHAR_INIT("rotation_rad")},
				{-1.0e308,		(e_double::LIMIT_MIN_X),					CHAR_INIT("limit_min_x")},
				{-1.0e308,		(e_double::LIMIT_MIN_Y),					CHAR_INIT("limit_min_y")},
				{1.0e308,		(e_double::LIMIT_MAX_X),					CHAR_INIT("limit_max_x")},
				{1.0e308,		(e_double::LIMIT_MAX_Y),					CHAR_INIT("limit_max_y")}
			};
			const SuperMap<bool> e_boolean_defaults = {
				{false,			(e_boolean::RESPECT_LIMITS),				CHAR_INIT("respect_limits")},
				{false,			(e_boolean::READONLY_NOW),					CHAR_INIT("readonly_now")}
			};
			const SuperMap<int> e_integer_defaults = {
				{0,				(e_integer::ID),							CHAR_INIT("id")}
			};
		}

		class Camera {
			struct camera_data {
				class layer_config {
					int id;
					double elasticity = -1;
				public:
					layer_config() = default;
					layer_config(const int a, const double b) { id = a; elasticity = b; }
					layer_config(const layer_config& o) { id = o.getID(); elasticity = o.getElasticity(); }
					int getID() const { return id; }
					bool hasCollision() const { return elasticity < 0.0; }
					double getElasticity() const { return elasticity; }
					void setID(const int a) { id = a; }
					void setElasticity(const double a) { if (a < 0.0) elasticity = -1.0; else elasticity = a; }
				};
				SuperMap<double>	double_data  = camera::e_double_defaults;
				SuperMap<bool>		boolean_data = camera::e_boolean_defaults;
				SuperMap<int>		integer_data = camera::e_integer_defaults;
				ALLEGRO_TRANSFORM transformation{};
				std::vector<layer_config> layers;
			} data;

			static Camera* last_camera_applied;

			ALLEGRO_TRANSFORM transf(ALLEGRO_BITMAP*, const float, const float, const float, const float, const float);
		public:
			Camera();
			Camera(Camera&);

			void reset();

			void setLayer(const camera_data::layer_config);
			void delLayer(const int);
			camera_data::layer_config* getLayer(const int);

			void set(const camera::e_double, double);
			void set(const camera::e_boolean, bool);
			void set(const camera::e_integer, int);
			void set(const std::string, double);
			void set(const std::string, bool);
			void set(const std::string, int);
			template<typename T, typename V> inline void s(const T a, V b) { set(a, b); };

			bool get(const camera::e_double, double&);
			bool get(const camera::e_boolean, bool&);
			bool get(const camera::e_integer, int&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, int&);

			double*	getRef(const camera::e_double);		// Direct reference to the value
			bool*	getRef(const camera::e_boolean);	// Direct reference to the value
			int*	getRef(const camera::e_integer);	// Direct reference to the value

			template<typename T, typename V> inline bool g(const T a, V& b) { return get(a, b); };
			template<typename T, typename V> inline bool s(const T a, V& b) { return set(a, b); };
			template<typename T, typename V> inline bool isEq(const T e, const V v) { V k; if (get(e, k)) return v == k; return false; }
			template<typename T, typename V> inline bool ie(const T e, const V v) { return isEq(e, v); };

			void refresh();
			void apply();
			Camera* getLastCameraApply() const;

			Camera::camera_data& copyRAW();
		};


		// ----------------------------------------------
		// Camera does not need a template for ResourceOf
		// ----------------------------------------------
	}
}