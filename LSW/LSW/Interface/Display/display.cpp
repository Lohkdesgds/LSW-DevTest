#include "display.h"

namespace LSW {
	namespace v5 {
		namespace Interface {
			std::shared_ptr<Bitmap> Display::get_buffer_ref()
			{
				if (!display) return nullptr;
				auto b = std::make_shared<Bitmap>();
				b->force(al_get_backbuffer(display.get()));
				return b;
			}
			bool Display::thread_run(Tools::boolThreadF keep)
			{
				bool verynice = true;
				size_t frames = 0;

				std::shared_ptr<ALLEGRO_TIMER> timar = std::shared_ptr<ALLEGRO_TIMER>(al_create_timer(1.0), [](ALLEGRO_TIMER* t) {al_destroy_timer(t); t = nullptr; });
				EventHandler handler;
				handler.add(al_get_timer_event_source(timar.get()));
				al_start_timer(timar.get());

				// always timer event lmao
				handler.set_run_autostart([&](const RawEvent& raw) {
					frames_per_second = frames;
					frames = 0;
				});

				try {
					thread_init();

					auto cam_cpy = camera_f();

					while (keep()) {
						frames++;

						if (refresh_camera) {
							std::lock_guard<std::mutex> cam_m_luck(cam_m);
							cam_cpy = camera_f();
							if (refresh_camera && cam_cpy) {
								if (refresh_camera) refresh_camera = !cam_cpy->classic_update(get_buffer_ref());
								//refresh_camera = false;
								cam_cpy->classic_refresh();
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
							catch (...) { // probably can skip (later: add counter by time)
#ifdef _DEBUG
								std::cout << "__INTERNAL__ __SKIP__ UNKNOWN ERROR AT DRAW_TASKS" << std::endl;
#endif
								fails_unexpected++;
							}
						}
						al_flip_display();
					}
				}
				catch (...) { // enhance later
					verynice = false;
				}

				if (once_tasks_m.is_locked()) once_tasks_m.unlock(); // this is my safer mutex no worries.

				thread_deinit();

				handler.deinit();
				timar.reset();

				return verynice;
			}
			void Display::thread_init()
			{
				fails_out_of_range = fails_unexpected = 0;
				al_set_new_display_flags(new_display_flags_apply);
				if (new_display_refresh_rate > 0) al_set_new_display_refresh_rate(new_display_refresh_rate);
				al_set_new_display_option(ALLEGRO_VSYNC, new_display_should_vsync ? 1 : 2, ALLEGRO_SUGGEST);
				// prettier drawings (anti aliasing on primitives)
				al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, ALLEGRO_SUGGEST, 2);
				al_set_new_display_option(ALLEGRO_SAMPLES, ALLEGRO_SUGGEST, 8);
				// endof prettier drawings

				display = DisplayStrongPtr(al_create_display(new_resolution[0], new_resolution[1]), [](ALLEGRO_DISPLAY* d) { al_destroy_display(d); d = nullptr; });
				
				al_apply_window_constraints(display.get(), true);
				al_set_window_constraints(display.get(), display::minimum_display_size[0], display::minimum_display_size[1], 0, 0); // minimum size 640,480, max not defined

				set_internal_display_as_bitmap_reference();

				display_events.add(Event(al_get_display_event_source(display.get())));
				display_events.set_run_autostart([&](const RawEvent& ev) {
					if (ev.type() == ALLEGRO_EVENT_DISPLAY_RESIZE) {
						al_acknowledge_resize(display.get());
						refresh_camera = true;
					}
				});

				refresh_camera = true;
			}
			void Display::thread_deinit()
			{
				display_events.deinit();
				if (display) {
					display.reset();
					display = nullptr;
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
				std::weak_ptr<ALLEGRO_DISPLAY> a = display; // won't lock display
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
				camera_f = [cam] { return cam; };
				refresh_camera = true;
			}
			void Display::set_camera(std::function<std::shared_ptr<Camera>(void)> f)
			{
				std::lock_guard<std::mutex> luck(cam_m);
				camera_f = f;
				refresh_camera = true;
			}
			std::shared_ptr<Camera> Display::get_current_camera() const
			{
				return camera_f();
			}
			bool Display::is_running() const
			{
				return display.operator bool();
			}
			void Display::set_new_flags(const int flags)
			{
				new_display_flags_apply = flags;
				if (!display) is_fullscreen = (new_display_flags_apply & ALLEGRO_FULLSCREEN || new_display_flags_apply & ALLEGRO_FULLSCREEN_WINDOW);
			}
			int Display::get_flags() const
			{
				return display.get() ? al_get_display_flags(display.get()) : new_display_flags_apply;
			}
			const int Display::get_width() const
			{
				return display.get() ? al_get_display_width(display.get()) : new_resolution[0];
			}
			const int Display::get_height() const
			{
				return display.get() ? al_get_display_height(display.get()) : new_resolution[1];
			}
			void Display::set_width(const int xx)
			{
				if (xx >= display::minimum_display_size[0]) {
					new_resolution[0] = xx;
					if (display.get()) al_resize_display(display.get(), new_resolution[0], get_height());
				}
			}
			void Display::set_height(const int yy)
			{
				if (yy >= display::minimum_display_size[1]) {
					new_resolution[1] = yy;
					if (display.get()) al_resize_display(display.get(), get_width(), new_resolution[1]);
				}
			}
			void Display::set_new_refresh_rate(const int hz)
			{
				new_display_refresh_rate = hz;
			}
			int Display::get_refresh_rate() const
			{
				return display.get() ? al_get_display_refresh_rate(display.get()) : new_display_refresh_rate;
			}
			void Display::set_new_vsync(const bool vs)
			{
				new_display_should_vsync = vs;
			}
			bool Display::toggle_fullscreen()
			{
				is_fullscreen = !is_fullscreen;
				if (display.get()) al_toggle_display_flag(display.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
				return is_fullscreen;
			}
			void Display::set_fullscreen(const bool fullscr)
			{
				is_fullscreen = fullscr;
				if (display.get()) al_toggle_display_flag(display.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
			}
			Tools::Future<DisplayAnySPtr> Display::add_once_task(DisplayAnyFSPtr tsk)
			{
				Tools::Promise<DisplayAnySPtr> my_future(tsk);
				Tools::Future<DisplayAnySPtr> future = std::move(my_future.get_future());

				Tools::AutoLock luck(once_tasks_m);
				
				once_tasks.emplace_back(std::move(my_future));
				
				return future;
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
				if (display) {
					return Event(al_get_display_event_source(display.get()));
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