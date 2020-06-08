#include "display.h"


namespace LSW {
	namespace v5 {
		void Display::__init(const int flags)
		{
			__print("Initializing display...");

			// guaranteed allegro initialization
			lsw_al_init();

			// guaranteed list of possible configurations
			al_set_new_display_flags(flags);
			display_now_config.latest_display_flags = flags;
			fullscreen.display_modes.clear();

			auto isSame = [](ALLEGRO_DISPLAY_MODE& a, ALLEGRO_DISPLAY_MODE& b) { return (a.format == b.format && a.height == b.height && a.refresh_rate == b.refresh_rate && a.width == b.width); };
			auto hasAlready = [&](ALLEGRO_DISPLAY_MODE& a) {for (auto& i : fullscreen.display_modes) if (isSame(i, a)) return true; return false; };

			int num_modes = al_get_num_display_modes();
			for (int j = 0; j < num_modes; ++j) {
				ALLEGRO_DISPLAY_MODE admode;
				if (al_get_display_mode(j, &admode) == &admode) {
					if (!hasAlready(admode)) {
						fullscreen.display_modes.push_back(admode);
					}
				}
			}

			__print("Initialized successfully.");
		}

		void Display::__deinit()
		{
			__print("Deinitializing display...");

			// PRINTING THREAD, DISPLAY & BUFFER
			close();

			// MANAGER (of screen options) CLEANUP //

			fullscreen.display_modes.clear();
			fullscreen.last_display_mode = 0;

			__print("Deinitialized successfully.");
		}

		bool Display::__lastWasWindow()
		{
			return was_window;
		}

		void Display::__setacknowledge()
		{
			should_acknowledge = true;
			time_last_acknowledge = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		}

		void Display::__checkacknowledge()
		{
			if (should_acknowledge && std::chrono::system_clock::now().time_since_epoch() - time_last_acknowledge > display::acknowledge_timeout_time) {

				al_acknowledge_resize(display_itself);

				should_acknowledge = false;

				display_now_config.chosen_mode.width = al_get_display_width(display_itself);
				display_now_config.chosen_mode.height = al_get_display_height(display_itself);

				if (use_doublebuffer) {

					auto buffer_prop = (*data.double_data[display::e_double::RESOLUTION_BUFFER_PROPORTION])();

					ALLEGRO_BITMAP* newbmp = al_create_bitmap(al_get_display_width(display_itself) * buffer_prop, al_get_display_height(display_itself) * buffer_prop);
					al_set_target_bitmap(newbmp);

					if (display_buffer) {
						al_draw_scaled_bitmap(display_buffer, 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_bitmap_width(newbmp), al_get_bitmap_height(newbmp), 0);
						al_destroy_bitmap(display_buffer);
						__print("Double buffering has updated and is now " + std::string(newbmp ? "ENABLED SUCCESSFULLY" : "NOT ENABLED, FAILED"));
					}
					else {
						__print("Double buffering is now " + std::string(newbmp ? "ENABLED SUCCESSFULLY" : "NOT ENABLED, FAILED"));
					}
					display_buffer = newbmp;

				}
				else if (display_buffer) {
					al_destroy_bitmap(display_buffer);
					display_buffer = nullptr;

					__print("Double buffering has been disabled.");
				}
			}
		}

		void Display::__print(const std::string&& str)
		{
			if (function_events) function_events(str);
		}

