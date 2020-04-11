#include "core.h"

namespace LSW {
	namespace v5 {

		Core::core_data Core::data;

		int Core::thr_base(const core::thr_ids b, Threads::boolThreadF& f)
		{
			Logger logg;
			logg << L::SLF << fsr(__FUNCSIG__) << "Initializing thread &2" << static_cast<int>(b) << "..." << L::ELF;

			for (bool exit_this = false; !exit_this;) { // while no fatal error, try?
				try {
					switch (b) {
					case core::thr_ids::DRAWING:
						thr_0(f);
						exit_this = true;
						break;
					case core::thr_ids::COLLIDING:
						thr_1(f);
						exit_this = true;
						break;
					case core::thr_ids::EVENTS:
						thr_2(f);
						exit_this = true;
						break;
					case core::thr_ids::FUNCTIONAL:
						thr_3(f);
						exit_this = true;
						break;
					}
				}
				catch (Abort::Abort err) {
					switch (err.getLevel()) {
					case Abort::abort_level::GIVEUP:
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&9Can't keep up! &eGIVEUP &9at Thread &2" << static_cast<int>(b) << "&9!" << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&9From: &e" << err.getWhereFrom() << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&9Details: &e" << err.getDetails() << L::ELF;
						logg.flush();
						err.runFunctionIfAny();
						break;
					case Abort::abort_level::WARN:
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&6Warning! &eWARN &6at Thread &2" << static_cast<int>(b) << "&6!" << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&6From: &e" << err.getWhereFrom() << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "&6Details: &e" << err.getDetails() << L::ELF;
						logg.flush();
						err.runFunctionIfAny();
						break;
					case Abort::abort_level::FATAL_ERROR:
						logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4FATAL ERROR! &cThread &e" << static_cast<int>(b) << "&c had a FATAL ERROR!" << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4From: &c" << err.getWhereFrom() << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4Details: &c" << err.getDetails() << L::ELF;
						logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4ABORTING APP EXECUTION!" << L::ELF;
						logg.flush();
						err.runFunctionIfAny();
						exit_this = true;
						return -1;
					}
				}
			}
			logg << L::SLF << fsr(__FUNCSIG__) << "Ending thread &2" << static_cast<int>(b) << "..." << L::ELF;
			return 0;
		}

		void Core::thr_0(Threads::boolThreadF& keep) // DRAWING
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Variables used while thread is on:

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			const int thr_id = static_cast<int>(core::thr_ids::DRAWING);
			Logger logg;
			Display disp;
			Database db;
			coloured_string clr_str;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Useful functions to quickly do stuff

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			const auto throw_event_update_display_size = [&]() {
				ALLEGRO_EVENT evv;
				evv.type = Cast::a_cast(Shared::my_events::CUSTOM_EVENT_SHARING_NEW_DISPLAY_SIZE);
				evv.user.data1 = al_get_display_width(disp.getRawDisplay());
				evv.user.data2 = al_get_display_height(disp.getRawDisplay());
				al_emit_user_event(&data.evsrc, &evv, NULL);
			};
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > INITIALIZE < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			if (data.display_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread DRAWING..." << L::ELF;

				disp.init();

				data.display_routine.routines.insert(&data.evsrc);
				data.display_routine.routines.insert(disp.getEvent());
				data.display_routine.routines.insert(logg.getEvent());

				data.display_routine.routines.start();

				throw_event_update_display_size();
			}
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > LOOP < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread DRAWING..." << L::ELF;

