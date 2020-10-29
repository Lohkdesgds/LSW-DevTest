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

			// Safe smart pointer of RAW DISPLAY.
			using DisplayStrongPtr = std::shared_ptr<ALLEGRO_DISPLAY>;
			// Any type.
			using DisplayAnySPtr = Tools::Any;
			// Function that returns any.
			using DisplayAnyFSPtr = std::function<DisplayAnySPtr(void)>;

			using DisplayTask = std::function<void(Camera&)>;

			class Display {
				int new_display_flags_apply = display::default_new_display_flags;
				int new_display_refresh_rate = 0; // not set
				int new_resolution[2] = { display::minimum_display_size[0], display::minimum_display_size[1] };
				bool new_display_should_vsync = false;
				bool is_fullscreen = false;
				size_t frames_per_second = 0;
				size_t fails_out_of_range = 0; // out of range ignored because this is faster
				size_t fails_unexpected = 0; // other error?

				DisplayStrongPtr display;
				EventHandler display_events;
				
				Tools::SuperThreadT<bool> thr;				

				size_t draw_tasks_count = 0;
				std::vector<std::pair<size_t, DisplayTask>> draw_tasks;
				Tools::SuperMutex draw_tasks_m;

				std::vector<Tools::Promise<DisplayAnySPtr>> once_tasks; // change later to a class that manages ALLEGRO_BITMAP*
				Tools::SuperMutex once_tasks_m;

				std::mutex cam_m;
				//std::shared_ptr<Camera> camera;
				std::function<std::shared_ptr<Camera>(void)> camera_f = [] {return nullptr; };
				bool refresh_camera = false;

				std::shared_ptr<Bitmap> get_buffer_ref();

				bool thread_run(Tools::boolThreadF);

				void thread_init();
				void thread_deinit();

			public:
				Display();
				~Display();

				/// <summary>
				/// <para>Sets Bitmap's reference as the display buffer (this is set by default once).</para>
				/// </summary>
				void set_internal_display_as_bitmap_reference();

				/// <summary>
				/// <para>Start drawing thread.</para>
				/// </summary>
				/// <returns>{Future} A Future that you can .wait() or .then() when display is closed.</returns>
				Tools::Future<bool> init();

				/// <summary>
				/// <para>Stops the thread and join.</para>
				/// </summary>
				void deinit();

				/// <summary>
				/// <para>Set the drawing thread to stop (no lock).</para>
				/// </summary>
				void set_stop();

				/// <summary>
				/// <para>Set a Camera to be applied to drawing thread.</para>
				/// </summary>
				/// <param name="{std::shared_ptr}">The Camera smart pointer.</param>
				void set_camera(std::shared_ptr<Camera>);

				/// <summary>
				/// <para>Set a function that gives a valid Camera to be applied to drawing thread.</para>
				/// </summary>
				/// <param name="{std::function}">The function that returns a Camera pointer.</param>
				void set_camera(std::function<std::shared_ptr<Camera>(void)>);

				/// <summary>
				/// <para>Gets the latest camera set in this Display (via set_camera).</para>
				/// </summary>
				/// <returns>{std::shared_ptr} The Camera.</returns>
				std::shared_ptr<Camera> get_current_camera() const;

				/// <summary>
				/// <para>Gets if drawing thread exists and it's running.</para>
				/// </summary>
				/// <returns>{bool} True if running.</returns>
				bool is_running() const;

				/// <summary>
				/// <para>What flags should the new display have?</para>
				/// </summary>
				/// <param name="{int}">Binary combination of flags (ALLEGRO flags, like ALLEGRO_OPENGL|ALLEGRO_WINDOWED).</param>
				void set_new_flags(const int = display::default_new_display_flags);

				/// <summary>
				/// <para>What flags does this display have set?</para>
				/// </summary>
				/// <returns>{int} Flags.</returns>
				int get_flags() const;

				/// <summary>
				/// <para>Get Display width.</para>
				/// </summary>
				/// <returns>{int} Display width.</returns>
				const int get_width() const;

				/// <summary>
				/// <para>Get Display height.</para>
				/// </summary>
				/// <returns>{int} Display height.</returns>
				const int get_height() const;

				/// <summary>
				/// <para>Set resolution for the display.</para>
				/// </summary>
				/// <param name="{int}">Width.</param>
				void set_width(const int);

				/// <summary>
				/// <para>Set resolution for the display.</para>
				/// </summary>
				/// <param name="{int}">Height.</param>
				void set_height(const int);

				/// <summary>
				/// <para>Set display refresh rate.</para>
				/// </summary>
				/// <param name="{int}">Refresh rate. 0 goes with default settings.</param>
				void set_new_refresh_rate(const int);

				/// <summary>
				/// <para>Get current display refresh rate setting.</para>
				/// </summary>
				/// <returns>{int} The refresh rate.</returns>
				int get_refresh_rate() const;

				/// <summary>
				/// <para>Should it use VSync?</para>
				/// </summary>
				/// <param name="{bool}">VSync on/off?</param>
				void set_new_vsync(const bool);

				/// <summary>
				/// <para>Switch between fullscreen and windowed mode.</para>
				/// </summary>
				/// <returns>{bool} Is it fullscreen now?</returns>
				bool toggle_fullscreen();

				/// <summary>
				/// <para>Sets is it should be fullscreen or not.</para>
				/// </summary>
				/// <param name="{bool}">Fullscreen?</param>
				void set_fullscreen(const bool);

				/// <summary>
				/// <para>Adds a task to be done once only. Future gives you the result.</para>
				/// <para>The Future doesn't hold the exact type here, so you'll have to remember what your task returns.</para>
				/// </summary>
				/// <param name="{DisplayAnyFSPtr}">std::function that returns any type.</param>
				/// <returns>{Future} A Future that will hold the result of your function once done by drawing thread.</returns>
				Tools::Future<DisplayAnySPtr> add_once_task(DisplayAnyFSPtr);

				/// <summary>
				/// <para>Adds a task to be done every draw (exactly before flipping the screen).</para>
				/// <para>Useful for drawing your stuff indefinitely.</para>
				/// </summary>
				/// <param name="{std::function}">A function.</param>
				/// <returns>{size_t} The function ID in functions internal vector.</returns>
				size_t add_draw_task(DisplayTask);

				/// <summary>
				/// <para>Removes a task by ID (added via add_draw_task).</para>
				/// </summary>
				/// <param name="{size_t}">The task ID.</param>
				/// <returns>{std::function} The task function removed.</returns>
				DisplayTask remove_draw_task(const size_t);

				/// <summary>
				/// <para>Gets the display Event source.</para>
				/// </summary>
				/// <returns>{Event} The Event source.</returns>
				Event get_event_source() const;

				/// <summary>
				/// <para>Gets the frames per second (updates every second).</para>
				/// </summary>
				/// <returns>{size_t} Frames per second.</returns>
				size_t get_frames_per_second() const;

				/// <summary>
				/// <para>Shows how many errors of "out of range" happened (normally skipped, just debugging).</para>
				/// </summary>
				/// <returns>{size_t} Number of errors since init.</returns>
				size_t debug_errors_out_of_range_skip() const;

				/// <summary>
				/// <para>Shows how many errors of other sources happened (normally skipped).</para>
				/// </summary>
				/// <returns>{size_t} Number of errors since init.</returns>
				size_t debug_errors_unexpected() const;
			};
			

			

		}
	}
}