#pragma once

// C
#define _CRT_RAND_S
#include <allegro5/allegro.h>
// C++
#include <string>
#include <vector>
#include <chrono>
#include <thread>
// Others
#include "..\Abort\abort.h"
#include "..\Lower\bitmap\bitmap.h"
#include "..\superthread\superthread.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\supermutex\supermutex.h"
#include "..\tools\tools.h"
#include "..\supermap\supermap.h"
#include "..\superresource\superresource.h"
#include "..\base\base.h"

namespace LSW {
	namespace v5 {

		namespace display {
			constexpr int display_default_flags = ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL;
			constexpr int display_minimum_flags = ALLEGRO_OPENGL;
			constexpr int bitmap_default_mode = ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP;
			constexpr size_t end_timeout_max_time = 5; // seconds
			constexpr auto acknowledge_timeout_time = std::chrono::milliseconds(100);
			constexpr int display_reference_size[] = { 1280,720 };

			/*
			double* ref_fx_amount = nullptr;
			double* ref_buffer_prop = nullptr;
			int* ref_fps_limit = nullptr;
			std::string* ref_print_path = nullptr;
			bool* ref_doublebuffer = nullptr;
			*/

			enum class e_double { FX_AMOUNT, RESOLUTION_BUFFER_PROPORTION };
			enum class e_integer { LIMIT_FPS };
			enum class e_string { PRINT_PATH };
			enum class e_boolean { DOUBLE_BUFFERING };

			const SuperMap<std::function<double(void)>> e_double_defaults = {
				{[] {return 0.0; },								(e_double::FX_AMOUNT),									CHAR_INIT("fx_amount")},
				{[] {return 1.0; },								(e_double::RESOLUTION_BUFFER_PROPORTION),				CHAR_INIT("buffer_proportion")}
			};
			const SuperMap<std::function<int(void)>> e_integer_defaults = {
				{[] {return 0; },								(e_integer::LIMIT_FPS),									CHAR_INIT("fps_limit")}
			};
			const SuperMap<std::function<std::string(void)>> e_string_defaults = {
				{[] {return std::string(""); },					(e_string::PRINT_PATH),									CHAR_INIT("print_path")}
			};
			const SuperMap<std::function<bool(void)>> e_boolean_defaults = {
				{[] {return false; },							(e_boolean::DOUBLE_BUFFERING),							CHAR_INIT("double_buffer")}
			};


		}

		class Display : public Workaround<double>, public Workaround<int>, public Workaround<std::string>, public Workaround<bool> {
			struct display_managing {
				std::vector<ALLEGRO_DISPLAY_MODE> display_modes;
				size_t last_display_mode = 0;
			} fullscreen;
			struct display_info {
				ALLEGRO_DISPLAY_MODE chosen_mode{};
				int latest_display_flags = display::display_minimum_flags;
			} display_now_config;
			/*struct display_windowed {
				size_t refresh_rate = 0;
				int display_size[2] = { 0, 0 };
			} window;*/

			bool was_window = false;

			std::function<void(std::string)> function_events;

			/*struct display_base_data {
				SuperMap<std::function<double(void)>>			double_data = display::e_double_defaults;
				SuperMap<std::function<int(void)>>				integer_data = display::e_integer_defaults;
				SuperMap<std::function<std::string(void)>>		string_data = display::e_string_defaults;
				SuperMap<std::function<bool(void)>>				boolean_data = display::e_boolean_defaults;
			} data;*/

			//ALLEGRO_DISPLAY_MODE chosen_mode{}; // DO NOT USE .FORMAT AS MODE! NOT THE SAME! .FORMAT IS PIXEL FORMAT AKA RGB STUFF
			//int latest_display_flags = display::display_minimum_flags;

			ALLEGRO_DISPLAY* display_itself = nullptr;
			std::shared_ptr<Bitmap> display_itself_follower;

			std::shared_ptr<Bitmap> display_buffer;
			const std::string display_buffer_name = Tools::generateRandomUniqueStringN();
			
			bool use_doublebuffer = false;

			std::chrono::microseconds time_last_draw = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			std::chrono::milliseconds time_last_acknowledge = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			bool should_acknowledge = false;

			bool is_printing_screen = false;
			bool print_pending = false;
			Threads::CustomThread<int> print_thread;

			void __init(const int);
			void __deinit();
			bool __lastWasWindow();
			void __setacknowledge();
			void __checkacknowledge();

			void __print(const std::string&&);
			void __printscreen();
		public:
			using Workaround<double>::set;
			using Workaround<double>::get;
			using Workaround<double>::getDirect;
			using Workaround<int>::set;
			using Workaround<int>::get;
			using Workaround<int>::getDirect;
			using Workaround<std::string>::set;
			using Workaround<std::string>::get;
			using Workaround<std::string>::getDirect;
			using Workaround<bool>::set;
			using Workaround<bool>::get;
			using Workaround<bool>::getDirect;


			Display();
			Display(const int);
			~Display();

			void reFlags(const int);

			// which config to use? you might want to use getNearestConfiguration
			bool launch(const size_t = static_cast<size_t>(-1));
			// launch as window
			bool launchWindow(const int, const int, size_t = static_cast<size_t>(-1));
			bool relaunch();
			void close();

			void flip();
			void printscreen();
			void refreshMemoryBitmaps();
			void acknowledgeDisplay();


			const std::vector<ALLEGRO_DISPLAY_MODE>& configurations() const;
			const size_t getNearestConfiguration(const int, const int, const size_t = static_cast<size_t>(-1));
			const size_t getHighestConfiguration();
			const size_t getHighestMonitorRefreshRate();
			const display_info getLatest();

			ALLEGRO_DISPLAY* getRawDisplay();
			ALLEGRO_EVENT_SOURCE* getEvent();
		};


	}
}