			while (keep()) {
				data.display_routine.tick();

				try {
					while (data.display_routine.pause) {
						Sleep(50); // 20 per sec
						data.display_routine.tick(); // keep saying it's alive
					}

					if (data.display_routine.routines.hasEvent()) {

						// REGISTERED EVENT (here)

						if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::LOOPTRACK))) {
							db.set(database::e_sizet::FRAMESPERSECOND, data.display_routine.routines.getNumCallsDefault());
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::CHECKMEMORYBITMAP))) {
							al_convert_bitmaps();
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::UPDATELOGONSCREEN))) {
							// to be done
						}

						// OTHER EVENTS (Allegro and stuff)

						else {
							auto ev = data.display_routine.routines.getEventRaw();

							switch (static_cast<int>(ev.type)) {
							case ALLEGRO_EVENT_DISPLAY_CLOSE:
								if (ev.display.source == disp.getRawDisplay()) { // indeed close the game
									internalEnd();
									logg << L::SLF << fsr(__FUNCSIG__) << "&5Got EVENT_DISPLAY_CLOSE event on main window, setting to close the game..." << L::ELF;
								}
								break;
							case static_cast<int>(Shared::my_events::CUSTOM_EVENT_LOG_STRING):
									clr_str = *(coloured_string*)ev.user.data1;
									// set later the string on screen
								break;
							case static_cast<int>(Shared::my_events::CUSTOM_EVENT_EXTERNAL_EXIT_CALL):
								internalEnd();
								logg << L::SLF << fsr(__FUNCSIG__) << "&5Got EVENT_EXTERNAL_EXIT_CALL event on main window, setting to close the game..." << L::ELF;
								break;

								// BEING DONE
							}
						}
					}
				}
				catch (...) {

				}


				logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				al_rest(0.5);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > TERMINATE < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			logg << L::SLF << fsr(__FUNCSIG__) << "Ending Thread DRAWING..." << L::ELF;

			{
				data.display_routine.routines.stop();
				data.display_routine.routines.remove(&data.evsrc);
				data.display_routine.routines.remove(disp.getEvent());
				data.display_routine.routines.remove(logg.getEvent());

				disp.deinit();
				data.display_routine.deinitialize(); // set as unitialized once ended the thread stuff
			}
		}

		void Core::thr_1(Threads::boolThreadF& keep) // COLLIDING
		{
			const int thr_id = static_cast<int>(core::thr_ids::COLLIDING);
			Logger logg;

			// variables used here:

			if (data.collision_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread COLLISION..." << L::ELF;

			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread COLLISION..." << L::ELF;
			while (keep()) {
				data.collision_routine.tick();
				logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				al_rest(0.5);
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Ending Thread COLLISION..." << L::ELF;
			data.collision_routine.deinitialize(); // set as unitialized once ended the thread stuff
		}

		void Core::thr_2(Threads::boolThreadF& keep) // EVENTS
		{
			const int thr_id = static_cast<int>(core::thr_ids::EVENTS);
			Logger logg;

			// variables used here:

			if (data.events_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread EVENTS..." << L::ELF;

			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread EVENTS..." << L::ELF;
			while (keep()) {
				data.events_routine.tick();
				logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				al_rest(0.5);
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Ending Thread EVENTS..." << L::ELF;
			data.events_routine.deinitialize(); // set as unitialized once ended the thread stuff
		}

		void Core::thr_3(Threads::boolThreadF& keep) // FUNCTIONAL
		{
			const int thr_id = static_cast<int>(core::thr_ids::FUNCTIONAL);
			Logger logg;

			// variables used here:

			if (data.functional_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread FUNCTIONAL..." << L::ELF;

			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread FUNCTIONAL..." << L::ELF;
			while (keep()) {
				data.functional_routine.tick();
				logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				al_rest(0.5);
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Ending Thread FUNCTIONAL..." << L::ELF;
			data.functional_routine.deinitialize(); // set as unitialized once ended the thread stuff
		}

		void Core::internalEnd()
		{
			data.display_routine.this_core.stop();
			data.collision_routine.this_core.stop();
			data.events_routine.this_core.stop();
			data.functional_routine.this_core.stop();
		}

		Core::Core()
		{
			data.gmute.lock();
			if (!data.has_init_once) {
				lsw_al_init();

				al_init_user_event_source(&data.evsrc);

				data.display_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::DRAWING, f); });
				data.collision_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::COLLIDING, f); });
				data.events_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::EVENTS, f); });
				data.functional_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::FUNCTIONAL, f); });

				data.has_init_once = true;
			}
			data.gmute.unlock();
		}

		void Core::init()
		{
			data.gmute.lock();

			data.display_routine.this_core.start();
			data.collision_routine.this_core.start();
			data.events_routine.this_core.start();
			data.functional_routine.this_core.start();

			data.gmute.unlock();
		}

		void Core::fEnd()
		{
			data.gmute.lock();

			// (this) ABORT tested and it's working pretty well!
			Abort::AutoAbort aa(5000, []() { Logger logg; logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4FATAL ERROR: COULD NOT END PROPERLY! FORCING GAME TO END after 5 seconds trying to end." << L::ELF; logg.flush(); });

			// not exactly needed because join(); actually calls stop(), but we can set everyone to stop first and then sync (instead of waiting each one stop to then call the other to stop and so on)
			internalEnd();

			data.display_routine.this_core.join();
			data.collision_routine.this_core.join();
			data.events_routine.this_core.join();
			data.functional_routine.this_core.join();

			aa.abort();

			data.gmute.unlock();
		}

		bool Core::allEnded()
		{
			return !data.display_routine.isInitialized() && !data.collision_routine.isInitialized() && !data.events_routine.isInitialized() && !data.functional_routine.isInitialized();
		}

		bool Core::oneAlive()
		{
			return data.display_routine.isAlive() || data.collision_routine.isAlive() || data.events_routine.isAlive() || data.functional_routine.isAlive();
		}

		bool Core::allAlive()
		{
			return data.display_routine.isAlive() && data.collision_routine.isAlive() && data.events_routine.isAlive() && data.functional_routine.isAlive();
		}

	}
}