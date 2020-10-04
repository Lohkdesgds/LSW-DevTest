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
				thread_init();

				while (keep()) {

					if (refresh_camera) {
						cam_m.lock();
						auto cam_cpy = camera;
						if (refresh_camera && cam_cpy) {
							if (refresh_camera) refresh_camera = !cam_cpy->classic_update(get_buffer_ref());
							//refresh_camera = false;
							cam_cpy->classic_refresh();
							cam_cpy->apply();
						}
						cam_m.unlock();
					}

					once_tasks_m.lock();
					for (auto& i : once_tasks) i.work(); //i.set_value(10);
					once_tasks.clear();
					once_tasks_m.unlock();


					al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));

					draw_tasks_m.lock();
					for (auto& i : draw_tasks) i.second();
					draw_tasks_m.unlock();

					al_flip_display();
				}

				thread_deinit();

				return true;
			}
			void Display::thread_init()
			{
				al_set_new_display_flags(new_display_flags_apply);
				if (new_display_refresh_rate > 0) al_set_new_display_refresh_rate(new_display_refresh_rate);
				al_set_new_display_option(ALLEGRO_VSYNC, new_display_should_vsync ? 1 : 2, ALLEGRO_SUGGEST);

				display = DisplayStrongPtr(al_create_display(640, 480), [](ALLEGRO_DISPLAY* d) { al_destroy_display(d); d = nullptr; });
				
				al_apply_window_constraints(display.get(), true);
				al_set_window_constraints(display.get(), display::minimum_display_size[0], display::minimum_display_size[1], 0, 0); // minimum size 640,480, max not defined

				display_events.add(Event(al_get_display_event_source(display.get())));
				display_events.set_run_autostart([&](RawEvent& ev) {
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
			Tools::Future<> Display::init()
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
			void Display::stop()
			{
				thr.stop();
			}
			void Display::set_camera(std::shared_ptr<Camera> cam)
			{
				cam_m.lock();
				camera = cam;
				cam_m.unlock();
				refresh_camera = true;
			}
			bool Display::is_running()
			{
				return display.operator bool();
			}
			void Display::set_new_display_flags(const int flags)
			{
				new_display_flags_apply = flags;
				if (!display) is_fullscreen = (new_display_flags_apply & ALLEGRO_FULLSCREEN || new_display_flags_apply & ALLEGRO_FULLSCREEN_WINDOW);
			}
			void Display::set_new_display_refresh_rate(const int hz)
			{
				new_display_refresh_rate = hz;
			}
			void Display::set_new_display_vsync(const bool vs)
			{
				new_display_should_vsync = vs;
			}
			void Display::toggle_fullscreen()
			{
				is_fullscreen = !is_fullscreen;
				al_toggle_display_flag(display.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
			}
			void Display::set_fullscreen(const bool fullscr)
			{
				is_fullscreen = fullscr;
				al_toggle_display_flag(display.get(), ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);
			}
			Tools::Future<DisplayAnySPtr> Display::add_once_task(DisplayAnyFSPtr tsk)
			{
				Tools::Promise<DisplayAnySPtr> my_future(tsk);
				Tools::Future<DisplayAnySPtr> future = std::move(my_future.get_future());
				once_tasks_m.lock();
				once_tasks.emplace_back(std::move(my_future));
				once_tasks_m.unlock();
				return future;
			}
			size_t Display::add_draw_task(DisplayTask tsk)
			{
				draw_tasks_m.lock();
				draw_tasks.push_back({ ++draw_tasks_count, tsk });
				size_t this_task = draw_tasks_count;
				draw_tasks_m.unlock();
				return this_task;
			}
			void Display::remove_draw_task(const size_t tskid)
			{
				draw_tasks_m.lock();
				for (size_t p = 0; p < draw_tasks.size(); p++) {
					if (draw_tasks[p].first == tskid) {
						draw_tasks.erase(draw_tasks.begin() + p);
						return;
					}
				}
				draw_tasks_m.unlock();
			}
			Event Display::get_event_source()
			{
				if (display) {
					return Event(al_get_display_event_source(display.get()));
				}
				return Event();
			}
			/*DisplayWeakPtr Display::_get_raw_display() const
			{
				return display;
			}*/
		}
	}
}