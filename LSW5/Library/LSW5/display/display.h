#pragma once

#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "..\Abort\abort.h"
#include "..\Database\database.h"
#include "..\superthread\superthread.h"
#include "..\logger\logger.h"

namespace LSW {
	namespace v5 {

		namespace display {

			//constexpr int display_default_mode = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL; // exists on Database default value for FLAGS known as "d_mode"
			constexpr int display_minimum_mode = ALLEGRO_RESIZABLE | ALLEGRO_OPENGL;
			constexpr int bitmap_default_mode = ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP;
			
			constexpr int display_reference_size[] = { 1280,720 };

			constexpr double display_fixed_acknowledge_delta_t = 1.5;
		}

		bool isDisplayModeEq(ALLEGRO_DISPLAY_MODE, ALLEGRO_DISPLAY_MODE);

		class Display {
			class modes {
				std::vector<ALLEGRO_DISPLAY_MODE> options;
				int lastmode = ALLEGRO_OPENGL;
			public:
				void loadOptions(const int);

				// last optional: format
				bool checkExistance(const int, const int, const int);

				const auto& getAvailableResolutions();
			};

			ALLEGRO_DISPLAY* display = nullptr;
			ALLEGRO_BITMAP* display_buffer = nullptr;
			double last_draw_tick = 0;
			double last_acknowledge = 0;

			bool print_pending = false;

			// linked directly to a config entry
			double*			ref_fx_amount		= nullptr;
			double*			ref_buffer_prop		= nullptr;
			int*			ref_fps_limit		= nullptr;
			std::string*	ref_print_path		= nullptr;
			bool*			ref_doublebuffer	= nullptr;

			modes display_modes;
			Threads::CustomThread<int> print_thread;

			void _print();
			void checkAcknowledge();
		public:
			Display(); // CALLS DATABASE! CREATE IT BEFORE CREATING A DISPLAY!
			~Display();

			void init();
			void restart();
			void deinit();

			void refreshBitmaps();
			void flip();

			void acknowledgeDisplay();

			void print();
			
			ALLEGRO_EVENT_SOURCE* getEvent() const;
			ALLEGRO_DISPLAY* getRawDisplay() const;
		};
	}
}
