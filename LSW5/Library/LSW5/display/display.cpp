#include "display.h"


namespace LSW {
	namespace v5 {

		bool isDisplayModeEq(ALLEGRO_DISPLAY_MODE a, ALLEGRO_DISPLAY_MODE b) {
			return (a.format == b.format && a.height == b.format && a.refresh_rate == b.refresh_rate && a.width == b.width);
		}

		void Display::modes::loadOptions(const int mds)
		{
			lsw_al_init();

			al_set_new_display_flags(mds);
			lastmode = mds;
			options.clear();

			int num_modes = al_get_num_display_modes();
			for (int j = 0; j < num_modes; ++j) {
				ALLEGRO_DISPLAY_MODE admode;
				if (al_get_display_mode(j, &admode) == &admode) {
					
					bool push = true;
					for (auto& i : options) {
						//if (mod.x == i.x && mod.y == i.y && mod.hz == i.hz) {
						if (isDisplayModeEq(admode, i)) {
							push = false;
							break;
						}
					}
					if (push) {
						options.push_back(admode);
					}
				}
			}
		}

		bool Display::modes::checkExistance(const int x, const int y, const int ref)
		{
			ALLEGRO_DISPLAY_MODE sm = { x, y, 0, ref };
			bool alright = false;

			if (lastmode & ALLEGRO_WINDOWED) // doesn't need to check fixed window size
			{
				alright = true;
				bool once_had_refresh_rate = false;
				for (auto& i : options)
				{
					if (sm.width > i.width || sm.height > i.height) {
						alright = false;
						break;
					}
					if (i.refresh_rate == sm.refresh_rate) {
						once_had_refresh_rate = true;
					}
				}
				alright = alright && once_had_refresh_rate; // game refresh rate matches one refresh rate found
			}
			else {
				for (auto i : options)
				{
					i.format = sm.format; // doesn't need to check format right now lmao
					if (isDisplayModeEq(i, sm)) {
						alright = true;
						break;
					}
				}
			}

			return alright;
		}

		const auto& Display::modes::getAvailableResolutions()
		{
			return options;
		}


		void Display::_print()
		{
			print_thread.stop(); // the thread is not exactly based on while(bool), but it's nice to use this maybe to debug?

			if (print_thread.result() == 0) return;

			print_thread.join();

			if (print_thread.result() != 1) {
				Logger logg;
				logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&cUnexpected behaviour on printing thread! It hasn't stopped as expected! Got return value: &e" << print_thread.result() << "&c!" << L::ELF;
			}

			auto u = al_get_target_bitmap();

			char temp[26];
			SYSTEMTIME t;
			GetLocalTime(&t);
			auto ss = sprintf_s(temp, "%02d_%02d_%02d_%02d_%02d_%02d_%04d]", t.wYear % 100, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, ((int)(al_get_time() * 1000) % 1000));
			if (ss == 0) {
				throw Abort::Abort(__FUNCSIG__, "Unexpected behaviour on sprintf_s: null string?", Abort::abort_level::GIVEUP);
			}

			std::string path = *ref_print_path; // copy the newest value always
			Tools::handlePath(path);

			auto rt = time(NULL);
			tm tt;
			auto v = localtime_s(&tt, &rt);

			if (v != 0 || path.length() == 0) throw Abort::Abort(__FUNCSIG__, "Can't get/set a proper name to save the screenshot!", Abort::abort_level::WARN);

			path = path + temp + ".jpg";
			
			int savv = al_get_new_bitmap_flags();
			al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
			auto cpy = al_clone_bitmap(u);
			al_set_new_bitmap_flags(savv);

			print_thread.setFunc([=](Threads::boolThreadF wl)->int {
				int retval = 1;
				if (!al_save_bitmap(path.c_str(), u)) {
					retval = 2;
				}
				al_destroy_bitmap(u);
				return retval;
			});

			print_thread.start();
		}

