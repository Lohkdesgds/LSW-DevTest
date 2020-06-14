#include "entities.h"

namespace LSW {
	namespace v5 {


		void Entity::draw_self()
		{
			if (bitmaps.empty()) return;

			int& frame = *data.integer_data[entity::e_integer::FRAME];
			double& delta = *data.double_data[entity::e_double::FRAMES_PER_SECOND]; // delta t, 1/t = sec
			std::chrono::milliseconds& last_time = *data.chronomillis_readonly_data[entity::e_chronomillis_readonly::LAST_FRAME];

			if (delta >= 0.0 && frame >= 0) { // if delta <= 0 or frame < 0, static
				std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));
				if (delta_tr.count() > 0) {
					while (std::chrono::system_clock::now().time_since_epoch() > last_time) {
						last_time += delta_tr;
						if (++frame >= bitmaps.size()) frame = 0;
					}
				}
			}
			else {
				frame = frame > 0 ? frame : -frame;
				if (frame >= bitmaps.size()) frame = bitmaps.size() - 1;
			}

			ALLEGRO_BITMAP* rn = bitmaps[frame].ref;
			if (!rn) throw Abort::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


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
		/*Entity::~Entity()
		{
			if (bmp) {
				al_destroy_bitmap(bmp);
				bmp = nullptr;
			}
		}*/
		void Entity::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			_bitmap binfo;

			binfo.ref = bmps.load(id, str);
			binfo.is_sub_bitmap = false;
			binfo.source = str;
			binfo.id = id;

			if (!binfo.ref) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");

			bitmaps.push_back(binfo);

		}
		void Entity::loadCut(const std::string id, const std::string parent_id, const int x, const int y, const int w, const int h)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			ALLEGRO_BITMAP* parent = nullptr;
			if (bmps.get(parent_id, parent))
			{
				_bitmap binfo;

				binfo.source = parent_id;
				binfo.is_sub_bitmap = true;
				binfo.id = id;
				binfo.ref = bmps.customLoad(id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(parent, x, y, w, h); });

				if (binfo.ref)
				{
					bitmaps.push_back(binfo);
					return;
				}
			}
			throw Abort::Abort(__FUNCSIG__, "Cannot get sub part of '" + parent_id + "'!");
		}
		void Entity::remove(const std::string id)
		{
			for (size_t p = 0; p < bitmaps.size(); p++) {
				auto& i = bitmaps[p];
				if (i.id == id) {
					bitmaps.erase(bitmaps.begin() + p);
				}
			}
		}
		void Entity::set(const entity::e_integer e, int v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}
		void Entity::set(const entity::e_double e, double v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Entity::set(const std::string e, int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}
		void Entity::set(const std::string e, double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Entity::get(const entity::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}
		bool Entity::get(const entity::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Entity::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}
		bool Entity::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Entity::get(const entity::e_chronomillis_readonly e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data.chronomillis_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}
	}
}