		void Display::__printscreen()
		{
			if (!print_pending) return;

			print_pending = false;

			if (print_thread.existThread()) {
				print_thread.stop(); // the thread is not exactly based on while(bool), but it's nice to use this maybe to debug?

				//if (print_thread.result() == 0) return;

				print_thread.join();

				if (print_thread.result() != 1) {
					throw Abort::Abort(__FUNCSIG__, "Unexpected behaviour on printing thread! It hasn't stopped as expected! Got return value: &e" + std::to_string(print_thread.result()) + "&c!", Abort::abort_level::GIVEUP);
				}
			}
			print_thread.kill();

			auto u = al_get_target_bitmap();

			char temp[26];
			SYSTEMTIME t;
			GetLocalTime(&t);
			auto ss = sprintf_s(temp, "%02d_%02d_%02d_%02d_%02d_%02d_%04d]", t.wYear % 100, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, ((int)(al_get_time() * 1000) % 1000));
			if (ss == 0) {
				throw Abort::Abort(__FUNCSIG__, "Unexpected behaviour on sprintf_s: null string?", Abort::abort_level::GIVEUP);
			}

			std::string path;
			if (auto i = data.string_data[display::e_string::PRINT_PATH]; i) {
				path = (*i)(); // copy the newest value always; // copy the newest value always)
				Tools::handlePath(path);
			}

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

		Display::Display()
		{
			__init(display::display_default_flags);
		}

		Display::Display(const int flags)
		{
			__init(flags | display::display_minimum_flags);
		}

		Display::~Display()
		{
			__deinit();
		}

		void Display::reFlags(const int flags)
		{
			__init(flags | display::display_minimum_flags);
		}

		bool Display::launch(const size_t option)
		{
			close();
			was_window = false;

			fullscreen.last_display_mode = option;
			if (option < fullscreen.display_modes.size()) {
				display_now_config.chosen_mode = fullscreen.display_modes[option];
			}
			else if (option == static_cast<size_t>(-1)) {
				fullscreen.last_display_mode = getHighestConfiguration();
				if (fullscreen.last_display_mode == static_cast<size_t>(-1)) { // bad news
					throw Abort::Abort(__FUNCSIG__, "Display couldn't get a valid configuration to launch.");
					return false;
				}
				display_now_config.chosen_mode = fullscreen.display_modes[fullscreen.last_display_mode];
			}

			al_set_new_display_refresh_rate(static_cast<int>(display_now_config.chosen_mode.refresh_rate));
			// already did al_set_new_display_flags on __init();
			al_set_new_bitmap_flags(display::bitmap_default_mode); // forced
			al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

			if (display_itself = al_create_display(display_now_config.chosen_mode.width, display_now_config.chosen_mode.height)) {
				__print("Created display successfully.");
			}
			else {
				throw Abort::Abort(__FUNCSIG__, "Could not create display!");
				return false;
			}
			al_convert_bitmaps();

			return true;
		}

		bool Display::launchWindow(const int x, const int y, size_t refresh_rate)
		{
			close();
			was_window = true;

			size_t max_refresh_rate = getHighestMonitorRefreshRate();
			if (refresh_rate == 0) refresh_rate = max_refresh_rate;
			else if (!(refresh_rate > 0 && refresh_rate <= max_refresh_rate)) throw Abort::Abort(__FUNCSIG__, "Invalid refresh rate to launch as window.");

			display_now_config.chosen_mode.width = x;
			display_now_config.chosen_mode.height = y;
			display_now_config.chosen_mode.refresh_rate = static_cast<int>(refresh_rate > 1e9 ? 1e9 : refresh_rate); // 1e9 frames per second? ARE YOU INSANE? :haha:

			al_set_new_display_refresh_rate(static_cast<int>(display_now_config.chosen_mode.refresh_rate));
			// already did al_set_new_display_flags on __init();
			al_set_new_bitmap_flags(display::bitmap_default_mode); // forced
			al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

			if (display_itself = al_create_display(display_now_config.chosen_mode.width, display_now_config.chosen_mode.height)) {
				__print("Created display successfully.");
			}
			else {
				throw Abort::Abort(__FUNCSIG__, "Could not create display!");
				return false;
			}
			al_convert_bitmaps();

			return true;
		}

		bool Display::relaunch()
		{
			// close() is automatic
			if (__lastWasWindow()) return launchWindow(display_now_config.chosen_mode.width, display_now_config.chosen_mode.height, display_now_config.chosen_mode.refresh_rate);
			return launch(fullscreen.last_display_mode);
		}

		void Display::close()
		{
			// STOP PRINTING THREAD //

			if (print_thread.existThread()) {
				Waiter wait_trigger;
				std::thread quick_exit([&] {print_thread.join(); wait_trigger.signal_all(); }); // simple thread

				wait_trigger.wait_signal(1000 * display::end_timeout_max_time);

				print_thread.kill(); // won't crash if successful
				quick_exit.join(); // sync
			}

			is_printing_screen = false;

			// DISPLAY BUFFER //

			if (display_buffer) {
				al_destroy_bitmap(display_buffer);
				display_buffer = nullptr;
			}

			// DISPLAY ITSELF //

			if (display_itself) {
				al_destroy_display(display_itself);
				display_itself = nullptr;
			}
		}

		void Display::flip()
		{
			if (display_buffer) {
				al_set_target_backbuffer(display_itself);

				ALLEGRO_TRANSFORM defaul;
				al_identity_transform(&defaul);
				al_use_transform(&defaul);

				auto ref_fx_amount = (*data.double_data[display::e_double::FX_AMOUNT])();

				if (ref_fx_amount == 0.0) {
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_scaled_bitmap(display_buffer, 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_display_width(display_itself), al_get_display_height(display_itself), 0);
				}
				else {
					double conv_quick_fx = pow(ref_fx_amount, 2.5);
					double conv_quick_fx2 = pow(ref_fx_amount, 0.8);
					double offsets[3];
					double scl = 10.0 * sqrt(al_get_bitmap_width(display_buffer) * al_get_bitmap_height(display_buffer)) / sqrt(display::display_reference_size[0] * display::display_reference_size[1]);
					offsets[0] = cos(al_get_time() * 0.40) * scl * conv_quick_fx;
					offsets[1] = sin(al_get_time() * 0.32) * scl * conv_quick_fx;
					offsets[2] = cos(al_get_time() * 0.29 + 0.5) * scl * conv_quick_fx;

					al_draw_filled_rectangle(0, 0, al_get_display_width(display_itself), al_get_display_height(display_itself), al_map_rgba_f(0.0, 0.0, 0.0, 1.0 - 0.952 * conv_quick_fx2));

					al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);

					double qfbl = 1.0 - 0.95 * conv_quick_fx2;

					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(qfbl, 0.000, 0.000, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[0], offsets[1], al_get_display_width(display_itself) + offsets[1], al_get_display_height(display_itself) + offsets[2], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.000, qfbl, 0.000, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[1], offsets[2], al_get_display_width(display_itself) + offsets[2], al_get_display_height(display_itself) + offsets[0], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.000, 0.000, qfbl, qfbl), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), offsets[2], offsets[0], al_get_display_width(display_itself) + offsets[0], al_get_display_height(display_itself) + offsets[1], 0);
					al_draw_tinted_scaled_bitmap(display_buffer, al_map_rgba_f(0.015, 0.015, 0.015, 0.015), 0, 0, al_get_bitmap_width(display_buffer), al_get_bitmap_height(display_buffer), 0, 0, al_get_display_width(display_itself), al_get_display_height(display_itself), 0);

					al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
				}

