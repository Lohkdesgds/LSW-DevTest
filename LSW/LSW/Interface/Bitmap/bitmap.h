#pragma once

// C++
#include <string>
#include <functional>
// Others
#include "..\..\Handling\Initialize\initialize.h"
#include "..\Color\color.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace bitmap {
				constexpr int default_new_bitmap_flags = ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR;
			}

			class Bitmap {
				static bool first_time;

				std::function<ALLEGRO_BITMAP*(void)> bitmap; // has shared ptr there
				static std::function<ALLEGRO_BITMAP*(void)> target;
				bool use_target_as_it = false;

				// set new at bitmap (set(al_load_bitmap...) or something like that)
				bool set(ALLEGRO_BITMAP*);
				// set bitmap with no del
				bool set_nodel(ALLEGRO_BITMAP*);

				// need it quick?
				ALLEGRO_BITMAP* quick() const;
				// need it quick?
				ALLEGRO_BITMAP* quick();
			public:
				Bitmap();
				// clone
				Bitmap(const Bitmap&);

				Bitmap& operator=(const Bitmap&);
				Bitmap& operator=(Bitmap&&);

				// if custom destructor is needed, can do nothing if not defined
				//bool custom(ALLEGRO_BITMAP*, std::function<void(ALLEGRO_BITMAP*&)> = std::function<void(ALLEGRO_BITMAP*&)>());
				// custom for display, just add, no function /// TODO: later when class Display is created, change this to work with that.
				//bool custom(ALLEGRO_DISPLAY*);

				// width, height
				bool create(const int, const int);
				// from, posx, posy, width, height
				//bool create_sub(ALLEGRO_BITMAP*, const int, const int, const int, const int);
				// from, posx, posy, width, height
				bool create_sub(Bitmap&, const int, const int, const int, const int);

				operator bool() const;
				const bool operator!() const;
				const bool operator==(Bitmap&) const;
				
				const bool empty() const;

				// set flags of new bitmaps being created (unfortunalety global)
				void set_new_bitmap_flags(const int = bitmap::default_new_bitmap_flags);

				// path
				bool load(const char*);
				// path
				bool save(const char*);
				// force_set
				void force(ALLEGRO_BITMAP*);

				// become part of target
				void be_reference_to_target(const bool);

				// from
				//bool clone(ALLEGRO_BITMAP*);
				// from
				bool clone(Bitmap&);

				const int get_flags() const;
				const int get_format() const;
				const int get_width() const;
				const int get_height() const;

				// posx, posy
				const Color get_pixel(const int, const int) const;

				const bool is_locked() const;
				const bool is_compatible_with_current_display() const;
				const bool is_sub() const;

				// color
				void clear_to_color(Color);

				// posx, posy, flags
				void draw(const float, const float, const int = 0) const;
				// posx, posy, width, height, flags
				void draw(const float, const float, const float, const float, const int = 0) const;
				// thiscut_x, thiscut_y, thiscut_w, thiscut_h, posx, posy, width, height, flags
				void draw(const float, const float, const float, const float, const float, const float, const float, const float, const int = 0) const;
				// cast to color, posx, posy, width, height, flags
				void draw(Color, const float, const float, const float, const float, const int = 0) const;
				// cast to color, thiscut_x, thiscut_y, thiscut_w, thiscut_h, posx, posy, width, height, flags
				void draw(Color, const float, const float, const float, const float, const float, const float, const float, const float, const int = 0) const;
				// centerx, centery, posx, posy, scalex, scaley, angle (rad), flags
				void draw(const float, const float, const float, const float, const float, const float, const float = 0.0, const int = 0) const;
				// cast to color, centerx, centery, posx, posy, scalex, scaley, angle (rad), flags
				void draw(Color, const float, const float, const float, const float, const float, const float, const float = 0.0, const int = 0) const;

				// color
				void mask_off(Color);

				void set_as_target() const;
				void set_as_reference_target();
			};
		}
	}
}