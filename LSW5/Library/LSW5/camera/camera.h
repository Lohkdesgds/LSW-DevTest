#pragma once

// C
#include <allegro5/allegro.h>
// C++
#include <map>
#include <vector>
// Others
#include "..\supermap\supermap.h"
#include "..\tools\tools.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\superresource\superresource.h"

namespace LSW {
	namespace v5 {
		namespace camera {

			constexpr double roughness_default = 0.999;

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
		public:
			class layer_each {
				int layer;
				double elasticity = -1;
				double roughness = camera::roughness_default;
			public:
				layer_each() = default;
				// layer, elasticity, roughness
				layer_each(const int, const double = -1, const double = camera::roughness_default);
				layer_each(const layer_each&);
				int getLayerID() const;
				bool hasCollision() const;
				double getElasticity() const;
				double getRoughness() const;
				void setLayerID(const int);
				void setElasticity(const double);
				void setRoughness(const double);
			};
		private:
			struct Camera_configuration { // THE configuration of a Camera
				SuperMap<double>	double_data = camera::e_double_defaults;	// its double values
				SuperMap<bool>		boolean_data = camera::e_boolean_defaults;	// its boolean values
				SuperMap<int>		integer_data = camera::e_integer_defaults;	// its integer values
				ALLEGRO_TRANSFORM transformation{};								// its transformation (can refresh() based on ^^)
				std::vector<layer_each> layers;									// its way to handle different layers
			};

			Camera_configuration data;
			static Camera* last_camera_applied;

			ALLEGRO_TRANSFORM transf(ALLEGRO_BITMAP*, const float, const float, const float, const float, const float);
		public:
			Camera();
			Camera(Camera&);

			void reset();

			void addLayerConfig(const layer_each);
			void delLayerConfig(const int);
			layer_each* getLayerConfig(const int);

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

			void matrix_debug();

			Camera::Camera_configuration& copyRAW();
		};


		template <> SuperResource<Camera>::_data<Camera>	SuperResource<Camera>::data = { lambda_default_load<Camera>, lambda_default_unload<Camera> };
	}
}