				// COMING
				//quick_fx//
			}

			__printscreen(); // check and printscreen

			if (auto ret_fps_limit = (*data.integer_data[display::e_integer::LIMIT_FPS])(); ret_fps_limit > 1.0) {

				al_flip_display();

				while ((std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch() - time_last_draw).count() * 1.0/1e6) < 1.0 / ret_fps_limit) al_rest(0.1 / ret_fps_limit);
				time_last_draw = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
			}
			else {
				al_flip_display();
			}

			__checkacknowledge();

			if (display_buffer) al_set_target_bitmap(display_buffer);
			else al_set_target_backbuffer(display_itself);
		}

		void Display::printscreen()
		{
			print_pending = true;
		}

		void Display::refreshMemoryBitmaps()
		{
			al_convert_bitmaps();
		}

		void Display::acknowledgeDisplay()
		{
			__setacknowledge();
		}

		void Display::set(const display::e_double e, double v)
		{
			set(e, [=] {return v; });
		}

		void Display::set(const display::e_integer e, int v)
		{
			set(e, [=] {return v; });
		}

		void Display::set(const display::e_string e, std::string v)
		{
			set(e, [=] {return v; });
		}

		void Display::set(const display::e_boolean e, bool v)
		{
			set(e, [=] {return v; });
		}

		void Display::set(const display::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Display::set(const display::e_integer e, std::function<int(void)> v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}

		void Display::set(const display::e_string e, std::function<std::string(void)> v)
		{
			if (auto* ptr = data.string_data(e); ptr)
				*ptr = v;
		}

		void Display::set(const display::e_boolean e, std::function<bool(void)> v)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				*ptr = v;
		}

		void Display::set(const std::string e, double v)
		{
			set(e, std::function<double(void)>([=] {return v; }));
		}

		void Display::set(const std::string e, int v)
		{
			set(e, std::function<int(void)>([=] {return v; }));
		}

		void Display::set(const std::string e, std::string v)
		{
			set(e, std::function<std::string(void)>([=] {return v; }));
		}

		void Display::set(const std::string e, bool v)
		{
			set(e, std::function<bool(void)>([=] {return v; }));
		}

		void Display::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Display::set(const std::string e, std::function<int(void)> v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Display::set(const std::string e, std::function<std::string(void)> v)
		{
			auto* ptr = data.string_data(e.c_str(), e.length());
			if (!ptr) data.string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Display::set(const std::string e, std::function<bool(void)> v)
		{
			auto* ptr = data.boolean_data(e.c_str(), e.length());
			if (!ptr) data.boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Display::get(const display::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_string e, std::string& v)
		{
			if (auto* ptr = data.string_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_boolean e, bool& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_integer e, std::function<int(void)>& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_string e, std::function<std::string(void)>& v)
		{
			if (auto* ptr = data.string_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const display::e_boolean e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, std::string& v)
		{
			if (auto* ptr = data.string_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, bool& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, std::function<int(void)>& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, std::function<std::string(void)>& v)
		{
			if (auto* ptr = data.string_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Display::get(const std::string e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		std::function<double(void)>* Display::getRef(const display::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<int(void)>* Display::getRef(const display::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<std::string(void)>* Display::getRef(const display::e_string e)
		{
			if (auto* ptr = data.string_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<bool(void)>* Display::getRef(const display::e_boolean e)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::vector<ALLEGRO_DISPLAY_MODE>& Display::configurations() const
		{
			return fullscreen.display_modes;
		}

		const size_t Display::getNearestConfiguration(const int x, const int y, const size_t hz)
		{
			size_t nearest = static_cast<size_t>(-1);
			size_t hz_last = 0;

			/*
			How it gets the "nearest":
			- Matches resolution: 1 point
			- Has higher refresh_rate = 1 point
			- Also matches refresh rate: chosen one
			*/

			for (size_t opt = 0; opt < fullscreen.display_modes.size(); opt++) {
				auto& i = fullscreen.display_modes[opt];

				if (i.width == x && i.height == y) { // good
					if (i.refresh_rate == hz) { // chosen one
						return opt; // yes
					}

					// these two could be in the same if, but it's clearer to read if split

					// if none is exactly same refresh (or if latest attempt is higher than expected), it will search for the "almost there" or "at least has this resolution"
					if ((i.refresh_rate > hz_last || hz_last > hz) && i.refresh_rate < hz) { // refresh_rate will never be == hz here (if above)
						hz_last = i.refresh_rate; // lower, but nearer
						nearest = opt;
					}
					// has higher refresh rate than wanted, but found same resolution, so...
					else if (hz_last == 0) {
						hz_last = i.refresh_rate;
						nearest = opt;
					}
				}
			}
			return nearest;
		}

		const size_t Display::getHighestConfiguration()
		{
			size_t highest = static_cast<size_t>(-1);
			ALLEGRO_DISPLAY_MODE highest_mode{ 0 };

			for (size_t opt = 0; opt < fullscreen.display_modes.size(); opt++) {
				auto& i = fullscreen.display_modes[opt];
				double a, b;
				a = static_cast<double>(i.width) * static_cast<double>(i.height);
				b = static_cast<double>(highest_mode.width) * static_cast<double>(highest_mode.height);

				if (a > b) { // higher pixel count
					highest = opt;
					highest_mode = i;
				}
				if (a == b && i.refresh_rate >= highest_mode.refresh_rate) { // same pixel amount, higher refresh rate
					highest = opt;
					highest_mode = i;
				}
			}
			return highest;
		}

		const size_t Display::getHighestMonitorRefreshRate()
		{
			size_t refresh_rate = 0;
			for (auto& i : fullscreen.display_modes) refresh_rate = (i.refresh_rate > refresh_rate ? i.refresh_rate : refresh_rate);
			return refresh_rate ? refresh_rate : static_cast<size_t>(-1);
		}

		const Display::display_info Display::getLatest()
		{
			return display_now_config;
		}

		ALLEGRO_DISPLAY* Display::getRawDisplay()
		{
			return display_itself;
		}

		ALLEGRO_EVENT_SOURCE* Display::getEvent()
		{
			return al_get_display_event_source(display_itself);
		}











		/*
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
			ref_doublebuffer	= (db.getRef(database::e_boolean::DOUBLE_BUFFERING));

			ALLEGRO_DISPLAY_MODE dm{};

			db.get(database::e_integer::SCREEN_X, dm.width);
			db.get(database::e_integer::SCREEN_Y, dm.height);
			db.get(database::e_integer::SCREEN_PREF_HZ, dm.refresh_rate);
			db.get(database::e_integer::SCREEN_FLAGS, dm.format);

			dm.format |= display::display_minimum_flags;

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
				//quick_fx//
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

		*/
	}
}
