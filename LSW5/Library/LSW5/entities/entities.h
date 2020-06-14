#pragma once

#include <numeric>

#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\tools\tools.h"

#undef max

namespace LSW {
	namespace v5 {

		namespace entity {

			enum class e_chronomillis_readonly { LAST_FRAME };
			enum class e_integer { FRAME };
			enum class e_double { FRAMES_PER_SECOND };

			const SuperMap<std::chrono::milliseconds> e_chronomillis_readonly_defaults{
				{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()),		(e_chronomillis_readonly::LAST_FRAME),				CHAR_INIT("last_frame")}
			};
			const SuperMap<int> e_integer_defaults = {
				{0,																													(e_integer::FRAME),									CHAR_INIT("frame")}
			};
			const SuperMap<double> e_double_defaults = {
				{10.0,																												(e_double::FRAMES_PER_SECOND),						CHAR_INIT("frames_per_second")}
			};

			constexpr size_t maximum_bitmap_amount = static_cast<size_t>(std::numeric_limits<int>::max());
			constexpr size_t static_frame = static_cast<size_t>(-1); // static frame X?
		}

		class Entity : public Sprite_Base {
			struct _bitmap {
				ALLEGRO_BITMAP* ref = nullptr;
				std::string source, id;
				bool is_sub_bitmap = false; // if true, source is source's id, else source is path
			};

			struct entity_data {
				SuperMap<int>						integer_data				= entity::e_integer_defaults;
				SuperMap<double>					double_data					= entity::e_double_defaults;
				SuperMap<std::chrono::milliseconds> chronomillis_readonly_data	= entity::e_chronomillis_readonly_defaults;
			} data;

			std::vector<_bitmap> bitmaps;


			//ALLEGRO_BITMAP* bmp = nullptr;

			void draw_self();
		public:
			Entity();
			Entity(Entity& o) : Sprite_Base(o) {}
			//~Entity(); // bitmaps are handled by SuperResource, there might be some other people around using the same resource

			// id, path
			void load(const std::string, const std::string);
			// id, from what id copy, x, y, w, h
			void loadCut(const std::string, const std::string, const int, const int, const int, const int);
			// removes from its list all matches
			void remove(const std::string);


			void set(const entity::e_integer, int);
			void set(const entity::e_double, double);
			void set(const std::string, int);
			void set(const std::string, double);

			bool get(const entity::e_integer, int&);
			bool get(const entity::e_double, double&);
			bool get(const std::string, int&);
			bool get(const std::string, double&);

			bool get(const entity::e_chronomillis_readonly, std::chrono::milliseconds&);

		};
	}
}