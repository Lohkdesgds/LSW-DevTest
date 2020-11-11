#include "display.h"

namespace LSW {
	namespace v5 {
		namespace Interface {
			std::shared_ptr<Bitmap> Display::get_buffer_ref()
			{
				if (!disp) return nullptr;
				auto b = std::make_shared<Bitmap>();
				b->force(al_get_backbuffer(disp.get()));
				return b;
			}
			bool Display::thread_run(Tools::boolThreadF keep)
			{
				bool verynice = true;
				bool redraw = false;
				bool regen_disp_pls = false;

				bool was_vsync = should_vsync;
				size_t frames = 0;

				EventTimer timar(1.0);
				EventTimer fpsc(1.0);

				EventHandler handler;
				handler.add(timar);
				handler.add(fpsc);

				timar.start();
				if (fps_cap) {
					fpsc.set_delta(1.0 / fps_cap);
					fpsc.start();
				}

				// always timer event lmao, refreshes once per sec
				handler.set_run_autostart([&](const RawEvent& raw) {

					if (raw.timer_event().source == timar) {
						frames_per_second = frames;
						frames = 0;

						// check vsync
						if (was_vsync != should_vsync) {
							was_vsync = should_vsync;
							if (force_vsync_refresh) regen_disp_pls = true;
						}

						// check fps cap
						if (fps_cap) {
							if (fps_cap > display::max_fps_cap) fps_cap = display::max_fps_cap;

							if (fpsc.get_delta() != 1.0 / fps_cap) {
								fpsc.set_delta(1.0 / fps_cap);
							}
							if (!fpsc.running()) {
								fpsc.start();
							}
						}
						else {
							if (fpsc.running()) fpsc.stop();
							redraw = true; // just in case
						}
					}
					else if (raw.timer_event().source == fpsc) {
						redraw = true;
					}

				});

				try {
					thread_init();

					auto cam_cpy = camera_f();

					while (keep()) {

						if (fps_cap) {
							while (!redraw && fps_cap) std::this_thread::sleep_for(std::chrono::milliseconds(1)); // up to 500 fps good cap, 1000 limit
							redraw = false;
						}
						if (regen_disp_pls) {
							regen_disp_pls = false;
							_reset_display_and_path();
							refresh_camera = true;
						}

						frames++;

						if (refresh_camera) {
							set_internal_display_as_bitmap_reference();
							std::lock_guard<std::mutex> cam_m_luck(cam_m);
							cam_cpy = camera_f();
							if (refresh_camera && cam_cpy) {
								if (refresh_camera) refresh_camera = !cam_cpy->classic_update(*get_buffer_ref());
								//refresh_camera = false;
								cam_cpy->classic_refresh();

								Bitmap::check_bitmaps_memory();
							}
						}

						if (once_tasks.size()) {
							Tools::AutoLock luck(once_tasks_m);
							for (auto& i : once_tasks) i.work(); //i.set_value(10);
							once_tasks.clear();
						}

						al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));

						/*draw_tasks_m.lock();
						for (auto& i : draw_tasks) i.second();
						draw_tasks_m.unlock();*/

						// has to have cam
						if (cam_cpy.get()) {
							cam_cpy->apply();
							try {
								for (size_t p = 0; p < draw_tasks.size(); p++) draw_tasks.at(p).second(*cam_cpy);
							}
#ifdef _DEBUG
							catch (const std::out_of_range& e) { // I know sometimes it WILL be out of range if you change the draw_tasks size, but mutex costs A LOT (from 5000+ fps to ~300 fps), so just ignore out of range...
								std::cout << "__INTERNAL__ __SKIP__ OUT OF RANGE ERROR AT DRAW_TASKS: " << e.what() << std::endl;
#else
							catch (const std::out_of_range&) {
#endif
								fails_out_of_range++;
							}
							catch (Handling::Abort a) {
#ifdef _DEBUG
								std::cout << "__INTERNAL__ __SKIP__ ERROR AT DRAW_TASKS: " << a.what() << std::endl;
#endif
								fails_unexpected++;
							}
							catch (...) { // probably can skip (later: add counter by time)
#ifdef _DEBUG
								std::cout << "__INTERNAL__ __SKIP__ UNKNOWN ERROR AT DRAW_TASKS" << std::endl;
#endif
								fails_unexpected++;
							}
						}

						if (should_vsync) al_wait_for_vsync();
						al_flip_display();
					}
				}
				catch (...) { // enhance later
					verynice = false;
				}

