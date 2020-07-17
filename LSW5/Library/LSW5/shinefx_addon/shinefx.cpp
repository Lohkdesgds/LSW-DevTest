#include "shinefx.h"

namespace LSW {
	namespace v5 {
		void ShineFX::createSwap(const int x, const int y)
		{
			auto& trg = reference;
			SuperResource<Bitmap> bmps;

			auto old = bmp.ref;
			bmp.ref = bmps.swapNew(bmp.id);
			bmp.ref->create(x, y);
			bmp.ref->set_as_target();
			if (old) old->draw(0, 0, x, y);
			trg->set_as_target();

			///auto nww = bmps.swapCustomLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			///if (!nww) nww = bmps.customLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			///al_set_target_bitmap(&(*nww));
			///if (bmp.ref) al_draw_scaled_bitmap(&(*bmp.ref), 0, 0, al_get_bitmap_width(&(*bmp.ref)), al_get_bitmap_height(&(*bmp.ref)), 0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), 0);
			/////else al_draw_filled_rectangle(0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), al_map_rgb(0, 255, 0));
			///bmp.ref = nww;
			///if (trg) al_set_target_bitmap(trg);
		}

		void ShineFX::_checkInternalBMP()
		{ 
			auto& trg = reference;

			int siz[2] = { 0,0 };
			const double _dd = 0.8;
			auto& delta_t = *data_shinefx->chronomillis_readonly_data[shinefx::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION];

			if (std::chrono::system_clock::now().time_since_epoch() > delta_t || !bmp.ref) {
				delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + shinefx::default_delta_t_frame_delay);

				if (trg) {
					siz[0] = trg->get_width() * _dd;
					siz[1] = trg->get_height() * _dd;
				}

				if (siz[0] < 1280) siz[0] = 1280;
				if (siz[1] < 720)  siz[1] = 720;

				int _w = 0, _h = 0;
				if (bmp.ref) {
					_w = bmp.ref->get_width();
					_h = bmp.ref->get_height();
				}

				if (_w != siz[0] || _h != siz[1]) {
					createSwap(siz[0], siz[1]);
				}
			}
		}


		void ShineFX::draw_self()
		{
			_checkInternalBMP();

			const double delta = (*data_shinefx->double_data[shinefx::e_double::FRAMES_PER_SECOND_UPDATE])(); // delta t, 1/t = sec
			std::chrono::milliseconds& last_time = *data_shinefx->chronomillis_readonly_data[shinefx::e_chronomillis_readonly::LAST_DRAW];

			if (delta > 0.0) { // if delta <= 0, inf
				std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));

				if (MILLI_NOW - last_time > delta_tr) {
					last_time = MILLI_NOW;

					// update bitmap
					auto& trg = reference;
					const double delta_rad = (*data_shinefx->double_data[shinefx::e_double::EACH_SIZE_RAD])();
					const double time_now = al_get_time() * (*data_shinefx->double_data[shinefx::e_double::SPEED_ROTATION_T])();
					const ALLEGRO_COLOR foreground_color = (*data_shinefx->color_data[shinefx::e_color::FOREGROUND])();
					const ALLEGRO_COLOR background_color = (*data_shinefx->color_data[shinefx::e_color::BACKGROUND])();

					if (bmp.ref) { // just to be sure
						bmp.ref->set_as_target();
						Camera cam;
						cam.refresh(bmp.ref);
						cam.apply();

						al_clear_to_color(background_color);

						auto nowarn = [&](float a, float b, float c, float d, float e, ALLEGRO_COLOR f) {
							return ALLEGRO_VERTEX({ a, b, c, d, e, f });
						};

						for (float radd = 0; radd < ALLEGRO_PI * 2; radd += delta_rad) {
							ALLEGRO_VERTEX v[] = {
							  { nowarn((2.0 * cos(radd + time_now * 0.2)), (2.0 * sin(radd + time_now * 0.2)), 0, 0, 0, foreground_color)},   //top left
							  { nowarn((2.0 * cos(radd + (delta_rad * 0.5) + time_now * 0.2)), (2.0 * sin(radd + (delta_rad * 0.5) + time_now * 0.2)), 0, 0, 0, foreground_color)},   //top right
							  { nowarn(0.0, 0.0, 0, 0, 0, foreground_color)}//,							  //center
							  //{ corner_rightdn[0], corner_rightdn[1], 0, 0, 0, foreground_color}, //bottom left
							  //{ corner_rightup[0], corner_rightup[1], 0, 0, 0, foreground_color}  //bottom right
							};
							al_draw_prim(v, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
						}

						trg->set_as_target();
					}
					else throw Abort::Abort(__FUNCSIG__, "Cannot draw ShineFX because it couldn't get a valid bitmap somehow!", Abort::abort_level::GIVEUP);
				}
			}

			// draw it
			auto& rnn = bmp.ref;
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

		ShineFX::ShineFX()
		{
			custom_draw_task = [&] {draw_self(); };
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
		}

		ShineFX::ShineFX(ShineFX& o) : Sprite_Base(o)
		{
			custom_draw_task = [&] {draw_self(); };
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
		}

		void ShineFX::twinUpAttributes(const std::shared_ptr<shinefx_data> oth)
		{
			data_shinefx = oth;
		}

		std::shared_ptr<ShineFX::shinefx_data> ShineFX::getAttributes()
		{
			return data_shinefx;
		}

		void ShineFX::set(const shinefx::e_double e, double v)
		{
			if (auto* ptr = data_shinefx->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void ShineFX::set(const shinefx::e_color e, ALLEGRO_COLOR v)
		{
			if (auto* ptr = data_shinefx->color_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void ShineFX::set(const shinefx::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_shinefx->double_data(e); ptr)
				*ptr = v;
		}

		void ShineFX::set(const shinefx::e_color e, std::function<ALLEGRO_COLOR(void)> v)
		{
			if (auto* ptr = data_shinefx->color_data(e); ptr)
				*ptr = v;
		}

		void ShineFX::set(const std::string e, double v)
		{
			auto* ptr = data_shinefx->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void ShineFX::set(const std::string e, ALLEGRO_COLOR v)
		{
			auto* ptr = data_shinefx->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void ShineFX::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_shinefx->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void ShineFX::set(const std::string e, std::function<ALLEGRO_COLOR(void)> v)
		{
			auto* ptr = data_shinefx->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		bool ShineFX::get(const shinefx::e_double e, double& v)
		{
			if (auto* ptr = data_shinefx->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool ShineFX::get(const shinefx::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_shinefx->color_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool ShineFX::get(const shinefx::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_shinefx->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool ShineFX::get(const shinefx::e_color e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_shinefx->color_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool ShineFX::get(const std::string e, double& v)
		{
			if (auto* ptr = data_shinefx->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool ShineFX::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_shinefx->color_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool ShineFX::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_shinefx->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool ShineFX::get(const std::string e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_shinefx->color_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		std::function<double(void)>* ShineFX::getRef(const shinefx::e_double e)
		{
			if (auto* ptr = data_shinefx->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<ALLEGRO_COLOR(void)>* ShineFX::getRef(const shinefx::e_color e)
		{
			if (auto* ptr = data_shinefx->color_data(e); ptr)
				return ptr;
			return nullptr;
		}

	}
}