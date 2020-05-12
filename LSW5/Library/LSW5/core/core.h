#pragma once

#include <thread>
#include <mutex>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <physfs.h>

#include "..\abort\abort.h"
#include "..\supertimer\supertimer.h"
#include "..\supermap\supermap.h"
#include "..\logger\logger.h"
#include "..\display\display.h"
#include "..\shared\shared.h"
#include "..\autocaster\autocaster.h"

#include "..\superresource\superresource.h"
#include "..\sprite\sprite.h"

/*
LATER: add a way to create threads to specific layers of collision
- this way a thread is responsible to do the work in a layer
*/


namespace LSW {
	namespace v5 {
		namespace core {

			/*
			LOOP_TRACK is just to update stuff (database data)
			*/

			enum class thr_display_routines		{ LOOP_TRACK, CHECK_MEMORY_BITMAP_AND_CAMERA, UPDATE_LOG_ON_SCREEN };
			enum class thr_collision_routines	{ LOOP_TRACK, COLLISION_WORK };
			enum class thr_events_routines		{ LOOP_TRACK, UPDATE_MOUSE };
			enum class thr_functional_routines	{ LOOP_TRACK };

			enum class thr_ids {
				ALL = -1, DRAWING, COLLIDING, EVENTS, FUNCTIONAL,
				__THREADS_COUNT
			};


			typedef SuperTimer<1, 5, 5>								__display_routines;
			typedef SuperTimer<1, Shared::game_timing_tps>			__collision_routines;
			typedef SuperTimer<1, 60>								__events_routines;
			typedef SuperTimer<1>									__functional_routines;
		}


		class Core {
			struct core_data {

				template<typename T>
				class core_each_core_data {
				public:
					Threads::CustomThread<int> this_core{};
					double last_loop = 0.0;
					double last_delta_loop = 0.0;
					T routines{};
					bool pause = false;
					bool success_pause = false;
					bool has_initialized_once = false;

					void tick() { last_delta_loop = al_get_time() - last_loop; last_loop = al_get_time(); } // update time
					bool isPaused() { return success_pause = pause; }
					double lastDeltaT() { return last_delta_loop; }

					bool isAlive() { return al_get_time() - last_loop < 1.0; } // had tick < 1 seg ago
					bool isInitialized() { return has_initialized_once; } // true if on mid of the thread (via initialize() in first line and unitialize() in the end ON THREAD!)
					bool hasPausedSuccessfully() { return success_pause; }

					bool initialize() { bool k = has_initialized_once; has_initialized_once = true; return !k; } // return true if has to initialize, and also set that initialized
					void deinitialize() { has_initialized_once = false; } // set as unitialized
				};

				core_each_core_data<core::__display_routines>		display_routine;
				core_each_core_data<core::__collision_routines>		collision_routine;
				core_each_core_data<core::__events_routines>		events_routine;
				core_each_core_data<core::__functional_routines>	functional_routine;

				ALLEGRO_EVENT_SOURCE evsrc{};
				std::mutex gmute;
				bool has_init_once = false;

				bool should_exit = false; // every thread tests this guy value
			};

			static core_data data;

			int thr_base(const core::thr_ids, Threads::boolThreadF&); // they all have try catch and same base

			void thr_0(Threads::boolThreadF&); // DRAWING
			void thr_1(Threads::boolThreadF&); // COLLIDING
			void thr_2(Threads::boolThreadF&); // EVENTS
			void thr_3(Threads::boolThreadF&); // FUNCTIONAL

			void internalEnd();
		public:
			Core();

			void init();
			void pause();
			void unpause();
			void fEnd();

			bool allEnded(); // all threads said they're not "initialized", so must be dead.
			bool oneAlive(); // at least one thread still running (1 sec max unload time)
			bool allAlive(); // all threads are running?
			bool allPaused();// have they paused successfully?
		};
	}
}