				if (once_tasks_m.is_locked()) once_tasks_m.unlock(); // this is my safer mutex no worries.

				thread_deinit();

				handler.stop();
				timar.stop();

				return verynice;
			}
			void Display::_reset_display_and_path()
			{
				//if (pathing.paths_set().size()) pathing.apply();

				al_set_new_display_flags(new_display_flags_apply);
				if (new_display_refresh_rate > 0) al_set_new_display_refresh_rate(new_display_refresh_rate);
				al_set_new_display_option(ALLEGRO_VSYNC, should_vsync ? 1 : 2, ALLEGRO_SUGGEST); // vsync after via al_wait_for_vsync
				// prettier drawings (anti aliasing on primitives)
				al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, ALLEGRO_SUGGEST, 2);
				al_set_new_display_option(ALLEGRO_SAMPLES, ALLEGRO_SUGGEST, 8);
				// endof prettier drawings

				if (display_events.running()) {
					display_events.stop();
				}
				if (disp) {
					disp.reset();
				}

				disp = DisplayStrongPtr(al_create_display(new_resolution[0], new_resolution[1]), [](ALLEGRO_DISPLAY* d) { al_destroy_display(d); d = nullptr; });
				if (!disp.get()) {
					Logger logg;
					logg << L::SLF << fsr(E::ERRR) << "Cannot create display with these settings/environment!" << L::ELF;
					debug("Cannot create display with these settings/environment!");
					logg.flush();
					std::terminate();
				}

				al_convert_bitmaps();

				al_apply_window_constraints(disp.get(), true);
				al_set_window_constraints(disp.get(), display::minimum_display_size[0], display::minimum_display_size[1], 0, 0); // minimum size 640,480, max not defined

				set_internal_display_as_bitmap_reference();
				al_inhibit_screensaver(true);


				if (hide_mouse_new) al_hide_mouse_cursor(disp.get());
				else				al_show_mouse_cursor(disp.get());


				display_events.add(Event(al_get_display_event_source(disp.get())));
				
