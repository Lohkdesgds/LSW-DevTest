#include "entity.h"

namespace LSW {
	namespace v5 {
		void Entity::draw_self()
		{
			if (!bmp) return;
			ALLEGRO_BITMAP* rn = bmp;

			float cx, cy, px, py, dsx, dsy, rot_rad;
			int bmpx, bmpy;
			bmpx = al_get_bitmap_width(rn);
			bmpy = al_get_bitmap_height(rn);
			if (bmpx <= 0 || bmpy <= 0) {
				throw Abort::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
			}

			cx = 1.0f * bmpx * ((*getRef(sprite::e_double::CENTER_X) + 1.0) * 0.5);
			cy = 1.0f * bmpy * ((*getRef(sprite::e_double::CENTER_Y) + 1.0) * 0.5);
			rot_rad = 1.0f * *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			/*px = 1.0f * data.dval[+Constants::io__sprite_double::POSX] * cos(rot_rad) + data.dval[+Constants::io__sprite_double::POSY] * sin(rot_rad);
			py = 1.0f * data.dval[+Constants::io__sprite_double::POSY] * cos(rot_rad) - data.dval[+Constants::io__sprite_double::POSX] * sin(rot_rad);*/
			px = *getRef(sprite::e_double_readonly::POSX);
			py = *getRef(sprite::e_double_readonly::POSY);
			dsx = 1.0f * *getRef(sprite::e_double::SCALE_X) * *getRef(sprite::e_double::SCALE_G) * (1.0 / bmpx);
			dsy = 1.0f * *getRef(sprite::e_double::SCALE_Y) * *getRef(sprite::e_double::SCALE_G) * (1.0 / bmpy);


			if (*getRef(sprite::e_boolean::USE_COLOR)) {
				al_draw_tinted_scaled_rotated_bitmap(rn, *getRef(sprite::e_color::COLOR), cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}
			else {
				al_draw_scaled_rotated_bitmap(rn, cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}

		}
		Entity::Entity() : Sprite_Base()
		{
			custom_draw_task = [&] {draw_self(); };
		}
		Entity::~Entity()
		{
			if (bmp) {
				al_destroy_bitmap(bmp);
				bmp = nullptr;
			}
		}
		void Entity::load(const std::string str)
		{
			if (!al_get_target_bitmap()) {
				int cpy = al_get_new_bitmap_flags();
				al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
				bmp = al_load_bitmap(str.c_str());
				al_set_new_bitmap_flags(cpy);
			}
			else bmp = al_load_bitmap(str.c_str());

			if (!bmp) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + str + "'!");
		}
	}
}