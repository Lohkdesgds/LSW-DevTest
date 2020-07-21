#include "block.h"

namespace LSW {
	namespace v5 {

		void Block::draw_self()
		{
			if (bitmaps.empty()) return;

			{
				const auto delta_t = getDirect<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION);

				if (const double _dd = getDirect<double>(block::e_double::TIE_SIZE_TO_DISPLAY_PROPORTION); _dd > 0.0 && (std::chrono::system_clock::now().time_since_epoch() > delta_t)) {
					set(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + block::default_delta_t_frame_delay));
					SuperResource<Bitmap> bmps;
					auto& trg = reference;
					if (trg && *trg) {
						int tx = trg->get_width() * _dd;
						int ty = trg->get_height() * _dd;

						for (auto& i : bitmaps) {
							int _w = i.ref->get_width();
							int _h = i.ref->get_height();
							if (_w == tx && _h == ty) continue; // no need to "reload"

							auto old = i.ref;
							i.ref = bmps.swapNew(i.id);
							i.ref->create(tx, ty);
							i.ref->set_as_target();
							if (old) old->draw(0, 0, tx, ty);
						}
						trg->set_as_target();
					}
				}
			}

			int frame = getDirect<int>(block::e_integer::FRAME);

			{
				const double delta = getDirect<double>(block::e_double::FRAMES_PER_SECOND); // delta t, 1/t = sec
				std::chrono::milliseconds last_time = getDirect<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_FRAME);

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

				set<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_FRAME, last_time);
			}

			if (!getDirect<bool>(block::e_boolean::SET_FRAME_VALUE_READONLY)) {
				set<int>(block::e_integer::FRAME, frame);
			}

			auto rnn = bitmaps[frame].ref;
			if (!rnn || !*rnn) throw Abort::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


			float cx, cy, px, py, dsx, dsy, rot_rad;
			int bmpx, bmpy;
			bmpx = rnn->get_width();
			bmpy = rnn->get_height();
			if (bmpx <= 0 || bmpy <= 0) {
				throw Abort::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
			}

			cx = 1.0f * bmpx * ((getDirect<double>(sprite::e_double::CENTER_X) + 1.0) * 0.5);
			cy = 1.0f * bmpy * ((getDirect<double>(sprite::e_double::CENTER_Y) + 1.0) * 0.5);
			rot_rad = 1.0f * getDirect<double>(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			px = getDirect<double>(sprite::e_double_readonly::POSX);
			py = getDirect<double>(sprite::e_double_readonly::POSY);
			dsx = 1.0f * (getDirect<double>(sprite::e_double::SCALE_X)) * (getDirect<double>(sprite::e_double::SCALE_G)) * (1.0 / bmpx);
			dsy = 1.0f * (getDirect<double>(sprite::e_double::SCALE_Y)) * (getDirect<double>(sprite::e_double::SCALE_G)) * (1.0 / bmpy);


			if (getDirect<bool>(sprite::e_boolean::USE_COLOR)) {
				rnn->draw(
					getDirect<ALLEGRO_COLOR>(sprite::e_color::COLOR),
					cx, cy,
					px, py,
					dsx, dsy,
					rot_rad);
			}
			else {
				rnn->draw(
					cx, cy,
					px, py,
					dsx, dsy,
					rot_rad);
			}

		}
		Block::Block() : Sprite_Base()
		{
			//if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);

			set<double>(block::e_double_defaults);
			set<bool>(block::e_boolean_defaults);
			set<int>(block::e_integer_defaults);
			set<std::chrono::milliseconds>(block::e_chronomillis_readonly_defaults);
		}
		Block::Block(Block& o) : Sprite_Base(o)
		{
			//if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);

			set<double>(block::e_double_defaults);
			set<bool>(block::e_boolean_defaults);
			set<int>(block::e_integer_defaults);
			set<std::chrono::milliseconds>(block::e_chronomillis_readonly_defaults);
		}
		void Block::twinUpAttributes(const std::shared_ptr<block_data> oth)
		{
			set<double>(oth->double_data);
			set<bool>(oth->boolean_data);
			set<std::string>(oth->string_data);
			set<int>(oth->integer_data);
			set<ALLEGRO_COLOR>(oth->color_data);
			set<uintptr_t>(oth->uintptrt_data);
			set<std::chrono::milliseconds>(oth->chronomillis_readonly_data);
		}
		std::shared_ptr<Block::block_data> Block::getAttributes()
		{
			std::shared_ptr<Block::block_data> oth = std::make_shared<Block::block_data>();

			oth->double_data = get<double>();
			oth->boolean_data = get<bool>();
			oth->string_data = get<std::string>();
			oth->integer_data = get<int>();
			oth->color_data = get<ALLEGRO_COLOR>();
			oth->uintptrt_data = get<uintptr_t>();
			oth->chronomillis_readonly_data = get<std::chrono::milliseconds>();
			return oth;
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
			SuperResource<Bitmap> bmps;
			_bitmap binfo;

			if (binfo.ref = bmps.load(id)) {
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
			SuperResource<Bitmap> bmps;
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
			SuperResource<Bitmap> bmps;
			if (std::shared_ptr<Bitmap> parent; bmps.get(parent_id, parent))
			{
				_bitmap binfo;

				binfo.source = parent_id;
				binfo.is_sub_bitmap = true;
				binfo.id = parent_id + "_cut&" + Tools::generateRandomUniqueStringN();
				binfo.ref = bmps.create(binfo.id);
				binfo.ref->create_sub(**parent, x, y, w, h);
				//binfo.ref = bmps.customLoad(binfo.id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(&(*parent), x, y, w, h); });

				if (binfo.ref && *binfo.ref)
				{
					bitmaps.push_back(binfo);
					return;
				}
			}
			throw Abort::Abort(__FUNCSIG__, "Cannot get sub part of '" + parent_id + "'!");
		}
		void Block::loadCut(const int x, const int y, const int w, const int h)
		{
			SuperResource<Bitmap> bmps;
			if (std::shared_ptr<Bitmap> parent = bmps.getMain())
			{
				_bitmap binfo;

				binfo.source.clear(); // main
				binfo.is_sub_bitmap = true;
				binfo.id = "_cut&" + Tools::generateRandomUniqueStringN();
				binfo.ref = bmps.create(binfo.id);
				binfo.ref->create_sub(**parent, x, y, w, h);
				//binfo.ref = bmps.customLoad(binfo.id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(&(*parent), x, y, w, h); });

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
	}
}