				display_events.set_run_autostart([&](const RawEvent& ev) {

					if (ev.type() == ALLEGRO_EVENT_DISPLAY_RESIZE) {
						al_acknowledge_resize(disp.get());
						refresh_camera = true;
					}
				});
			}
			void Display::thread_init()
			{
				fails_out_of_range = fails_unexpected = 0;

				_reset_display_and_path();

				refresh_camera = true;
			}
			void Display::thread_deinit()
			{
				display_events.stop();
				if (disp) {
					disp.reset();
					disp = nullptr;
				}
			}
			Display::Display()
			{
				Handling::init_basic();
				Handling::init_graphics();
				Handling::init_font();
			}
			Display::~Display()
			{
				deinit();
			}
			void Display::set_internal_display_as_bitmap_reference()
			{
				std::weak_ptr<ALLEGRO_DISPLAY> a = disp; // won't lock display
				Bitmap::set_custom_reference([a]() -> ALLEGRO_BITMAP* {
					if (auto d = a.lock(); d.get())
						return al_get_backbuffer(d.get());
					return nullptr;
				});
			}
			Tools::Future<bool> Display::init()
			{
				thr.join();
				thr.set([&](Tools::boolThreadF f) {return thread_run(f); });
				return std::move(thr.start());
			}
			void Display::deinit()
			{
				thr.stop();
				thr.join();
			}
			void Display::set_stop()
			{
				thr.stop();
			}
			void Display::set_camera(std::shared_ptr<Camera> cam)
			{
				std::lock_guard<std::mutex> luck(cam_m);
				camera_f = cam;
				refresh_camera = true;
			}
			void Display::set_camera(std::function<std::shared_ptr<Camera>(void)> f)
			{
				std::lock_guard<std::mutex> luck(cam_m);
				camera_f = f;
				refresh_camera = true;
			}
			void Display::set_fps_cap(const size_t cap)
			{
				fps_cap = cap;
			}
			std::shared_ptr<Camera> Display::get_current_camera()
			{
				return camera_f();
			}
			bool Display::running() const
			{
				return disp.operator bool();
			}
			void Display::set_new_flags(const int flags)
			{
				new_display_flags_apply = flags;
				if (!disp) is_fullscreen = (new_display_flags_apply & ALLEGRO_FULLSCREEN || new_display_flags_apply & ALLEGRO_FULLSCREEN_WINDOW);
			}
			int Display::get_flags() const
			{
				return disp.get() ? al_get_display_flags(disp.get()) : new_display_flags_apply;
			}
			const int Display::get_width() const
			{
				return disp.get() ? al_get_display_width(disp.get()) : new_resolution[0];
			}
			const int Display::get_height() const
			{
				return disp.get() ? al_get_display_height(disp.get()) : new_resolution[1];
			}
			void Display::set_width(const int xx)
			{
				if (xx >= display::minimum_display_size[0]) {
					new_resolution[0] = xx;
					if (disp.get()) al_resize_display(disp.get(), new_resolution[0], get_height());
				}
			}
			void Display::set_height(const int yy)
			{
				if (yy >= display::minimum_display_size[1]) {
					new_resolution[1] = yy;
					if (disp.get()) al_resize_display(disp.get(), get_width(), new_resolution[1]);
				}
			}
			void Display::set_new_refresh_rate(const int hz)
			{
				new_display_refresh_rate = hz;
			}
			int Display::get_refresh_rate() const
			{
				return disp.get() ? al_get_display_refresh_rate(disp.get()) : new_display_refresh_rate;
			}
			void Display::set_vsync(const bool vs, const bool force)
			{
				should_vsync = vs;
				force_vsync_refresh = force;
			}
			bool Display::get_vsync() const
			{
				return should_vsync;
			}
			bool Display::toggle_fullscreen()
			{
				is_fullscreen = !is_fullscreen;
				if (disp.get()) al_toggle_display_flag(disp.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
				return is_fullscreen;
			}
			void Display::set_fullscreen(const bool fullscr)
			{
				is_fullscreen = fullscr;
				if (disp.get()) al_toggle_display_flag(disp.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
			}
			void Display::hide_mouse(const bool hid)
			{
				if (disp.get()) {
					if (hid) al_hide_mouse_cursor(disp.get());
					else al_show_mouse_cursor(disp.get());
				}
				hide_mouse_new = hid;
			}

			void Display::set_refresh_camera()
			{
				refresh_camera = true;
			}
			
			bool Display::display_ready() const
			{
				return disp.get();
			}
			size_t Display::get_fps_cap() const
			{
				return fps_cap;
			}
			Tools::Future<DisplayAnySPtr> Display::add_once_task(DisplayAnyFSPtr tsk)
			{
				Tools::Promise<DisplayAnySPtr> my_future(tsk);
				Tools::Future<DisplayAnySPtr> future = std::move(my_future.get_future());

				Tools::AutoLock luck(once_tasks_m);
				
				once_tasks.emplace_back(std::move(my_future));
				
				return std::move(future);
			}
			size_t Display::add_draw_task(DisplayTask tsk)
			{
				Tools::AutoLock luck(draw_tasks_m);

				draw_tasks.push_back({ ++draw_tasks_count, std::move(tsk) });
				size_t this_task = draw_tasks_count;

				return this_task;
			}
			DisplayTask Display::remove_draw_task(const size_t tskid)
			{
				DisplayTask task;
				Tools::AutoLock luck(draw_tasks_m);

				for (size_t p = 0; p < draw_tasks.size(); p++) {
					if (draw_tasks[p].first == tskid) {
						task = draw_tasks[p].second;
						draw_tasks.erase(draw_tasks.begin() + p);
						return task;
					}
				}
				return task;
			}
			Event Display::get_event_source() const
			{
				if (disp) {
					return Event(al_get_display_event_source(disp.get()));
				}
				return Event();
			}
			size_t Display::get_frames_per_second() const
			{
				return frames_per_second;
			}
			size_t Display::debug_errors_out_of_range_skip() const
			{
				return fails_out_of_range;
			}
			size_t Display::debug_errors_unexpected() const
			{
				return fails_unexpected;
			}
		}
	}
}