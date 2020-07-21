#include "bubblefx.h"

namespace LSW {
	namespace v5 {

		void BubbleFX::createSwap(const int x, const int y)
		{
			auto& trg = reference;
			SuperResource<Bitmap> bmps;

			auto old = bmp.ref;
			bmp.ref = bmps.swapNew(bmp.id);
			bmp.ref->create(x, y);
			bmp.ref->set_as_target();
			if (old) old->draw(0, 0, x, y);
			trg->set_as_target();

			///ALLEGRO_BITMAP* trg = al_get_target_bitmap();
			///SuperResource<ALLEGRO_BITMAP> bmps;

			///auto nww = bmps.swapCustomLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			///if (!nww) nww = bmps.customLoad(bmp.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(x, y); });
			///al_set_target_bitmap(&(*nww));
			///if (bmp.ref) al_draw_scaled_bitmap(&(*bmp.ref), 0, 0, al_get_bitmap_width(&(*bmp.ref)), al_get_bitmap_height(&(*bmp.ref)), 0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), 0);
			/////else al_draw_filled_rectangle(0, 0, al_get_bitmap_width(&(*nww)), al_get_bitmap_height(&(*nww)), al_map_rgb(0, 255, 0));
			///bmp.ref = nww;

			///if (trg) al_set_target_bitmap(trg);
		}

		void BubbleFX::_checkInternalBMP()
		{
			auto& trg = reference;

			const double _dd = 0.8;
			const auto delta_t = getDirect<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION);

			if (std::chrono::system_clock::now().time_since_epoch() > delta_t || !bmp.ref) {
				set<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + bubblefx::default_delta_t_frame_delay));

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

		void BubbleFX::_checkUpdateBMP()
		{
			if (getDirect<bool>(bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN)) {
				set<bool>(bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN, false);

				auto& trg = reference;

				if (bmp.ref) { // just to be sure
					bmp.ref->set_as_target();

					const float blur_intensity = static_cast<float>(getDirect<double>(bubblefx::e_double::BLUR_INTENSITY));
					const size_t p_assist = getDirect<size_t>(bubblefx::e_sizet_readonly::VECTOR_POSITION_DRAWING);

					ALLEGRO_TRANSFORM clean;
					al_identity_transform(&clean);
					al_use_transform(&clean);


					for (size_t k = 0; k < bubblefx::default_bubbles_amount_draw_per_tick_max && (k + p_assist) < positions.size(); k++)
					{
						auto& i = positions[k + p_assist];
						al_draw_filled_circle(i.lastpositionscalculated[0], i.lastpositionscalculated[1], i.lastsize, al_map_rgba_f(
							static_cast<float>(Tools::maxone(al_get_time() * 0.3f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							static_cast<float>(Tools::maxone(al_get_time() * 0.1f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							static_cast<float>(Tools::maxone(al_get_time() * 0.8f * ((Tools::rand() % 1000) / 1000.0f))) * static_cast<float>(1.0f - (blur_intensity)),
							+(1.0f - (blur_intensity))));
					}

					if (p_assist + bubblefx::default_bubbles_amount_draw_per_tick_max >= positions.size())
						set<size_t>(bubblefx::e_sizet_readonly::VECTOR_POSITION_DRAWING, 0ULL);
					else set<size_t>(bubblefx::e_sizet_readonly::VECTOR_POSITION_DRAWING, p_assist + bubblefx::default_bubbles_amount_draw_per_tick_max);

					trg->set_as_target();
				}
				else throw Abort::Abort(__FUNCSIG__, "Cannot draw BubbleFX because it couldn't get a valid bitmap somehow!", Abort::abort_level::GIVEUP);
			}
		}

		void BubbleFX::think()
		{
			const double delta = getDirect<double>(bubblefx::e_double::FRAMES_PER_SECOND); // delta t, 1/t = sec
			const std::chrono::milliseconds last_time = getDirect<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly::LAST_FRAME);

			if (delta > 0.0) { // if delta <= 0, inf
				std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));

				if (MILLI_NOW - last_time > delta_tr) {
					set<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly::LAST_FRAME, MILLI_NOW);
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

			set<bool>(bubblefx::e_boolean_readonly::SHOULD_UPDATE_SCREEN, true);
		}

		void BubbleFX::draw_self()
		{
			// think internal bitmap size compared to reference
			_checkInternalBMP();

			// drawing itself (update itself points)
			_checkUpdateBMP();

			// same as Block
			auto& rnn = bmp.ref;
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

		BubbleFX::BubbleFX(const size_t amount, const double fps) : Sprite_Base()
		{
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);
			set(bubblefx::e_double::FRAMES_PER_SECOND, fps);

			set<double>(bubblefx::e_double_defaults);
			set<bool>(bubblefx::e_boolean_readonly_defaults);
			set<size_t>(bubblefx::e_sizet_readonly_defaults); // UINTPTR_T
			set<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly_defaults);


			bmp.id = "BUBBLE_UNIQUE_" + Tools::generateRandomUniqueStringN();

			createSwap(1280, 720);

			//custom_think_task = [&] {think(); };
			custom_draw_task = [&] {think();  draw_self(); };

			reset(amount);

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
		}

		BubbleFX::BubbleFX(BubbleFX& o) : Sprite_Base(o)
		{
			set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));
			set(sprite::e_boolean::SET_TARG_POS_VALUE_READONLY, true);

			set<double>(bubblefx::e_double_defaults);
			set<bool>(bubblefx::e_boolean_readonly_defaults);
			set<size_t>(bubblefx::e_sizet_readonly_defaults); // UINTPTR_T
			set<std::chrono::milliseconds>(bubblefx::e_chronomillis_readonly_defaults);

			bmp.id = "BUBBLE_UNIQUE_" + Tools::generateRandomUniqueStringN();

			//custom_think_task = [&] {think(); };
			custom_draw_task = [&] {think(); draw_self(); };

			reference = std::make_shared<Bitmap>();
			reference->be_reference_to_target(true);
		}

		void BubbleFX::twinUpAttributes(const std::shared_ptr<bubblefx_data> oth)
		{
			set<double>(oth->double_data);
			set<bool>(oth->boolean_data);
			set<std::string>(oth->string_data);
			set<int>(oth->integer_data);
			set<ALLEGRO_COLOR>(oth->color_data);
			set<uintptr_t>(oth->uintptrt_data);
			set<std::chrono::milliseconds>(oth->chronomillis_readonly_data);
		}

		std::shared_ptr<BubbleFX::bubblefx_data> BubbleFX::getAttributes()
		{
			std::shared_ptr<BubbleFX::bubblefx_data> oth = std::make_shared<BubbleFX::bubblefx_data>();

			oth->double_data = get<double>();
			oth->boolean_data = get<bool>();
			oth->string_data = get<std::string>();
			oth->integer_data = get<int>();
			oth->color_data = get<ALLEGRO_COLOR>();
			oth->uintptrt_data = get<uintptr_t>();
			oth->chronomillis_readonly_data = get<std::chrono::milliseconds>();
			return oth;
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
	}
}