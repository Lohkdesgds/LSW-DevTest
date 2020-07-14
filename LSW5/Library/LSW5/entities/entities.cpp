#include "entities.h"

namespace LSW {
	namespace v5 {


		void Block::draw_self()
		{
			if (bitmaps.empty()) return;

			{
				auto& delta_t = *data_block->chronomillis_readonly_data[block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION];

				if (const double _dd = (*data_block->double_data[block::e_double::TIE_SIZE_TO_DISPLAY_PROPORTION])(); _dd > 0.0 && (std::chrono::system_clock::now().time_since_epoch() > delta_t)) {
					delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + block::default_delta_t_frame_delay);
					ALLEGRO_BITMAP* trg = al_get_target_bitmap();
					if (trg) {
						SuperResource<ALLEGRO_BITMAP> bmps;
						int tx = al_get_bitmap_width(trg) * _dd;
						int ty = al_get_bitmap_height(trg) * _dd;

						for (auto& i : bitmaps) {
							int _w = al_get_bitmap_width(&(*i.ref));
							int _h = al_get_bitmap_height(&(*i.ref));
							if (_w == tx && _h == ty) continue; // no need to "reload"

							auto nww = bmps.swapCustomLoad(i.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(tx, ty); });
							if (!nww) nww = bmps.customLoad(i.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(tx, ty); });
							al_set_target_bitmap(&(*nww));
							al_draw_scaled_bitmap(&(*i.ref), 0, 0, al_get_bitmap_width(&(*i.ref)), al_get_bitmap_height(&(*i.ref)), 0, 0, al_get_bitmap_width(trg), al_get_bitmap_height(trg), 0);
							i.ref = nww;
						}
						al_set_target_bitmap(trg);
					}
				}
			}

			int frame = (*data_block->integer_data[block::e_integer::FRAME])();

			{
				const double delta = (*data_block->double_data[block::e_double::FRAMES_PER_SECOND])(); // delta t, 1/t = sec
				std::chrono::milliseconds& last_time = *data_block->chronomillis_readonly_data[block::e_chronomillis_readonly::LAST_FRAME];

				if (delta > 0.0 && frame >= 0) { // if delta <= 0 or frame < 0, static
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
					if (frame >= bitmaps.size()) frame = static_cast<int>(bitmaps.size() - 1);
				}
			}

			if (!(*data_block->boolean_data[block::e_boolean::SET_FRAME_VALUE_READONLY])()) {
				*data_block->integer_data[block::e_integer::FRAME] = [=]{return frame;};
			}

			ALLEGRO_BITMAP* rn = &(*bitmaps[frame].ref);
			if (!rn) throw Abort::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


			float cx, cy, px, py, dsx, dsy, rot_rad;
			int bmpx, bmpy;
			bmpx = al_get_bitmap_width(rn);
			bmpy = al_get_bitmap_height(rn);
			if (bmpx <= 0 || bmpy <= 0) {
				throw Abort::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
			}

			cx = 1.0f * bmpx * (((*getRef(sprite::e_double::CENTER_X))() + 1.0) * 0.5);
			cy = 1.0f * bmpy * (((*getRef(sprite::e_double::CENTER_Y))() + 1.0) * 0.5);
			rot_rad = 1.0f * *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			/*px = 1.0f * data.dval[+Constants::io__sprite_double::POSX] * cos(rot_rad) + data.dval[+Constants::io__sprite_double::POSY] * sin(rot_rad);
			py = 1.0f * data.dval[+Constants::io__sprite_double::POSY] * cos(rot_rad) - data.dval[+Constants::io__sprite_double::POSX] * sin(rot_rad);*/
			px = *getRef(sprite::e_double_readonly::POSX);
			py = *getRef(sprite::e_double_readonly::POSY);
			dsx = 1.0f * (*getRef(sprite::e_double::SCALE_X))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpx);
			dsy = 1.0f * (*getRef(sprite::e_double::SCALE_Y))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpy);


			if ((*getRef(sprite::e_boolean::USE_COLOR))()) {
				al_draw_tinted_scaled_rotated_bitmap(rn, (*getRef(sprite::e_color::COLOR))(), cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}
			else {
				al_draw_scaled_rotated_bitmap(rn, cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}

		}
		Block::Block() : Sprite_Base()
		{
			if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };
		}
		Block::Block(Block& o) : Sprite_Base(o)
		{
			if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };
		}
		void Block::twinUpAttributes(const std::shared_ptr<block_data> oth)
		{
			data_block = oth; // now they're the same
		}
		std::shared_ptr<Block::block_data> Block::getAttributes()
		{
			return data_block;
		}
		/*Block::~Block()
		{
			if (bmp) {
				al_destroy_bitmap(bmp);
				bmp = nullptr;
			}
		}*/
		void Block::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			_bitmap binfo;

			if (binfo.ref = bmps.load(id, str)) {
				binfo.is_sub_bitmap = false;
				binfo.source = str;
				binfo.id = id;

				bitmaps.push_back(binfo);
				return;
			}
			throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");
		}
		void Block::load()
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			_bitmap binfo;

			if (binfo.ref = bmps.getMain()) {
				binfo.is_sub_bitmap = false;
				binfo.source.clear(); // main
				binfo.id.clear(); // main

				bitmaps.push_back(binfo);
				return;
			}
			throw Abort::Abort(__FUNCSIG__, "There's no MAIN TEXTURE set! (See SuperResource.setMain(...))");
		}
		void Block::loadCut(const std::string parent_id, const int x, const int y, const int w, const int h)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			std::shared_ptr<ALLEGRO_BITMAP> parent;
			if (bmps.get(parent_id, parent))
			{
				_bitmap binfo;

				binfo.source = parent_id;
				binfo.is_sub_bitmap = true;
				binfo.id = parent_id + "_cut&" + Tools::generateRandomUniqueStringN();
				binfo.ref = bmps.customLoad(binfo.id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(&(*parent), x, y, w, h); });

				if (binfo.ref)
				{
					bitmaps.push_back(binfo);
					return;
				}
			}
			throw Abort::Abort(__FUNCSIG__, "Cannot get sub part of '" + parent_id + "'!");
		}
		void Block::loadCut(const int x, const int y, const int w, const int h)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			if (std::shared_ptr<ALLEGRO_BITMAP> parent = bmps.getMain())
			{
				_bitmap binfo;

				binfo.source.clear(); // main
				binfo.is_sub_bitmap = true;
				binfo.id = "_cut&" + Tools::generateRandomUniqueStringN();
				binfo.ref = bmps.customLoad(binfo.id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(&(*parent), x, y, w, h); });

				if (binfo.ref)
				{
					bitmaps.push_back(binfo);
					return;
				}
			}
			throw Abort::Abort(__FUNCSIG__, "There's no MAIN TEXTURE set! (See SuperResource.setMain(...))");
		}
		void Block::remove(const std::string id)
		{
			for (size_t p = 0; p < bitmaps.size(); p++) {
				auto& i = bitmaps[p];
				if (i.id == id) {
					bitmaps.erase(bitmaps.begin() + p);
				}
			}
		}
		void Block::set(const block::e_integer e, int v)
		{
			if (auto* ptr = data_block->integer_data(e); ptr)
				*ptr = [=] {return v; };
		}
		void Block::set(const block::e_double e, double v)
		{
			if (auto* ptr = data_block->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Block::set(const block::e_boolean e, bool v)
		{
			if (auto* ptr = data_block->boolean_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Block::set(const block::e_integer e, std::function<int(void)> v)
		{
			if (auto* ptr = data_block->integer_data(e); ptr)
				*ptr = v;
		}

		void Block::set(const block::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_block->double_data(e); ptr)
				*ptr = v;
		}

		void Block::set(const block::e_boolean e, std::function<bool(void)> v)
		{
			if (auto* ptr = data_block->boolean_data(e); ptr)
				*ptr = v;
		}

		void Block::set(const std::string e, int v)
		{
			auto* ptr = data_block->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}
		void Block::set(const std::string e, double v)
		{
			auto* ptr = data_block->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Block::set(const std::string e, bool v)
		{
			auto* ptr = data_block->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Block::set(const std::string e, std::function<int(void)> v)
		{
			auto* ptr = data_block->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}
		void Block::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_block->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}
		void Block::set(const std::string e, std::function<bool(void)> v)
		{
			auto* ptr = data_block->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		bool Block::get(const block::e_integer e, int& v)
		{
			if (auto* ptr = data_block->integer_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}
		bool Block::get(const block::e_double e, double& v)
		{
			if (auto* ptr = data_block->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Block::get(const block::e_boolean e, bool& v)
		{
			if (auto* ptr = data_block->boolean_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Block::get(const block::e_integer e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_block->integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Block::get(const block::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_block->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Block::get(const block::e_boolean e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_block->boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Block::get(const std::string e, int& v)
		{
			if (auto* ptr = data_block->integer_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}
		bool Block::get(const std::string e, double& v)
		{
			if (auto* ptr = data_block->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Block::get(const std::string e, bool& v)
		{
			if (auto* ptr = data_block->boolean_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool LSW::v5::Block::get(const std::string e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_block->integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool LSW::v5::Block::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_block->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool LSW::v5::Block::get(const std::string e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_block->boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Block::get(const block::e_chronomillis_readonly e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data_block->chronomillis_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		std::function<int(void)>* Block::getRef(const block::e_integer e)
		{
			if (auto* ptr = data_block->integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<double(void)>* Block::getRef(const block::e_double e)
		{
			if (auto* ptr = data_block->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<bool(void)>* Block::getRef(const block::e_boolean e)
		{
			if (auto* ptr = data_block->boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::chrono::milliseconds* Block::getRef(const block::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data_block->chronomillis_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}


		void Text::__link_hard_task()
		{
			/*SuperResource<Camera> cameras;
			//if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
			std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera*/
			//std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
			//Database conf;
			//ALLEGRO_DISPLAY* d = al_get_current_display();


			data_text->custom_tags = {
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSX) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0)));
					},
						CHAR_INIT("%pos_x%"), 						(text::e_custom_tags::T_POSX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSY) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0)));
					},
						CHAR_INIT("%pos_y%"), 						(text::e_custom_tags::T_POSY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSX) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0)));
					},
						CHAR_INIT("%screen_pos_x%"), 				(text::e_custom_tags::T_SCREEN_POSX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSY) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0)));

					},
						CHAR_INIT("%screen_pos_y%"), 				(text::e_custom_tags::T_SCREEN_POSY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						return Tools::sprintf_a("%s", (follow ? "Y" : "N"));
					},
						CHAR_INIT("%is_following%"), 				(text::e_custom_tags::T_ISFOLLOWING)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().r);
					},
						CHAR_INIT("%color_r%"), 					(text::e_custom_tags::T_COLOR_R)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().g);
					},
						CHAR_INIT("%color_g%"), 					(text::e_custom_tags::T_COLOR_G)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().b);
					},
						CHAR_INIT("%color_b%"), 					(text::e_custom_tags::T_COLOR_B)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().a);
					},
						CHAR_INIT("%color_a%"), 					(text::e_custom_tags::T_COLOR_A)
				},
				{
					[&] {
						return Tools::sprintf_a("%d", (*data_text->integer_data[text::e_integer::STRING_MODE])());
					},
						CHAR_INIT("%mode%"), 						(text::e_custom_tags::T_MODE)
				},
				{
					[&] {
						Database conf;
						return Tools::sprintf_a("%.3lf", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - *conf.getRef(database::e_chronomillis_readonly::STARTED_APP_TIME)).count() / 1000.0);
					},
						CHAR_INIT("%time%"), 						(text::e_custom_tags::T_TIME)
				},
				{
					[&] {
						Database conf;
						return Tools::sprintf_a("%s", *conf.getRef(database::e_boolean::DOUBLE_BUFFERING) ? "Y" : "N");
					},
						CHAR_INIT("%is_using_buf%"), 				(text::e_custom_tags::T_ISUSINGBUF)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_width(d) : -1));

					},
						CHAR_INIT("%curr_res_x%"), 					(text::e_custom_tags::T_GB_RESX)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_height(d) : -1));
					},
						CHAR_INIT("%curr_res_y%"), 					(text::e_custom_tags::T_GB_RESY)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_refresh_rate(d) : -1));
					},
						CHAR_INIT("%curr_refresh_rate%"), 			(text::e_custom_tags::T_REFRESHRATE)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::FRAMESPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_fps%"), 					(text::e_custom_tags::T_FPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::COLLISIONSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);

					},
						CHAR_INIT("%int_tps%"), 					(text::e_custom_tags::T_TPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::USEREVENTSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_ups%"), 					(text::e_custom_tags::T_UPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::ADVANCEDFUNCSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_aps%"), 					(text::e_custom_tags::T_APS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::MUSICSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_mps%"), 					(text::e_custom_tags::T_MPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_fps%"), 				(text::e_custom_tags::T_I_FPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_tps%"), 				(text::e_custom_tags::T_I_TPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_ups%"), 				(text::e_custom_tags::T_I_UPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_aps%"), 				(text::e_custom_tags::T_I_APS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_MUSICSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_mps%"), 				(text::e_custom_tags::T_I_MPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_fps%"), 						(text::e_custom_tags::T_MS_FPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_tps%"), 						(text::e_custom_tags::T_MS_TPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_ups%"), 						(text::e_custom_tags::T_MS_UPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_aps%"), 						(text::e_custom_tags::T_MS_APS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_MUSICSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_aps%"), 						(text::e_custom_tags::T_MS_MPS)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0));
					},
						CHAR_INIT("%cam_x%"), 						(text::e_custom_tags::T_CAM_X)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0));
					},
						CHAR_INIT("%cam_y%"), 						(text::e_custom_tags::T_CAM_Y)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? (*cam->getRef(camera::e_double::SCALE_G) * sqrt(*cam->getRef(camera::e_double::SCALE_X) * *cam->getRef(camera::e_double::SCALE_Y))) : 0.0));
					},
						CHAR_INIT("%cam_zoom_combined%"), 			(text::e_custom_tags::T_CAM_ZOOM)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_G) : 0.0));
					},
						CHAR_INIT("%cam_zoom_g%"), 					(text::e_custom_tags::T_CAM_ZOOMG)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_X) : 0.0));
					},
						CHAR_INIT("%cam_zoom_x%"), 					(text::e_custom_tags::T_CAM_ZOOMX)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_Y) : 0.0));
					},
						CHAR_INIT("%cam_zoom_y%"), 					(text::e_custom_tags::T_CAM_ZOOMY)
				},
				{
					[&] {
						Database conf;
						std::string str;
						conf.get(database::e_string::CURRENT_STRING, str);
						return str;
					},
						CHAR_INIT("%curr_string%"), 				(text::e_custom_tags::T_CURRSTRING)
				},
				{
					[&] {
						Database conf;
						std::string str;
						conf.get(database::e_string::LAST_STRING, str);
						return str;
					},
						CHAR_INIT("%last_string%"), 				(text::e_custom_tags::T_LASTSTRING)
				},
				{
					[&] {
						Database conf;
						double x;
						conf.get(database::e_double::MOUSE_X, x);
						return Tools::sprintf_a("%.3lf", x);
					},
						CHAR_INIT("%mouse_x%"), 					(text::e_custom_tags::T_MOUSE_X)
				},
				{
					[&] {
						Database conf;
						double x;
						conf.get(database::e_double::MOUSE_Y, x);
						return Tools::sprintf_a("%.3lf", x);
					},
						CHAR_INIT("%mouse_y%"), 					(text::e_custom_tags::T_MOUSE_Y)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							double val;
							follow->get(sprite::e_double_readonly::SPEED_X, val);
							return Tools::sprintf_a("%.3lf", val);
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_speed_x%"), 				(text::e_custom_tags::T_SPRITE_SPEEDX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							double val;
							follow->get(sprite::e_double_readonly::SPEED_Y, val);
							return Tools::sprintf_a("%.3lf", val);
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_speed_y%"), 				(text::e_custom_tags::T_SPRITE_SPEEDY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							std::string val;
							follow->get(sprite::e_string::ID, val);
							return val;
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_name%"), 				(text::e_custom_tags::T_SPRITE_NAME)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							if (follow->isThisEntity()) {
								std::string nickname;
								Entity* lmao = (Entity*)follow;
								lmao->get(Constants::io__entity_string::NICKNAME, nickname);
								sprintf_s(tempstr_c, "%s", nickname.c_str());
							}
							else sprintf_s(tempstr_c, "NULL");
						}
						else sprintf_s(tempstr_c, "NULL");*/
					},
						CHAR_INIT("%entity_name%"), 				(text::e_custom_tags::T_SPRITE_ENTITY_NAME)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							if (follow->isThisEntity()) {
								double health = 0.0;
								Entity* lmao = (Entity*)follow;
								lmao->get(Constants::io__entity_double::HEALTH, health);
								sprintf_s(tempstr_c, "%.0lf%c", 100.0 * health, '%');
							}
							else sprintf_s(tempstr_c, "NULL");
						}
						else sprintf_s(tempstr_c, "NULL");*/
					},
						CHAR_INIT("%entity_health%"), 				(text::e_custom_tags::T_SPRITE_ENTITY_HEALTH)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							std::string str = follow->__debug_str();

							if (str.length() >= 128) str = str.substr(0, 124) + "...";

							if (str.length() > 0) sprintf_s(tempstr_c, "%s", str.c_str());
							else sprintf_s(tempstr_c, "NO INFORMATION");
						}
						else sprintf_s(tempstr_c, "NOT_IMPLEMENTED");*/
					},
						CHAR_INIT("%sprite_debug%"), 				(text::e_custom_tags::_T_SPRITE_DEBUG)
				},
				{
					[&] {
						SuperResource<ALLEGRO_BITMAP> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_images%"), 					(text::e_custom_tags::T_TEXTURES_LOADED)
				},
				{
					[&] {
						SuperResource<ALLEGRO_FONT> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_fonts%"), 					(text::e_custom_tags::T_FONTS_LOADED)
				},
				{
					[&] {
						SuperResource<Sprite_Base> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_sprites%"), 				(text::e_custom_tags::T_SPRITES_LOADED)
				},
				{
					[&] {
						/*ResourceOf<Track> tracks;
						sprintf_s(tempstr_c, "%zu", tracks.size());*/
						return std::string("NOT_AVAILABLE");
					},
						CHAR_INIT("%num_tracks%"), 					(text::e_custom_tags::T_TRACKS_LOADED)
				},
				{
					[&] {
						/*Mixer mixer;
						sprintf_s(tempstr_c, "%.0lf", mixer.getVolume() * 100.0);*/
						return std::string("NOT_AVAILABLE");
					},
						CHAR_INIT("%volume_perc%"), 				(text::e_custom_tags::T_VOLUME)
				},
				{
					[&] {
						return Shared::version_app;
					},
						CHAR_INIT("%version%"), 					(text::e_custom_tags::T_VERSION)
				},
				{
					[&] {
						Database conf;
						if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) {
							double d = 1.0;
							conf.get(database::e_double::RESOLUTION_BUFFER_PROPORTION, d);
							d *= 100.0;
							return Tools::sprintf_a("%.2lf", d);
						}
						return std::string("Disabled");
					},
						CHAR_INIT("%screen_buf_proportion%"), 		(text::e_custom_tags::T_RESOLUTION_PROPORTION)
				},
				{
					[&] {
						Database conf;
						if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) {
							double d = 1.0;
							conf.get(database::e_double::FX_AMOUNT, d);
							d *= 100.0;
							return Tools::sprintf_a("%.2lf", d);
						}
						return std::string("Disabled");
					},
						CHAR_INIT("%screen_chroma_fx%"), 			(text::e_custom_tags::T_CHROMA_FX)
				},
				{
					[&] {
						Database conf;
						int fpp;
						conf.get(database::e_integer::LIMIT_FPS, fpp);
						if (fpp > 0) return Tools::sprintf_a("%d", fpp);
						return std::string("Unlimited");
					},
						CHAR_INIT("%fps_cap%"), 					(text::e_custom_tags::T_FPS_CAP)
				}
			};
		}

		void Text::interpretTags(coloured_string& s)
		{
			size_t ref_p = 0;
#ifdef _DEBUG
			for (auto now_t = MILLI_NOW; MILLI_NOW - now_t < text::timeout_interpret_debugging;)
#else
			for (auto now_t = MILLI_NOW; MILLI_NOW - now_t < text::timeout_interpret;)
#endif
			{
				size_t in_p = (s.substr(ref_p).find('%'));

				if (in_p == std::string::npos || in_p + 1 >= s.size()) return; // cancel
				in_p += ref_p; // absolute position EXACTLY BEHIND % (s.substr(this) = %...

				if (in_p != 0 && s[in_p - 1].ch == '\\') {
					ref_p = in_p + 1;
					continue;
				}

				size_t en_l = (s.substr(in_p + 1).find('%'));

				if (en_l == std::string::npos || en_l >= s.size()) return; // cancel
				en_l += 2; // relative position (length) to in_p (including both %%)

				if (en_l != 0 && s[en_l - 2].ch == '\\') {
					ref_p = in_p + 1;
					continue;
				}

				std::string the_substr = s.substr(in_p, en_l).s_str();
								
				if (auto res = data_text->custom_tags(the_substr.c_str(), the_substr.length()); res) {
					std::string sec_res = (*res)();
					s = s.substr(0, in_p) + sec_res + s.substr(en_l + in_p);
					ref_p = in_p + 1;
				}
			}

			throw Abort::Abort(__FUNCSIG__, "Took too long interpreting '" + s.s_str() + "'! [Stopped at '" + s.s_str() + "']", Abort::abort_level::GIVEUP);
			return;

			//s = fina;
		}

		ALLEGRO_COLOR Text::hex(const int hx)
		{
			switch (hx) {
			case 0x0:
				return al_map_rgb(0, 0, 0);
			case 0x1:
				return al_map_rgb(0, 0, 170);
			case 0x2:
				return al_map_rgb(0, 170, 0);
			case 0x3:
				return al_map_rgb(0, 170, 170);
			case 0x4:
				return al_map_rgb(170, 0, 0);
			case 0x5:
				return al_map_rgb(170, 0, 170);
			case 0x6:
				return al_map_rgb(170, 170, 0);
			case 0x7:
				return al_map_rgb(170, 170, 170);
			case 0x8:
				return al_map_rgb(84, 84, 84);
			case 0x9:
				return al_map_rgb(84, 84, 255);
			case 0xA:
				return al_map_rgb(84, 255, 84);
			case 0xB:
				return al_map_rgb(84, 255, 255);
			case 0xC:
				return al_map_rgb(255, 84, 84);
			case 0xD:
				return al_map_rgb(255, 84, 255);
			case 0xE:
				return al_map_rgb(255, 255, 84);
			default:
				return al_map_rgb(255, 255, 255);
			}
		}


		void Text::draw_self()
		{
			if (!fontt) {
#ifdef _DEBUG
				throw Abort::Abort(__FUNCSIG__, "No font texture, but called draw anyway!", Abort::abort_level::GIVEUP);
#elif
				load(); // assume you have default texture set up (main)
#endif
			}

			double off_x = 0.0;
			double off_y = 0.0;
			bool use_following_color = (*data_text->boolean_data[text::e_boolean::USE_FOLLOWING_COLOR_INSTEAD])();

			// auto looks smaller, maybe good?
			auto& posx				= *getRef(sprite::e_double_readonly::POSX);
			auto& posy				= *getRef(sprite::e_double_readonly::POSY);
			const auto s_dist_x		= (*data_text->double_data[text::e_double::SHADOW_DISTANCE_X])();
			const auto s_dist_y		= (*data_text->double_data[text::e_double::SHADOW_DISTANCE_Y])();
			const auto s_col		= (*data_text->color_data[text::e_color::SHADOW_COLOR])();
			auto n_col				= (*getRef(sprite::e_color::COLOR))();
			auto& p_str				= *data_text->string_readonly_data[text::e_string_readonly::PROCESSED_STRING];
			const auto mode			= (*data_text->integer_data[text::e_integer::STRING_MODE])();
			const auto scale_g		= (*getRef(sprite::e_double::SCALE_G))();
			const auto scale_x		= (*getRef(sprite::e_double::SCALE_X))();
			const auto scale_y		= (*getRef(sprite::e_double::SCALE_Y))();
			auto& delta_t			= *data_text->chronomillis_readonly_data[text::e_chronomillis_readonly::LAST_UPDATE_STRING];
			const auto per_char		= (*data_text->boolean_data[text::e_boolean::USE_PER_CHAR_COLORING])();

			double t_rotation_rad = *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			double p_rotation_rad = 0.0;

			// STRING
			if (std::chrono::system_clock::now().time_since_epoch() > delta_t) // do update string
			{
				delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + text::default_delta_t_text_update_delay);

				p_str = (*data_text->string_data[text::e_cstring::STRING])();
				if (!(*data_text->boolean_data[text::e_boolean::NO_AUTO_STRING_INTERPRETATION])()) interpretTags(p_str);
			}



			if (std::shared_ptr<Sprite_Base> fl = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock(); fl)
			{
				fl->get(sprite::e_double_readonly::POSX, off_x);
				fl->get(sprite::e_double_readonly::POSY, off_y);
				fl->get(sprite::e_double_readonly::ROTATION, p_rotation_rad);
				p_rotation_rad *= ALLEGRO_PI / 180.0;

				if (use_following_color) {
					fl->get(sprite::e_color::COLOR, n_col);
				}
			}

			SuperResource<Camera> cameras;
			std::shared_ptr<Camera> ruler;
			if (ruler = cameras.getMain(); !ruler) throw Abort::Abort(__FUNCSIG__, "NO MAIN CAMERA HAS BEEN SET UP! Please set up a Camera! (Using SuperResource.setMain())");
			Camera preset;

			//double camx, camy, camg;
			if (*getRef(sprite::e_boolean::AFFECTED_BY_CAM)) preset = *ruler; // copy

			double rotation_rad = t_rotation_rad + p_rotation_rad;

			bool should_care_about_shadow = (s_dist_x != 0.0 || s_dist_y != 0.0);

			double pos_now[2];

			pos_now[0] = 1.0 * text::default_sharpness_font * (((posx) * cos(p_rotation_rad)) - ((posy) * sin(p_rotation_rad)) + off_x); // transformed to sprite's coords
			pos_now[1] = 1.0 * text::default_sharpness_font * (((posy) * cos(p_rotation_rad)) + ((posx) * sin(p_rotation_rad)) + off_y); // transformed to sprite's coords

			double pos_shadow_now[2] = { 0,0 };

			if (should_care_about_shadow) {
				pos_shadow_now[0] = 1.0 * text::default_sharpness_font * (((posx + s_dist_x) * cos(p_rotation_rad)) - ((posy + s_dist_y) * sin(p_rotation_rad)) + off_x);
				pos_shadow_now[1] = 1.0 * text::default_sharpness_font * (((posy + s_dist_y) * cos(p_rotation_rad)) + ((posx + s_dist_x) * sin(p_rotation_rad)) + off_y);
			}

			// not used anymore because of change there on preset.set OFFSET (this moves center to point and then rotates exactly there)

			/*double targ_draw_xy[2];
			targ_draw_xy[0] = pos_now[0];// *cos(rotation_rad) - pos_now[1] * sin(rotation_rad);
			targ_draw_xy[1] = pos_now[1];// *cos(rotation_rad) + pos_now[0] * sin(rotation_rad);
			targ_draw_xy[0] /= scale_x;
			targ_draw_xy[1] /= scale_y;


			double targ_draw_xy_shadow[2] = { 0,0 };

			if (should_care_about_shadow) {
				targ_draw_xy_shadow[0] = pos_shadow_now[0] * cos(rotation_rad) - pos_shadow_now[1] * sin(rotation_rad);
				targ_draw_xy_shadow[1] = pos_shadow_now[1] * cos(rotation_rad) + pos_shadow_now[0] * sin(rotation_rad);
				targ_draw_xy_shadow[0] /= scale_x;
				targ_draw_xy_shadow[1] /= scale_y;
			}*/

			preset.set(camera::e_double::SCALE_G,  *preset.getRef(camera::e_double::SCALE_G)  * scale_g * 1.0 / text::default_sharpness_font);
			preset.set(camera::e_double::SCALE_X,  *preset.getRef(camera::e_double::SCALE_X)  * scale_x);
			preset.set(camera::e_double::SCALE_Y,  *preset.getRef(camera::e_double::SCALE_Y)  * scale_y);
			preset.set(camera::e_double::OFFSET_X, (*preset.getRef(camera::e_double::OFFSET_X) * text::default_sharpness_font / scale_g) - (pos_now[0] / (scale_x * scale_g))); // offset of current cam + move center to its pos
			preset.set(camera::e_double::OFFSET_Y, (*preset.getRef(camera::e_double::OFFSET_Y) * text::default_sharpness_font / scale_g) - (pos_now[1] / (scale_y * scale_g))); // offset of current cam + move center to its pos

			preset.set(camera::e_double::ROTATION_RAD, *preset.getRef(camera::e_double::ROTATION_RAD) + rotation_rad); // now it rotates based on center
			preset.refresh();
			preset.apply();

			if (s_dist_x != 0.0 || s_dist_y != 0.0) {
				al_draw_text(&(*fontt), s_col, 0.0, 0.0, mode, p_str.s_str().c_str());
			}
			if (per_char) {
				//char minibuf[2] = { 0 };
				std::string thebuff;
				int offset_x_f = 0;
				char_c* data_ = p_str.data();

				for (size_t p = 0; p < p_str.size(); p++) {
					
					auto clr_now = hex(static_cast<int>(data_->cr));

					for (auto _ref = data_->cr; _ref == data_->cr && p < p_str.size();) {
						thebuff += data_->ch;
						data_++;
						p++;
					}

					al_draw_text(&(*fontt), clr_now,
						offset_x_f/* * cos(t_rotation_rad + p_rotation_rad)*/,
						/*- offset_x_f * sin(t_rotation_rad + p_rotation_rad)*/ 0.0,
						mode, thebuff.c_str());

					offset_x_f += al_get_text_width(&(*fontt), thebuff.c_str());
					thebuff.clear();
				}
			}
			else al_draw_text(&(*fontt), n_col, 0.0, 0.0, mode, p_str.s_str().c_str());
			
			if (isEq(sprite::e_boolean::SHOWDOT, true)) {
				al_draw_filled_circle(
					/* X1: */ 0.0,
					/* Y1: */ 0.0,
					/* SCL */ 2.0f * text::default_sharpness_font,
					al_map_rgba(0, 45, 90, 90));
			}

			ruler->apply();
		}

		Text::Text() : Sprite_Base()
		{
			custom_draw_task = [&] {draw_self(); };
			__link_hard_task();
		}

		Text::Text(Text& o) : Sprite_Base(o)
		{
			custom_draw_task = [&] {draw_self(); };
			__link_hard_task();
		}

		void Text::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_FONT> bmps;

			/*font.ref = bmps.load(id, str);
			font.source = str;
			font.id = id;*/
			fontt = bmps.load(id, str);

			if (!fontt) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");

		}

		void Text::load()
		{
			SuperResource<ALLEGRO_FONT> fonts;
			fontt = fonts.getMain();
			if (!fontt) throw Abort::Abort(__FUNCSIG__, "There's no MAIN FONT set! (See SuperResource.setMain(...))");
		}

		void Text::remove()
		{
			/*font.ref.reset();
			font.source.clear();
			font.id.clear();*/
			fontt.reset();
		}

		void Text::twinUpAttributes(const std::shared_ptr<text_data> oth)
		{
			data_text = oth;
		}

		std::shared_ptr<Text::text_data> Text::getAttributes()
		{
			return data_text;
		}

		void Text::addNewEntry(const std::string id, std::function<std::string(void)> f)
		{
			removeEntry(id);
			data_text->custom_tags.add({ f, CHAR_INIT(('%' + id + '%').c_str()) });
		}

		void Text::removeEntry(const std::string rid)
		{
			const std::string id = ('%' + rid + '%');
			for (size_t p = 0; p < data_text->custom_tags.size(); p++) {
				auto& u = (*(data_text->custom_tags.begin() + p));
				if (u(id.c_str(), id.length())) {
					if (u.hasType<text::e_custom_tags>()) {
						throw Abort::Abort(__FUNCSIG__, "User tried to replace/delete internal tag!", Abort::abort_level::FATAL_ERROR);
						return;
					}
					data_text->custom_tags.erase(p);
					break;
				}
			}
		}

		void Text::set(const text::e_cstring e, coloured_string v)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				*ptr = [=] {return v;};
		}

		void Text::set(const text::e_double e, double v)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_color e, ALLEGRO_COLOR v)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_integer e, int v)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_sprite_ptr e, std::shared_ptr<Sprite_Base> v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_boolean e, bool v)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const std::string e, coloured_string v)
		{
			auto* ptr = data_text->string_data(e.c_str(), e.length());
			if (!ptr) data_text->string_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, double v)
		{
			auto* ptr = data_text->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, ALLEGRO_COLOR v)
		{
			auto* ptr = data_text->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, int v)
		{
			auto* ptr = data_text->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, std::shared_ptr<Sprite_Base> v)
		{
			auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length());
			if (!ptr) data_text->sprite_ptr_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, bool v)
		{
			auto* ptr = data_text->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}



		void Text::set(const text::e_cstring e, std::function<coloured_string(void)> v)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_color e, std::function<ALLEGRO_COLOR(void)> v)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_integer e, std::function<int(void)> v)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_sprite_ptr e, std::function<std::shared_ptr<Sprite_Base>(void)> v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_boolean e, std::function<bool(void)> v)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const std::string e, std::function<coloured_string(void)> v)
		{
			auto* ptr = data_text->string_data(e.c_str(), e.length());
			if (!ptr) data_text->string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_text->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<ALLEGRO_COLOR(void)> v)
		{
			auto* ptr = data_text->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<int(void)> v)
		{
			auto* ptr = data_text->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<std::shared_ptr<Sprite_Base>(void)> v)
		{
			auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length());
			if (!ptr) data_text->sprite_ptr_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<bool(void)> v)
		{
			auto* ptr = data_text->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		bool Text::get(const text::e_string_readonly e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_cstring e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_double e, double& v)
		{
			if (auto* ptr = data_text->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_text->color_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_integer e, int& v)
		{
			if (auto* ptr = data_text->integer_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_sprite_ptr e, std::shared_ptr<Sprite_Base>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data[e]; ptr)
			{
				v = (*ptr)().lock();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_boolean e, bool& v)
		{
			if (auto* ptr = data_text->boolean_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, double& v)
		{
			if (auto* ptr = data_text->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_text->color_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, int& v)
		{
			if (auto* ptr = data_text->integer_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::shared_ptr<Sprite_Base>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)().lock();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, bool& v)
		{
			if (auto* ptr = data_text->boolean_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}



		bool Text::get(const text::e_cstring e, std::function<coloured_string(void)>& v)
		{
			if (auto* ptr = data_text->string_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_text->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_color e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_text->color_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_integer e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_text->integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_sprite_ptr e, std::function<std::weak_ptr<Sprite_Base>(void)>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_boolean e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_text->boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<coloured_string(void)>& v)
		{
			if (auto* ptr = data_text->string_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_text->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_text->color_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_text->integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<std::weak_ptr<Sprite_Base>(void)>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_text->boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}




		std::function<coloured_string(void)>* Text::getRef(const text::e_cstring e)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<double(void)>* Text::getRef(const text::e_double e)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<ALLEGRO_COLOR(void)>* Text::getRef(const text::e_color e)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<int(void)>* Text::getRef(const text::e_integer e)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<std::weak_ptr<Sprite_Base>(void)>* Text::getRef(const text::e_sprite_ptr e)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<bool(void)>* Text::getRef(const text::e_boolean e)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const coloured_string* Text::getRef(const text::e_string_readonly e) const
		{
			if (auto* ptr = data_text->string_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::chrono::milliseconds* Text::getRef(const text::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data_text->chronomillis_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		/*
		Button::Button()
		{
			Block::set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE)); // text collision is easier to do different than its size
			// find "itself" to make Text follow "itself"
			SuperResource<Sprite_Base> sprites;
			sprites.lock();
			for (auto& i : sprites) {
				if (&(*i.data()) == (void*)this) {
					Text::set(text::e_sprite_ptr::FOLLOWING, i.data());
				}
			}
			sprites.unlock();
		}

		Text* Button::getTextPart()
		{
			return reinterpret_cast<Text*>(Text::getThis());
		}

		Block* Button::getBlockPart()
		{
			return reinterpret_cast<Block*>(Block::getThis());
		}*/


		Button::Button()
		{
			SuperResource<Sprite_Base> sprites;
			std::string sign = Tools::generateRandomUniqueStringN();
			std::string my_id;

			std::shared_ptr<Sprite_Base> thus;

			sprites.lock();
			for (auto& i : sprites) {
				if (&(*i.data()) == (void*)this) {
					my_id = i.getID();
					thus = i.data();
					break;
				}
			}
			sprites.unlock();

			block = sprites.customLoad(my_id + "_b&" + sign + "+", [](Sprite_Base*& b) {return (b = new Block()); });
			text = sprites.customLoad(my_id + "_t&" + sign + "+", [](Sprite_Base*& b) {return (b = new Text()); });
			
			getText()->set(text::e_sprite_ptr::FOLLOWING, block);
			getText()->set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));

			reinterpret_cast<Sprite_Base*>(getBlock())->twinUpAttributes(reinterpret_cast<Sprite_Base*>(this)->getAttributes()); // b() attributes now are this attributes (spritewise)
		}

		Text* Button::getText()
		{
			return (Text*)&(*text);
		}

		Block* Button::getBlock()
		{
			return (Block*)&(*block);
		}


		void BubbleFX::createSwap(const int x, const int y)
		{
			ALLEGRO_BITMAP* trg = al_get_target_bitmap();
			SuperResource<ALLEGRO_BITMAP> bmps;

			auto nww = bmps.swapCustomLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			if (!nww) nww = bmps.customLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			al_set_target_bitmap(&(*nww));
			if (bmp.ref) al_draw_scaled_bitmap(&(*bmp.ref), 0, 0, al_get_bitmap_width(&(*bmp.ref)), al_get_bitmap_height(&(*bmp.ref)), 0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), 0);
			//else al_draw_filled_rectangle(0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), al_map_rgb(0, 255, 0));
			bmp.ref = nww;

			if (trg) al_set_target_bitmap(trg);
		}

		void BubbleFX::_checkInternalBMP()
		{
			ALLEGRO_BITMAP* trg = al_get_target_bitmap();

			const double _dd = 0.8;
			auto& delta_t = *data_bubblefx->chronomillis_readonly_data[bubblefx::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION];

			if (std::chrono::system_clock::now().time_since_epoch() > delta_t || !bmp.ref) {
				delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + bubblefx::default_delta_t_frame_delay);

				if (trg) {
					siz[0] = al_get_bitmap_width(trg) * _dd;
					siz[1] = al_get_bitmap_height(trg) * _dd;
				}

				if (siz[0] < 1280) siz[0] = 1280;
				if (siz[1] < 720)  siz[1] = 720;

				int _w = 0, _h = 0;
				if (bmp.ref) {
					_w = al_get_bitmap_width(&(*bmp.ref));
					_h = al_get_bitmap_height(&(*bmp.ref));
				}

				if (_w != siz[0] || _h != siz[1]) {
					createSwap(siz[0], siz[1]);
				}
			}
		}

		void BubbleFX::_checkUpdateBMP()
		{
			if (*data_bubblefx->boolean_readonly_data[bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN]) {
				*data_bubblefx->boolean_readonly_data[bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN] = false;

				ALLEGRO_BITMAP* trg = al_get_target_bitmap();

				if (bmp.ref) { // just to be sure
					al_set_target_bitmap(&(*bmp.ref));

					const float blur_intensity = static_cast<float>((*data_bubblefx->double_data[bubblefx::e_double::BLUR_INTENSITY])());
					size_t& p_assist = *data_bubblefx->sizet_readonly_data[bubblefx::e_sizet_readonly::VECTOR_POSITION_DRAWING];

					ALLEGRO_TRANSFORM clean;
					al_identity_transform(&clean);
					al_use_transform(&clean);


					for(size_t k = 0; k < bubblefx::default_bubbles_amount_draw_per_tick_max && (k + p_assist) < positions.size(); k++)
					{
						auto& i = positions[k + p_assist];
						al_draw_filled_circle(i.lastpositionscalculated[0], i.lastpositionscalculated[1], i.lastsize, al_map_rgba_f(
							static_cast<float>(Tools::maxone(al_get_time() * 0.3f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							static_cast<float>(Tools::maxone(al_get_time() * 0.1f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							static_cast<float>(Tools::maxone(al_get_time() * 0.8f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							+(1.0f - (blur_intensity))));
					}

					if (p_assist + bubblefx::default_bubbles_amount_draw_per_tick_max >= positions.size()) p_assist = 0;
					else p_assist += bubblefx::default_bubbles_amount_draw_per_tick_max;

					al_set_target_bitmap(trg);
				}
				else throw Abort::Abort(__FUNCSIG__, "Cannot draw BubbleFX because it couldn't get a valid bitmap somehow!", Abort::abort_level::GIVEUP);
			}
		}

		void BubbleFX::think()
		{
			const double delta = (*data_bubblefx->double_data[bubblefx::e_double::FRAMES_PER_SECOND])(); // delta t, 1/t = sec
			std::chrono::milliseconds& last_time = *data_bubblefx->chronomillis_readonly_data[bubblefx::e_chronomillis_readonly::LAST_FRAME];

			if (delta > 0.0) { // if delta <= 0, inf
				std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));

				if (MILLI_NOW - last_time > delta_tr) {
					last_time = MILLI_NOW;
				}
				else return; // no task
			}


			for (auto& i : positions)
			{
				i.lastsize = (18.0 + (Tools::rand() % 800) / 100.0) * 4.0 * sqrt(siz[0] * siz[1]) / 1440.0;
				i.posx = 1.1 - (Tools::rand() % 1200) / 500.0;
				i.posy = 1.1 - (Tools::rand() % 1200) / 500.0;

				/*i.lastpositionscalculated[0] = 0.0;
				i.lastpositionscalculated[1] = 0.0;*/

				i.lastpositionscalculated[0] = ((i.posx + 1.0) / 2.0) * siz[0];
				i.lastpositionscalculated[1] = ((i.posy + 1.0) / 2.0) * siz[1];
			}

			*data_bubblefx->boolean_readonly_data[bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN] = true;
		}

		void BubbleFX::draw_self()
		{
			// think internal bitmap size compared to reference
			_checkInternalBMP();

			// drawing itself (update itself points)
			_checkUpdateBMP();

			// same as Block
			ALLEGRO_BITMAP* rn = &(*bmp.ref);
			if (!rn) throw Abort::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


			float cx, cy, px, py, dsx, dsy, rot_rad;
			int bmpx, bmpy;
			bmpx = al_get_bitmap_width(rn);
			bmpy = al_get_bitmap_height(rn);
			if (bmpx <= 0 || bmpy <= 0) {
				throw Abort::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
			}

			cx = 1.0f * bmpx * (((*getRef(sprite::e_double::CENTER_X))() + 1.0) * 0.5);
			cy = 1.0f * bmpy * (((*getRef(sprite::e_double::CENTER_Y))() + 1.0) * 0.5);
			rot_rad = 1.0f * *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			/*px = 1.0f * data.dval[+Constants::io__sprite_double::POSX] * cos(rot_rad) + data.dval[+Constants::io__sprite_double::POSY] * sin(rot_rad);
			py = 1.0f * data.dval[+Constants::io__sprite_double::POSY] * cos(rot_rad) - data.dval[+Constants::io__sprite_double::POSX] * sin(rot_rad);*/
			px = *getRef(sprite::e_double_readonly::POSX);
			py = *getRef(sprite::e_double_readonly::POSY);
			dsx = 1.0f * (*getRef(sprite::e_double::SCALE_X))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpx);
			dsy = 1.0f * (*getRef(sprite::e_double::SCALE_Y))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpy);


			if ((*getRef(sprite::e_boolean::USE_COLOR))()) {
				al_draw_tinted_scaled_rotated_bitmap(rn, (*getRef(sprite::e_color::COLOR))(), cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}
			else {
				al_draw_scaled_rotated_bitmap(rn, cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}
		}

		BubbleFX::BubbleFX(const size_t amount, const double fps) : Sprite_Base()
		{
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);
			set(bubblefx::e_double::FRAMES_PER_SECOND, fps);

			bmp.id = "BUBBLE_UNIQUE_" + Tools::generateRandomUniqueStringN();

			createSwap(1280, 720);

			//custom_think_task = [&] {think(); };
			custom_draw_task = [&] {think();  draw_self(); };

			reset(amount);
		}

		BubbleFX::BubbleFX(BubbleFX& o) : Sprite_Base(o)
		{
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);

			bmp.id = "BUBBLE_UNIQUE_" + Tools::generateRandomUniqueStringN();

			//custom_think_task = [&] {think(); };
			custom_draw_task = [&] {think(); draw_self(); };
		}

		void BubbleFX::reset(const size_t amount)
		{
			if (!amount) throw Abort::Abort(__FUNCSIG__, "Bubble amount shall never be ZERO!", Abort::abort_level::FATAL_ERROR);

			if (positions.size() > 0) positions.clear();

			for (unsigned c = 0; c < amount; c++)
			{
				particle poss;

				poss.posx = 1.0 - (Tools::rand() % 1000) / 500.0;
				poss.posy = 1.0 - (Tools::rand() % 1000) / 500.0;
				poss.lastsize = (12.0 + (Tools::rand() % 500) / 100.0) * 3.0 * sqrt(siz[0] * siz[1]) / 1440.0;

				positions.push_back(poss);
			}
		}

		void BubbleFX::set(const bubblefx::e_double e, double v)
		{
			if (auto* ptr = data_bubblefx->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void BubbleFX::set(const bubblefx::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_bubblefx->double_data(e); ptr)
				*ptr = v;
		}

		void BubbleFX::set(const std::string e, double v)
		{
			auto* ptr = data_bubblefx->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void BubbleFX::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_bubblefx->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		bool BubbleFX::get(const bubblefx::e_double e, double& v)
		{
			if (auto* ptr = data_bubblefx->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool BubbleFX::get(const bubblefx::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_bubblefx->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool BubbleFX::get(const std::string e, double& v)
		{
			if (auto* ptr = data_bubblefx->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool BubbleFX::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_bubblefx->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool BubbleFX::get(const bubblefx::e_chronomillis_readonly e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data_bubblefx->chronomillis_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool BubbleFX::get(const bubblefx::e_boolean_readonly e, bool& v)
		{
			if (auto* ptr = data_bubblefx->boolean_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool BubbleFX::get(const bubblefx::e_sizet_readonly e, size_t& v)
		{
			if (auto* ptr = data_bubblefx->sizet_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		std::function<double(void)>* BubbleFX::getRef(const bubblefx::e_double e)
		{
			if (auto* ptr = data_bubblefx->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::chrono::milliseconds* BubbleFX::getRef(const bubblefx::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data_bubblefx->chronomillis_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const bool* BubbleFX::getRef(const bubblefx::e_boolean_readonly e) const
		{
			if (auto* ptr = data_bubblefx->boolean_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const size_t* BubbleFX::getRef(const bubblefx::e_sizet_readonly e) const
		{
			if (auto* ptr = data_bubblefx->sizet_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

	}
}