		void Display::checkAcknowledge()
		{
			if (last_acknowledge != 0.0 && al_get_time() - last_acknowledge > display::display_fixed_acknowledge_delta_t) {
				Logger logg;

				al_acknowledge_resize(display);
				last_acknowledge = 0.0;

				if (ref_doublebuffer) {

					ALLEGRO_BITMAP* newbmp = al_create_bitmap(al_get_display_width(display) * *ref_buffer_prop, al_get_display_height(display) * *ref_buffer_prop);
					al_set_target_bitmap(newbmp);

					if (display_buffer) {
						al_draw_scaled_bitmap(display_buffer, 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_bitmap_width(newbmp), al_get_bitmap_height(newbmp), 0);
						al_destroy_bitmap(display_buffer);
						logg << L::SLF << fsr(__FUNCSIG__) << "Double buffering has updated and is now " << (newbmp ? "&aENABLED SUCCESSFULLY" : "&eNOT ENABLED, FAILED") << L::ELF;
					}
					else {
						logg << L::SLF << fsr(__FUNCSIG__) << "Double buffering is now " << (newbmp ? "&aENABLED SUCCESSFULLY" : "&eNOT ENABLED, FAILED") << L::ELF;
					}
					display_buffer = newbmp;

				}
				else if (display_buffer) {
					al_destroy_bitmap(display_buffer);
					display_buffer = nullptr;

					logg << L::SLF << fsr(__FUNCSIG__) << "Double buffering has been disabled." << L::ELF;
				}
			}
		}

