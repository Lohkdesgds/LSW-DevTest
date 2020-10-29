#include "bitmap.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			bool Bitmap::first_time = true;
			std::function<ALLEGRO_BITMAP*(void)> Bitmap::target;


			bool Bitmap::set(ALLEGRO_BITMAP* nptr)
			{
				if (!nptr) return false;
				auto s = std::shared_ptr<ALLEGRO_BITMAP>(nptr, [](ALLEGRO_BITMAP*& b) { if (al_is_system_installed() && b) { al_destroy_bitmap(b); b = nullptr; } });
				bitmap = [s] {return s.get(); };
				return !(!s);
			}
			bool Bitmap::set_nodel(ALLEGRO_BITMAP* nptr)
			{
				if (!nptr) return false;
				auto s = std::shared_ptr<ALLEGRO_BITMAP>(nptr, [](auto*&) { });
				bitmap = [s] {return s.get(); };
				return !(!s);
			}
			ALLEGRO_BITMAP* Bitmap::quick() const
			{
				if (use_target_as_it) return target ? (target)() : nullptr;
				if (bitmap) return bitmap();
				return nullptr;
			}
			ALLEGRO_BITMAP* Bitmap::quick()
			{
				if (use_target_as_it) return target ? (target)() : nullptr;
				if (bitmap) return bitmap();
				return nullptr;
			}

			int Bitmap::interpret_drawing_mode(const bitmap::drawing_mode e) const
			{
				switch (e) {
				case bitmap::drawing_mode::DEFAULT:
					return 0;
				case bitmap::drawing_mode::FLIP_HORIZONTAL:
					return ALLEGRO_FLIP_HORIZONTAL;
				case bitmap::drawing_mode::FLIP_VERTICAL:
					return ALLEGRO_FLIP_VERTICAL;
				case bitmap::drawing_mode::FLIP_HORIZONTAL_VERTICAL:
					return ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL;
				}
				return 0;
			}

			Bitmap::Bitmap()
			{
				Handling::init_basic();
				Handling::init_graphics();

				if (first_time) {
					first_time = false;
					set_new_bitmap_flags(); // first setup
				}
			}

			Bitmap::Bitmap(const Bitmap& oth)
			{
				bitmap = oth.bitmap;
				use_target_as_it = oth.use_target_as_it;

				if (first_time) {
					first_time = false;
					set_new_bitmap_flags(); // first setup
				}
			}

			Bitmap& Bitmap::operator=(const Bitmap& oth)
			{
				bitmap = oth.bitmap;
				use_target_as_it = oth.use_target_as_it;
				return *this;
			}

			Bitmap& Bitmap::operator=(Bitmap&& oth)
			{
				bitmap = oth.bitmap;
				use_target_as_it = oth.use_target_as_it;
				return *this;
			}

			/*bool Bitmap::custom(ALLEGRO_BITMAP* nptr, std::function<void(ALLEGRO_BITMAP*&)> f)
			{
				if (!nptr) return false;
				auto s = std::shared_ptr<ALLEGRO_BITMAP>(nptr, f);
				bitmap = [s] {return &(*s); };
				return !(!s);
			}

			bool Bitmap::custom(ALLEGRO_DISPLAY* d)
			{
				if (!d) return false;
				bitmap = [d] {
					return al_get_backbuffer(d);
				};
				return true;
			}*/


			bool Bitmap::create(const int w, const int h)
			{
				if (!bitmap) {
					return set(al_create_bitmap(w, h));
				}
				return false;
			}

			/*bool Bitmap::create_sub(ALLEGRO_BITMAP* o, const int x, const int y, const int w, const int h)
			{
				if (!bitmap) {
					return set(al_create_sub_bitmap(o, x, y, w, h));
				}
				return false;
			}*/

			bool Bitmap::create_sub(Bitmap& o, const int x, const int y, const int w, const int h)
			{
				if (!bitmap) {
					return set(al_create_sub_bitmap(o.quick(), x, y, w, h));
				}
				return false;
			}

			Bitmap::operator bool() const
			{
				return !(use_target_as_it ? !target : !bitmap); // ikr but visual studio doesnt
			}

			const bool Bitmap::operator!() const
			{
				return use_target_as_it ? !target : !bitmap;
			}

			const bool Bitmap::operator==(Bitmap& o) const
			{
				return quick() == o.quick();
			}

			const bool Bitmap::empty() const
			{
				return use_target_as_it ? !target : !bitmap;
			}

			void Bitmap::set_new_bitmap_flags(const int flags)
			{
				al_set_new_bitmap_flags(flags);
			}

			/*const bool Bitmap::operator==(ALLEGRO_BITMAP*& o) const
			{
				return quick() == o;
			}

			ALLEGRO_BITMAP* Bitmap::operator*()
			{
				return quick();
			}*/

			bool Bitmap::load(const char* p)
			{
				if (!bitmap) {
					return set(al_load_bitmap(p));
				}
				return false;
			}

			bool Bitmap::save(const char* p)
			{
				if (bitmap) {
					return al_save_bitmap(p, quick());
				}
				return false;
			}

			void Bitmap::force(ALLEGRO_BITMAP* b)
			{
				if (b) set_nodel(b);
			}

			void Bitmap::be_reference_to_target(const bool b)
			{
				use_target_as_it = b;
			}

			/*bool Bitmap::clone(ALLEGRO_BITMAP* o)
			{
				if (!bitmap) {
					return set(al_clone_bitmap(o));
				}
				return false;
			}*/

			bool Bitmap::clone(Bitmap& o)
			{
				if (!bitmap) {
					return set(al_clone_bitmap(o.quick()));
				}
				return false;
			}

			const int Bitmap::get_flags() const
			{
				if (auto q = quick(); q) return al_get_bitmap_flags(q);
				return -1;
			}

			const int Bitmap::get_format() const
			{
				if (auto q = quick(); q) return al_get_bitmap_format(q);
				return -1;
			}

			const int Bitmap::get_width() const
			{
				if (auto q = quick(); q) return al_get_bitmap_width(q);
				return -1;
			}

			const int Bitmap::get_height() const
			{
				if (auto q = quick(); q) return al_get_bitmap_height(q);
				return -1;
			}

			const Color Bitmap::get_pixel(const int x, const int y) const
			{
				if (auto q = quick(); q) return al_get_pixel(q, x, y);
				return Color();
			}

			ALLEGRO_LOCKED_REGION* Bitmap::lock(const int format, const int flags)
			{
				return quick() ? al_lock_bitmap(quick(), format, flags) : nullptr;
			}

			ALLEGRO_LOCKED_REGION* Bitmap::lock(const int x, const int y, const int w, const int h, const int format, const int flags)
			{
				return quick() ? al_lock_bitmap_region(quick(), x, y, w, h, format, flags) : nullptr;
			}

			void Bitmap::unlock()
			{
				if (quick() && is_locked()) al_unlock_bitmap(quick());
			}

			const bool Bitmap::is_locked() const
			{
				if (auto q = quick(); q) return al_is_bitmap_locked(q);
				return false;
			}

			const bool Bitmap::is_compatible_with_current_display() const
			{
				if (auto q = quick(); q) return al_is_compatible_bitmap(q);
				return false;
			}

			const bool Bitmap::is_sub() const
			{
				if (auto q = quick(); q) return al_is_sub_bitmap(q);
				return false;
			}

			void Bitmap::clear_to_color(Color c)
			{
				if (auto q = quick(); q) {
					ALLEGRO_BITMAP* cpy = al_get_target_bitmap();
					al_set_target_bitmap(q);
					al_clear_to_color(c);
					if (cpy) al_set_target_bitmap(cpy);
				}
			}

			void Bitmap::draw(const float x, const float y, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_bitmap(q, x, y, interpret_drawing_mode(f));
			}

			void Bitmap::draw(const float x, const float y, const float w, const float h, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_scaled_bitmap(q, 0, 0, get_width(), get_height(), x, y, w, h, interpret_drawing_mode(f));
			}

			void Bitmap::draw(const float x1, const float y1, const float w1, const float h1, const float x2, const float y2, const float w2, const float h2, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_scaled_bitmap(q, x1, y1, w1, h1, x2, y2, w2, h2, interpret_drawing_mode(f));
			}

			void Bitmap::draw(Color c, const float x, const float y, const float w, const float h, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_tinted_scaled_bitmap(q, c, 0, 0, get_width(), get_height(), x, y, w, h, interpret_drawing_mode(f));
			}

			void Bitmap::draw(Color c, const float x1, const float y1, const float w1, const float h1, const float x2, const float y2, const float w2, const float h2, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_tinted_scaled_bitmap(q, c, x1, y1, w1, h1, x2, y2, w2, h2, interpret_drawing_mode(f));
			}

			void Bitmap::draw(const float cx, const float cy, const float px, const float py, const float sx, const float sy, const float a, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_scaled_rotated_bitmap(q, cx, cy, px, py, sx, sy, a, interpret_drawing_mode(f));
			}

			void Bitmap::draw(Color c, const float cx, const float cy, const float px, const float py, const float sx, const float sy, const float a, const bitmap::drawing_mode f) const
			{
				if (auto q = quick(); q) al_draw_tinted_scaled_rotated_bitmap(q, c, cx, cy, px, py, sx, sy, a, interpret_drawing_mode(f));
			}

			void Bitmap::mask_off(Color c)
			{
				if (auto q = quick(); q) al_convert_mask_to_alpha(q, c);
			}

			void Bitmap::set_as_target() const
			{
				if (auto q = quick(); q) {
					al_set_target_bitmap(q);
				}
			}

			void Bitmap::set_reference_as_target() const
			{
				if (!target) return;
				if (auto cpy = target(); cpy) al_set_target_bitmap(cpy);
			}

			bool Bitmap::has_global_reference_set() const
			{
				return target ? target() : false;
			}

			void Bitmap::copy_reference_to_this()
			{
				if (auto q = quick(); q) {
					target = bitmap;
				}
			}
			void Bitmap::set_custom_reference(std::function<ALLEGRO_BITMAP*(void)> f)
			{
				if (f()) {
					target = f;
				}
			}
		}
	}
}