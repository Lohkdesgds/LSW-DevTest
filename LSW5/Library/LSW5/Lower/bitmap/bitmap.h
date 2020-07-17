#pragma once

// C++
#include <string>
#include <functional>
// Others
#include "..\..\allegroinitialization\allegroinitialization.h"


namespace LSW {
	namespace v5 {
			
		class Bitmap {
			std::function<ALLEGRO_BITMAP*(void)> bitmap;
			static std::function<ALLEGRO_BITMAP*(void)> target;
			bool use_target_as_it = false;

			// set new at bitmap (set(al_load_bitmap...) or something like that)
			bool set(ALLEGRO_BITMAP*);
			// need it quick?
			ALLEGRO_BITMAP* quick() const;
			// need it quick?
			ALLEGRO_BITMAP* quick();
		public:
			Bitmap();

			// if custom destructor is needed, can do nothing if not defined
			bool custom(ALLEGRO_BITMAP*, std::function<void(ALLEGRO_BITMAP*&)> = std::function<void(ALLEGRO_BITMAP*&)>());
			// custom for display, just add, no function /// TODO: later when class Display is created, change this to work with that.
			bool custom(ALLEGRO_DISPLAY*);

			// width, height
			bool create(const int, const int);
			// from, posx, posy, width, height
			bool create_sub(ALLEGRO_BITMAP*, const int, const int, const int, const int);
			// from, posx, posy, width, height
			bool create_sub(Bitmap&, const int, const int, const int, const int);

			operator bool() const;
			const bool operator!() const;
			const bool operator==(Bitmap&) const;
			const bool operator==(ALLEGRO_BITMAP*&) const;
			ALLEGRO_BITMAP* operator*();

			// path
			bool load(const char*);
			// path
			bool save(const char*);

			// become part of target
			void be_reference_to_target(const bool);

			// from
			bool clone(ALLEGRO_BITMAP*);
			// from
			bool clone(Bitmap&);
				
			const int get_flags() const;
			const int get_format() const;
			const int get_width() const;
			const int get_height() const;
				
			// posx, posy
			const ALLEGRO_COLOR get_pixel(const int, const int) const;
				
			const bool is_locked() const;
			const bool is_compatible_with_current_display() const;
			const bool is_sub() const;
				
			// color
			void clear_to_color(ALLEGRO_COLOR);

			// posx, posy, flags
			void draw(const int, const int, const int = 0);
			// posx, posy, width, height, flags
			void draw(const int, const int, const int, const int, const int = 0);
			// thiscut_x, thiscut_y, thiscut_w, thiscut_h, posx, posy, width, height, flags
			void draw(const int, const int, const int, const int, const int, const int, const int, const int, const int = 0);
			// cast to color, posx, posy, width, height, flags
			void draw(ALLEGRO_COLOR, const int, const int, const int, const int, const int = 0);
			// cast to color, thiscut_x, thiscut_y, thiscut_w, thiscut_h, posx, posy, width, height, flags
			void draw(ALLEGRO_COLOR, const int, const int, const int, const int, const int, const int, const int, const int, const int = 0);
			// centerx, centery, posx, posy, scalex, scaley, angle (rad), flags
			void draw(const float, const float, const float, const float, const float, const float, const float = 0.0, const int = 0);
			// cast to color, centerx, centery, posx, posy, scalex, scaley, angle (rad), flags
			void draw(ALLEGRO_COLOR, const float, const float, const float, const float, const float, const float, const float = 0.0, const int = 0);

			// color
			void mask_off(ALLEGRO_COLOR);

			void set_as_target();
			void set_as_reference_target();
		};
	}
}