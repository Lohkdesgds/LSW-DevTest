#include "block.h"

namespace LSW {
	namespace v5 {

		void Block::draw_self()
		{
			if (bitmaps.empty()) return;

			{
				auto& delta_t = *data_block->chronomillis_readonly_data[block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION];

				if (const double _dd = (*data_block->double_data[block::e_double::TIE_SIZE_TO_DISPLAY_PROPORTION])(); _dd > 0.0 && (std::chrono::system_clock::now().time_since_epoch() > delta_t)) {
					delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + block::default_delta_t_frame_delay);
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
				*data_block->integer_data[block::e_integer::FRAME] = [=] {return frame; };
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

			cx = 1.0f * bmpx * (((*getRef(sprite::e_double::CENTER_X))() + 1.0) * 0.5);
			cy = 1.0f * bmpy * (((*getRef(sprite::e_double::CENTER_Y))() + 1.0) * 0.5);
			rot_rad = 1.0f * *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			px = *getRef(sprite::e_double_readonly::POSX);
			py = *getRef(sprite::e_double_readonly::POSY);
			dsx = 1.0f * (*getRef(sprite::e_double::SCALE_X))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpx);
			dsy = 1.0f * (*getRef(sprite::e_double::SCALE_Y))() * (*getRef(sprite::e_double::SCALE_G))() * (1.0 / bmpy);


			if ((*getRef(sprite::e_boolean::USE_COLOR))()) {
				rnn->draw(
					(*getRef(sprite::e_color::COLOR))(),
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
			if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
		}
		Block::Block(Block& o) : Sprite_Base(o)
		{
			if (!data_block) throw Abort::Abort(__FUNCSIG__, "Failed to create Block's data!");
			custom_draw_task = [&] {draw_self(); };

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
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
	}
}