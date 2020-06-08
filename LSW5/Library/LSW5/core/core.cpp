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
			Camera gcam;

			SuperResource<Sprite_Base> sprites;

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

				/*				
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
				*/

				disp.set(display::e_double::FX_AMOUNT,						[&] {double d;		db.get(database::e_double::FX_AMOUNT, d);						return d; });
				disp.set(display::e_double::RESOLUTION_BUFFER_PROPORTION,	[&] {double d;		db.get(database::e_double::RESOLUTION_BUFFER_PROPORTION, d);	return d; });
				disp.set(display::e_integer::LIMIT_FPS,						[&] {int d;			db.get(database::e_integer::LIMIT_FPS, d);						return d; });
				disp.set(display::e_string::PRINT_PATH,						[&] {std::string d; db.get(database::e_string::PRINT_PATH, d);						return d; });
				disp.set(display::e_boolean::DOUBLE_BUFFERING,				[&] {bool d;		db.get(database::e_boolean::DOUBLE_BUFFERING, d);				return d; });

				int d_width, d_height, d_refresh_rate, d_flags;
				db.get(database::e_integer::SCREEN_X, d_width);
				db.get(database::e_integer::SCREEN_Y, d_height);
				db.get(database::e_integer::SCREEN_PREF_HZ, d_refresh_rate);
				db.get(database::e_integer::SCREEN_FLAGS, d_flags);

				disp.reFlags(d_flags);
				if (d_flags & ALLEGRO_WINDOWED) disp.launchWindow(d_width ? d_width : display::display_reference_size[0], d_height ? d_height : display::display_reference_size[1], d_refresh_rate);
				else disp.launch((d_width != 0 && d_height != 0) ? disp.getNearestConfiguration(d_width, d_height, d_refresh_rate) : disp.getHighestConfiguration());

				if (db.isEq(database::e_boolean::HIDEMOUSE, true)) al_hide_mouse_cursor(disp.getRawDisplay());

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

					while (data.display_routine.isPaused()) {
						Sleep(50); // 20 per sec
						data.display_routine.tick(); // keep saying it's alive
					}

					if (data.display_routine.routines.hasEvent()) { // LOOP_TRACK, CHECK_MEMORY_BITMAP_AND_CAMERA, UPDATE_LOG_ON_SCREEN

						// REGISTERED EVENT (here)

						if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::LOOP_TRACK))) {
							db.set(database::e_sizet::FRAMESPERSECOND, data.display_routine.routines.getNumCallsDefault());
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::CHECK_MEMORY_BITMAP_AND_CAMERA))) {
							al_convert_bitmaps();
							Camera cam;
							if (auto* ptr = cam.getLastCameraApply(); ptr) {
								ptr->refresh();
								ptr->apply();
							}
							else {
								throw Abort::Abort(__FUNCSIG__, "No camera found. Please do a Camera.apply() somewhere and keep the Camera object alive to work.", Abort::abort_level::GIVEUP);
							}
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::UPDATE_LOG_ON_SCREEN))) {
							// to be done
							// there's no such class to handle STRING yet (Text)
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
							case ALLEGRO_EVENT_DISPLAY_RESIZE:
								if (ev.display.source == disp.getRawDisplay()) {
									disp.acknowledgeDisplay();
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


					// draw?
					Camera* cam = gcam.getLastCameraApply();

					for(auto& i : sprites)
					{
						i->self->draw(cam);
					}

					//cam.getLastCameraApply()->matrix_debug();

					disp.flip();
					al_clear_to_color(al_map_rgb(0, 0, 0));
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
				}


				//logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				//al_rest(0.5);
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

				db.set(database::e_double::FX_AMOUNT,						(*disp.getRef(display::e_double::FX_AMOUNT))());
				db.set(database::e_double::RESOLUTION_BUFFER_PROPORTION,	(*disp.getRef(display::e_double::RESOLUTION_BUFFER_PROPORTION))());
				db.set(database::e_integer::LIMIT_FPS,						(*disp.getRef(display::e_integer::LIMIT_FPS))());
				db.set(database::e_string::PRINT_PATH,						(*disp.getRef(display::e_string::PRINT_PATH))());
				db.set(database::e_boolean::DOUBLE_BUFFERING,				(*disp.getRef(display::e_boolean::DOUBLE_BUFFERING))());

				auto d_info = disp.getLatest();
				db.set(database::e_integer::SCREEN_X, d_info.chosen_mode.width);
				db.set(database::e_integer::SCREEN_Y, d_info.chosen_mode.height);
				db.set(database::e_integer::SCREEN_PREF_HZ, d_info.chosen_mode.refresh_rate);
				db.set(database::e_integer::SCREEN_FLAGS, d_info.latest_display_flags);


				disp.close();
				data.display_routine.deinitialize(); // set as unitialized once ended the thread stuff
			}
		}

		void Core::thr_1(Threads::boolThreadF& keep) // COLLIDING
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Variables used while thread is on:

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			const int thr_id = static_cast<int>(core::thr_ids::COLLIDING);
			Logger logg;
			Database db;
			const Camera gcam;

			SuperResource<Sprite_Base> sprites;


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > INITIALIZE < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			if (data.collision_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread COLLISION..." << L::ELF;

				data.collision_routine.routines.start();
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > LOOP < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread COLLISION..." << L::ELF;
			while (keep()) {
				data.collision_routine.tick();

				try {

					while (data.collision_routine.isPaused()) {
						Sleep(50); // 20 per sec
						data.collision_routine.tick(); // keep saying it's alive
					}

					if (data.collision_routine.routines.hasEvent()) { // LOOP_TRACK, COLLISION_WORK

						// REGISTERED EVENT (here)

						if (data.collision_routine.routines.isThisThis(static_cast<size_t>(core::thr_collision_routines::LOOP_TRACK))) {
							db.set(database::e_sizet::COLLISIONSPERSECOND, data.collision_routine.routines.getNumCallsDefault());
						}
						else if (data.collision_routine.routines.isThisThis(static_cast<size_t>(core::thr_collision_routines::COLLISION_WORK))) {

							bool gottem = false;

							for (size_t times = 0; !(gottem = sprites.tryLock()) && times < 10; Sleep(10));

							if (gottem) {

								Camera* cam = gcam.getLastCameraApply();
								
								for (auto& i : sprites) {
									i->self->update(cam); // process positioning

									for (auto& j : sprites) {
										if (i->self != j->self) i->self->collide(cam, *j->self);
									}
								}




								sprites.unlock();
							}
							else {
								throw Abort::Abort(__FUNCSIG__, "Couldn't get mutex for Sprites after 10 tries.", Abort::abort_level::GIVEUP);
							}
						}
					}
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
				}

				//logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				//al_rest(0.5);
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Ending Thread COLLISION..." << L::ELF;
			data.collision_routine.deinitialize(); // set as unitialized once ended the thread stuff
		}

		void Core::thr_2(Threads::boolThreadF& keep) // EVENTS
		{
			const int thr_id = static_cast<int>(core::thr_ids::EVENTS);
			Logger logg;
			Database db;

			// variables used here:

			if (data.events_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread EVENTS..." << L::ELF;

				data.events_routine.routines.insert(&data.evsrc);

				data.events_routine.routines.start();
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread EVENTS..." << L::ELF;
			while (keep()) {
				data.events_routine.tick();

				try {
					while (data.events_routine.isPaused()) {
						Sleep(50); // 20 per sec
						data.events_routine.tick(); // keep saying it's alive
					}

					if (data.events_routine.routines.hasEvent()) { // LOOP_TRACK, UPDATE_MOUSE
						if (data.events_routine.routines.isThisThis(static_cast<size_t>(core::thr_events_routines::LOOP_TRACK))) {
							db.set(database::e_sizet::USEREVENTSPERSECOND, data.events_routine.routines.getNumCallsDefault());
						}
						else if (data.events_routine.routines.isThisThis(static_cast<size_t>(core::thr_events_routines::UPDATE_MOUSE))) {
							// get mouse and set position
						}

						// OTHER EVENTS (Allegro and stuff)

						//else {
							//auto ev = data.display_routine.routines.getEventRaw();

						//}
					}
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG, but this beta version won't handle any of this yet lmao." << L::ELF;
				}

				//logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF;
				//al_rest(0.5);
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

				while (data.functional_routine.isPaused()) {
					Sleep(50); // 20 per sec
					data.functional_routine.tick(); // keep saying it's alive
				}

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

		void Core::pause()
		{
			data.display_routine.pause = true;
			data.collision_routine.pause = true;
			data.events_routine.pause = true;
			data.functional_routine.pause = true;
		}

		void Core::unpause()
		{
			data.display_routine.pause = false;
			data.collision_routine.pause = false;
			data.events_routine.pause = false;
			data.functional_routine.pause = false;
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

		bool Core::allPaused()
		{
			return data.display_routine.success_pause && data.collision_routine.success_pause && data.events_routine.success_pause && data.functional_routine.success_pause;
		}

	}
}