		Display::Display()
		{
			lsw_al_init();
			print_thread.resetResultTo(0);
		}
		Display::~Display()
		{
			deinit();
		}
		void Display::init()
		{
			Database db;

			// initializing references later:
			ref_fx_amount		= (db.getRef(database::e_double::FX_AMOUNT));
			ref_buffer_prop		= (db.getRef(database::e_double::RESOLUTION_BUFFER_PROPORTION));
			ref_fps_limit		= (db.getRef(database::e_integer::LIMIT_FPS));
			ref_print_path		= (db.getRef(database::e_string::PRINT_PATH));
			ref_doublebuffer	= (db.getRef(database::e_boolean::DOUBLEBUFFERING));

			ALLEGRO_DISPLAY_MODE dm{};

			db.get(database::e_integer::SCREEN_X, dm.width);
			db.get(database::e_integer::SCREEN_Y, dm.height);
			db.get(database::e_integer::SCREEN_PREF_HZ, dm.refresh_rate);
			db.get(database::e_integer::SCREEN_FLAGS, dm.format);

			dm.format |= display::display_minimum_mode;

			db.set(database::e_integer::SCREEN_FLAGS, dm.format);

			if (display) deinit();

			display_modes.loadOptions(dm.format);

			auto u = display_modes.getAvailableResolutions();

			const auto f_higher = [](ALLEGRO_DISPLAY_MODE& a, ALLEGRO_DISPLAY_MODE& b)->bool {return ((a.width > b.width&& a.height >= b.height) || (a.width >= b.width && a.height > b.height)); }; // if a is "bigger" then b
			const auto f_eq_siz = [](ALLEGRO_DISPLAY_MODE& a, ALLEGRO_DISPLAY_MODE& b)->bool {return a.width == b.width && a.height == b.height; };
			const auto f_faster = [](ALLEGRO_DISPLAY_MODE& a, ALLEGRO_DISPLAY_MODE& b)->bool {return a.refresh_rate > b.refresh_rate; };

			if (dm.refresh_rate <= 0) {
				dm.refresh_rate = 0;
				for (auto& i : u) {
					if (f_higher(i, dm)) {
						dm.refresh_rate = i.refresh_rate;
						dm.width = i.width;
						dm.height = i.height;
					}
					if (f_eq_siz(i, dm) && f_faster(i, dm)) {
						dm.refresh_rate = i.refresh_rate;
					}
				}
			}

			if ((dm.format & ALLEGRO_WINDOWED) || display_modes.checkExistance(dm.width, dm.height, dm.refresh_rate)) {
				al_set_new_bitmap_flags(display::bitmap_default_mode);
				al_set_new_display_refresh_rate(dm.refresh_rate);
				al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

				if (display = al_create_display(dm.width, dm.height)) {
					Logger logg;
					logg << L::SLF << fsr(__FUNCSIG__) << "Created display successfully: " << dm.width << "x" << dm.height << "@" << (dm.refresh_rate == 0 ? "auto" : std::to_string(dm.refresh_rate)) << ";flags=" << dm.format << L::ELF;
				}
				else {
					throw Abort::Abort(__FUNCSIG__, "Could not create display!");
				}
				al_convert_bitmaps();

				{
					Database db;
					if (db.isEq(database::e_boolean::HIDEMOUSE, true)) al_hide_mouse_cursor(display);
				}
			}
			else {
				throw Abort::Abort(__FUNCSIG__, "Invalid resolution, sorry!");
			}


		}
		void Display::restart()
		{
			deinit();
			init();
		}
		void Display::deinit()
		{
			// unlink
			ref_fx_amount		= nullptr;
			ref_buffer_prop		= nullptr;
			ref_fps_limit		= nullptr;
			ref_print_path		= nullptr;
			ref_doublebuffer	= nullptr;

			if (display) {
				if (display_buffer) al_destroy_bitmap(display_buffer);
				al_destroy_display(display);

				display_buffer = nullptr;
				display = nullptr;

				if (print_thread.existThread()) {
					Abort::AutoAbort aa(5000, []() { Logger logg; logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4FATAL ERROR: COULD NOT END PROPERLY! FORCING GAME TO END after 5 seconds trying to end." << L::ELF; logg.flush(); });
					print_thread.join();
					aa.abort();
				}
				print_thread.kill();
			}
		}
		void Display::refreshBitmaps()
		{
			al_convert_bitmaps();
		}
		void Display::flip()
		{
			if (display_buffer) {
				al_set_target_backbuffer(display);

				ALLEGRO_TRANSFORM defaul;
				al_identity_transform(&defaul);
				al_use_transform(&defaul);

				if (*ref_fx_amount == 0.0) {
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_scaled_bitmap(display_buffer, 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);
				}
				else {
					double conv_quick_fx = pow(*ref_fx_amount, 2.5);
					double conv_quick_fx2 = pow(*ref_fx_amount, 0.8);
					double offsets[3];
					double scl = 10.0 * sqrt(al_get_bitmap_width(display_buffer) * al_get_bitmap_height(display_buffer)) / sqrt(display::display_reference_size[0] * display::display_reference_size[1]);
					offsets[0] = cos(al_get_time() * 0.40) * scl * conv_quick_fx;
					offsets[1] = sin(al_get_time() * 0.32) * scl * conv_quick_fx;
					offsets[2] = cos(al_get_time() * 0.29 + 0.5) * scl * conv_quick_fx;

					al_draw_filled_rectangle(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgba_f(0.0, 0.0, 0.0, 1.0 - 0.952 * conv_quick_fx2));

					al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);

					double qfbl = 1.0 - 0.95 * conv_quick_fx2;

					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(qfbl, 0.000, 0.000, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[0], offsets[1], al_get_display_width(display) + offsets[1], al_get_display_height(display) + offsets[2], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.000, qfbl, 0.000, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[1], offsets[2], al_get_display_width(display) + offsets[2], al_get_display_height(display) + offsets[0], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.000, 0.000, qfbl, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[2], offsets[0], al_get_display_width(display) + offsets[0], al_get_display_height(display) + offsets[1], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.015, 0.015, 0.015, 0.015), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);

					al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
				}

				// COMING
				/*quick_fx*/
			}

			if (print_pending) _print();

			if (*ref_fps_limit > 1.0) {
				if (last_draw_tick == 0.0) last_draw_tick = al_get_time() - 1.0 / *ref_fps_limit;

				al_flip_display();

				while (al_get_time() - last_draw_tick < 1.0 / *ref_fps_limit) al_rest(0.1 / *ref_fps_limit);
				last_draw_tick += 1.0 / *ref_fps_limit;
			}
			else {
				al_flip_display();
			}


			checkAcknowledge();

			if (display_buffer) al_set_target_bitmap(display_buffer);
			else al_set_target_backbuffer(display);
		}
		void Display::acknowledgeDisplay()
		{
			last_acknowledge = al_get_time() + 0.5;
		}
		void Display::print()
		{
			print_pending = true;
		}
		ALLEGRO_EVENT_SOURCE* Display::getEvent() const
		{
			return display ? al_get_display_event_source(display) : nullptr;
		}
		ALLEGRO_DISPLAY* Display::getRawDisplay() const
		{
			return display;
		}
	}
}
