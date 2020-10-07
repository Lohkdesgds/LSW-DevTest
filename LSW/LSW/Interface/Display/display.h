#pragma once

#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Tools\SuperThread\superthread.h"
#include "..\..\Tools\Future\future.h"
#include "..\..\Tools\SuperMutex\supermutex.h"
#include "..\..\Tools\Common\common.h"
#include "..\..\Tools\Any\any.h"
#include "..\Events\events.h"
#include "..\Bitmap\bitmap.h"
#include "..\Font\font.h"
#include "..\Camera\camera.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace display {
				const int minimum_display_size[2] = { 640, 480 };
				constexpr int default_new_display_flags = ALLEGRO_RESIZABLE | ALLEGRO_OPENGL/* | ALLEGRO_FULLSCREEN_WINDOW*/;
			}


			using DisplayStrongPtr = std::shared_ptr<ALLEGRO_DISPLAY>;
			//using DisplayWeakPtr = std::weak_ptr<ALLEGRO_DISPLAY>;
			using DisplayAnySPtr = Tools::Any;
			using DisplayAnyFSPtr = std::function<DisplayAnySPtr(void)>;

			using DisplayTask = std::function<void(void)>;

			class Display {
				int new_display_flags_apply = display::default_new_display_flags;
				int new_display_refresh_rate = 0; // not set
				bool new_display_should_vsync = false;
				bool is_fullscreen = true;

				DisplayStrongPtr display;
				EventHandler display_events;
				
				Tools::SuperThreadT<bool> thr;				

				size_t draw_tasks_count = 0;
				std::vector<std::pair<size_t, DisplayTask>> draw_tasks;
				Tools::SuperMutex draw_tasks_m;

				std::vector<Tools::Promise<DisplayAnySPtr>> once_tasks; // change later to a class that manages ALLEGRO_BITMAP*
				Tools::SuperMutex once_tasks_m;

				std::mutex cam_m;
				std::shared_ptr<Camera> camera;
				bool refresh_camera = false;

				std::shared_ptr<Bitmap> get_buffer_ref();

				bool thread_run(Tools::boolThreadF);

				void thread_init();
				void thread_deinit();

			public:
				Display();
				~Display();

				// Starts the display thread and starts refresing screen
				Tools::Future<bool> init();
				// Stops tasks and closes screen
				void deinit();
				// Just sets to stop (do not close fully)
				void set_stop();

				// set a camera to be applied every time
				void set_camera(std::shared_ptr<Camera>);

				// if display exists, thread is running, so true
				bool is_running() const;

				// set new display flags
				void set_new_display_flags(const int);

				// set new display max refresh rate
				void set_new_display_refresh_rate(const int);

				// set new display if should vsync (default false)
				void set_new_display_vsync(const bool);

				// toggles between fullscreen and window mode
				void toggle_fullscreen();

				// sets if it should be fullscreen or window mode
				void set_fullscreen(const bool);

				// adds one task to be done once in the drawing task (like loading bitmaps or such)
				Tools::Future<DisplayAnySPtr> add_once_task(DisplayAnyFSPtr);
				// adds something that the drawing thread should do every time. Returns ID so you can manage later by this.
				size_t add_draw_task(DisplayTask);

				// removes something that the drawing thread is doing (that you've added with add_draw_task)
				void remove_draw_task(const size_t);

				// get Display event source (resize, close, and so on)
				Event get_event_source() const;

				//DisplayWeakPtr _get_raw_display() const;
			};
			

			

		}
	}
}