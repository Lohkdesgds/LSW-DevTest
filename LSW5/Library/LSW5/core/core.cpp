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
			SuperResource<Camera> cameras;
			SuperResource<Sprite_Base> sprites;

			/*if (cameras.size() == 0) {
				throw Abort::Abort(__FUNCSIG__, "NO CAMERA HAS BEEN SET UP! Please set up a Camera! (Using SuperResource)");
			}*/


			//std::shared_ptr<Camera> main_cam = cameras.getMain(); // get main cam as main camera
			/*{
				auto ref_orig = sprites.customLoad("_DEBUG_TEXT", [](Sprite_Base*& b) {return (b = new Text()); });
				Text* mtt = (Text*)&(*ref_orig);
				mtt->load("_FONT");
				mtt->set(text::e_cstring::STRING,			[&] {return clr_str; });
				//mtt->set(sprite::e_boolean::DRAW,			[&]	{bool d;			db.get(database::e_boolean::WAS_OSD_ON, d);						return d; }());
				mtt->set(sprite::e_double::SCALE_G, 0.03);
				mtt->set(sprite::e_double::SCALE_X, 0.55);
				mtt->set(sprite::e_double::TARG_POSY, 0.965); // 0.935
				mtt->set(sprite::e_double::TARG_POSX, -1.0);
				//mtt->set(text::e_integer::STRING_MODE, +text::e_alignment::ALIGN_LEFT);
				mtt->set(text::e_double::TEXT_UPDATE_TIME, 1.0 / 5);
				mtt->set(sprite::e_boolean::AFFECTED_BY_CAM, false);
				mtt->set(sprite::e_integer::LAYER, 100);
				//mtt->set(sprite::e_color::COLOR, [&] {return clr_str; }());
			}*/




			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			Useful functions to quickly do stuff

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			/*const auto throw_event_update_display_size = [&]() {
				ALLEGRO_EVENT evv;
				evv.type = Cast::a_cast(Shared::my_events::CUSTOM_EVENT_SHARING_NEW_DISPLAY_SIZE);
				evv.user.data1 = al_get_display_width(disp.getRawDisplay());
				evv.user.data2 = al_get_display_height(disp.getRawDisplay());
				al_emit_user_event(&data.evsrc, &evv, NULL);
			};*/
			
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
				//data.display_routine.routines.insert(logg.getEvent()); // don't need to handle this at all

				data.display_routine.routines.start();

				//throw_event_update_display_size();
			}
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			> > > > LOOP < < < <

			* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread DRAWING..." << L::ELF;

			while (keep()) {
				data.display_routine.tick();

				try {

					while (data.display_routine.isPaused()) {
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						//Sleep(50); // 20 per sec
						data.display_routine.tick(); // keep saying it's alive
					}

					if (data.display_routine.routines.hasEvent()) { // LOOP_TRACK, CHECK_MEMORY_BITMAP_AND_CAMERA, UPDATE_LOG_ON_SCREEN

						// REGISTERED EVENT (here)

						if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::LOOP_TRACK))) {
							db.set(database::e_sizet::FRAMESPERSECOND, data.display_routine.routines.getNumCallsDefault());
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::CHECK_MEMORY_BITMAP))) {
							al_convert_bitmaps();
						}
						else if (data.display_routine.routines.isThisThis(static_cast<size_t>(core::thr_display_routines::CHECK_CAMERA_REFRESH))) {
							if (auto cam = cameras.getMain(); cam) {
								cam->refresh();
								cam->apply();
							}
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
							case static_cast<int>(Shared::my_events::CUSTOM_EVENT_CALL_FULLSCREEN):
								al_set_display_flag(disp.getRawDisplay(), ALLEGRO_FULLSCREEN_WINDOW, (bool)ev.user.data1);
								ev.display.source = disp.getRawDisplay();
								// yes, no break
							case ALLEGRO_EVENT_DISPLAY_RESIZE:
								if (ev.display.source == disp.getRawDisplay()) {
									disp.acknowledgeDisplay();
									//throw_event_update_display_size();
								}
								break;
							/*case static_cast<int>(Shared::my_events::CUSTOM_EVENT_LOG_STRING): // no reason to handle this here
									clr_str = *(coloured_string*)ev.user.data1;
									// set later the string on screen
								break;*/
							/*case static_cast<int>(Shared::my_events::CUSTOM_EVENT_EXTERNAL_EXIT_CALL):
								internalEnd();
								logg << L::SLF << fsr(__FUNCSIG__) << "&5Got EVENT_EXTERNAL_EXIT_CALL event on main window, setting to close the game..." << L::ELF;
								break;*/
							case static_cast<int>(Shared::my_events::CUSTOM_EVENT_DISPLAY_UPDATE_RESOLUTION_SCALE):
								{
									double scale = ev.user.data1 * 1.0 / 100;
									bool is_enabled = ev.user.data2;

									db.set(database::e_double::RESOLUTION_BUFFER_PROPORTION, scale);
									db.set(database::e_boolean::DOUBLE_BUFFERING, is_enabled);

									disp.acknowledgeDisplay();
								}
								break;
							case static_cast<int>(Shared::my_events::CUSTOM_EVENT_DISPLAY_CHROMA_FX):
								{
									double fx_amount = ev.user.data1 * 1.0 / 1000;
									db.set(database::e_double::FX_AMOUNT, fx_amount);

									disp.acknowledgeDisplay();
								}
								break;

								// BEING DONE
							}
						}
					}


					// draw?
					if (auto main_cam = cameras.getMain(); main_cam) {
						main_cam->matrix_debug();
						main_cam->apply();
						auto& con = main_cam->getAttributes();

						for (auto& c : con.layers) {
							for (auto& i : sprites)
							{
								i->draw(&(*main_cam), c.getLayerID());
							}
						}
					}
					else {
						throw Abort::Abort(__FUNCSIG__, "NO MAIN CAMERA HAS BEEN SET UP! Please set up a Camera for drawing! (Using SuperResource.setMain())");
					}

					//cam.getLastCameraApply()->matrix_debug();

					disp.flip();
					al_clear_to_color(al_map_rgb(0, 0, 0));
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [LSW], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (std::exception err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [STD], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.what() << L::ELF;

				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [???], but this beta version won't handle any of this yet lmao." << L::ELF;
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
				//data.display_routine.routines.remove(logg.getEvent());

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
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						//Sleep(50); // 20 per sec
						data.collision_routine.tick(); // keep saying it's alive
					}

					data.collision_routine.routines.hasEventWait();// LOOP_TRACK, COLLISION_WORK

					// REGISTERED EVENT (here)

					if (data.collision_routine.routines.isThisThis(static_cast<size_t>(core::thr_collision_routines::LOOP_TRACK))) {
						db.set(database::e_sizet::COLLISIONSPERSECOND, data.collision_routine.routines.getNumCallsDefault());
					}
					else if (data.collision_routine.routines.isThisThis(static_cast<size_t>(core::thr_collision_routines::COLLISION_WORK))) {

						sprites.lock();

						SuperResource<Camera> cameras;

						if (auto main_cam = cameras.getMain(); main_cam) {
							for (auto& i : sprites) {
								i->update(&(*main_cam)); // process positioning

								for (auto& j : sprites) {
									if (i != j) i->collide(&(*main_cam), *j.data());
								}
							}
						}
						else {
							sprites.unlock();
							throw Abort::Abort(__FUNCSIG__, "NO MAIN CAMERA HAS BEEN SET UP! Please set up a Camera! (Using SuperResource.setMain())");
						}

						sprites.unlock();
					}
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [LSW], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (std::exception err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [STD], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.what() << L::ELF;

				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [???], but this beta version won't handle any of this yet lmao." << L::ELF;
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

			SuperResource<Sprite_Base> sprites;

			// variables used here:

			if (data.events_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread EVENTS..." << L::ELF;

				data.events_routine.routines.insert(&data.evsrc);
				data.events_routine.routines.insert(al_get_keyboard_event_source());
				data.events_routine.routines.insert(al_get_mouse_event_source());

				data.events_routine.routines.start();
			}



			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread EVENTS..." << L::ELF;
			while (keep()) {
				data.events_routine.tick();

				try {
					while (data.events_routine.isPaused()) {
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						//Sleep(50); // 20 per sec
						data.events_routine.tick(); // keep saying it's alive
					}

					data.events_routine.routines.hasEventWait();// LOOP_TRACK, UPDATE_MOUSE
					if (data.events_routine.routines.isThisThis(static_cast<size_t>(core::thr_events_routines::LOOP_TRACK))) {
						db.set(database::e_sizet::USEREVENTSPERSECOND, data.events_routine.routines.getNumCallsDefault());
					}
					else if (data.events_routine.routines.isThisThis(static_cast<size_t>(core::thr_events_routines::UPDATE_MOUSE))) {
						SuperResource<Camera> cameras;

						double md[2];
						db.get(database::e_double::RAW_MOUSE_X, md[0]);
						db.get(database::e_double::RAW_MOUSE_Y, md[1]);

						float m[2];
						for (short p = 0; p < 2; p++) m[p] = static_cast<float>(md[p]);

						// transform based on cam
						if (auto psf = cameras.getMain(); psf) {

							ALLEGRO_TRANSFORM untransf = psf->getTransform();
							al_invert_transform(&untransf);
							al_transform_coordinates(&untransf, &m[0], &m[1]);

							for (short p = 0; p < 2; p++) {
								md[p] = static_cast<double>(m[p]);
							}

							db.set(database::e_double::MOUSE_X, md[0]);
							db.set(database::e_double::MOUSE_Y, md[1]);
						}
						else {
							md[0] = md[1] = 0.0;
							throw Abort::Abort(__FUNCSIG__, "Cannot work with mouse positioning without a camera set at pos 0!", Abort::abort_level::GIVEUP);
						}

						for (auto& i : sprites) {
							if (!i.getEnabled()) continue;
							if (i->isEq(sprite::e_boolean::FOLLOW_MOUSE, true)) {
								i->set(sprite::e_double::TARG_POSX, md[0]);
								i->set(sprite::e_double::TARG_POSY, md[1]);
							}
						}
					}

					// OTHER EVENTS (Allegro and stuff)

					else {
						auto ev = data.events_routine.routines.getEventRaw();

						switch (ev.type) {
						case ALLEGRO_EVENT_KEY_DOWN: // IN GAME
							db.key(ev.keyboard.keycode) = true;
							break;
						case ALLEGRO_EVENT_KEY_UP: // IN GAME
							db.key(ev.keyboard.keycode) = false;
							break;
						case ALLEGRO_EVENT_KEY_CHAR: // User input
							if (db.isEq(database::e_boolean::SAVING_STRING_INPUT, true)) { // LAST_STRING CURRENT_STRING
								if (ev.keyboard.unichar >= 32)
								{
									char multibyte[5] = { 0, 0, 0, 0, 0 };

									al_utf8_encode(multibyte, ev.keyboard.unichar <= 32 ? ' ' : ev.keyboard.unichar);
									char v = (char)strlen(multibyte);
									if (v > 4) throw Abort::Abort(__FUNCSIG__, "Got an exception on user input: invalid key code, couldn't translate to a valid string", Abort::abort_level::GIVEUP);

									auto& max_str_len = *db.getRef(database::e_sizet::MAXIMUM_STRING_INPUT_LEN);
									auto& strr = *db.getRef(database::e_string::CURRENT_STRING);

									for (auto& i : multibyte) {
										if (i) {
											if (strr.length() < max_str_len ? max_str_len : std::string::npos) strr += i; // limit
										}
									}
								}
								else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
								{
									auto& strr = *db.getRef(database::e_string::CURRENT_STRING);
									auto str_c = Tools::wideUp(strr);

									if (str_c.length() > 1) {
										str_c.pop_back();
										while (Tools::wideUp(strr) != str_c && strr.length() > 0) strr.pop_back();
									}
									else {
										strr.clear();
									}
								}
								else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
								{
									auto& strr = *db.getRef(database::e_string::CURRENT_STRING);
									auto& lstr = *db.getRef(database::e_string::LAST_STRING);
									lstr = strr;
									strr.clear();
								}
							}
							break;
						case ALLEGRO_EVENT_MOUSE_AXES:
							{
								double prop = 1.0;
								db.get(database::e_double::RESOLUTION_BUFFER_PROPORTION, prop);

								db.set(database::e_double::RAW_MOUSE_X, ev.mouse.x * prop);
								db.set(database::e_double::RAW_MOUSE_Y, ev.mouse.y * prop);
							}
							break;
						case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
							db.set(static_cast<database::e_boolean>(static_cast<int>(database::e_boolean::MOUSE_0) + ev.mouse.button > 7 ? 7 : ev.mouse.button), true);
							break;
						case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
							db.set(static_cast<database::e_boolean>(static_cast<int>(database::e_boolean::MOUSE_0) + ev.mouse.button > 7 ? 7 : ev.mouse.button), false);
							break;
						}

					}
				}
				catch (Abort::Abort err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [LSW], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eFrom: " << err.getWhereFrom() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.getDetails() << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eLevel: " << static_cast<int>(err.getLevel()) << L::ELF;
				}
				catch (std::exception err) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [STD], but this beta version won't handle any of this yet lmao." << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&cMore about the error: " << L::ELF;
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&6- &eDetails: " << err.what() << L::ELF;

				}
				catch (...) {
					logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "&4SOMETHING WENT WRONG [???], but this beta version won't handle any of this yet lmao." << L::ELF;
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
			Database db;

			// variables used here:

			if (data.functional_routine.initialize()) { // has to initialize (once)
				logg << L::SLF << fsr(__FUNCSIG__) << "Initializing Thread FUNCTIONAL..." << L::ELF;

				data.functional_routine.routines.start();
			}

			logg << L::SLF << fsr(__FUNCSIG__) << "Looping Thread FUNCTIONAL..." << L::ELF;

			while (keep()) {
				data.functional_routine.tick();

				while (data.functional_routine.isPaused()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					//Sleep(50); // 20 per sec
					data.functional_routine.tick(); // keep saying it's alive
				}

				data.functional_routine.routines.hasEventWait(); // LOOP_TRACK, UPDATE_MOUSE
				
				if (data.functional_routine.routines.isThisThis(static_cast<size_t>(core::thr_functional_routines::LOOP_TRACK))) {
					db.set(database::e_sizet::ADVANCEDFUNCSPERSECOND, data.functional_routine.routines.getNumCallsDefault());
				}
				else {
					auto ev = data.functional_routine.routines.getEventRaw();

					switch (ev.type) {
					case ALLEGRO_EVENT_TIMER:
						data.funcs_m.lock();
						/*for (auto& i : data.funcs) {
							if (i[ev.timer.source]) {
								if (auto j = i.getValue(); j) j();
								break;
							}
						}*/
						if (auto j = data.funcs[(uintptr_t)ev.timer.source]; j) (*j)();
						data.funcs_m.unlock();
						break;
					}
				}
				



				//logg << L::SLF << fsr(__FUNCSIG__) << "Thread &2" << thr_id << "&f looping." << L::ELF; // stop haha
				//al_rest(0.5);
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
			data.funcs.clear();
			data.gmute.lock();
			if (!data.has_init_once) {
				lsw_al_init_all();

				al_init_user_event_source(&data.evsrc);

				data.display_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::DRAWING, f); });
				data.collision_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::COLLIDING, f); });
				data.events_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::EVENTS, f); });
				data.functional_routine.this_core.setFunc([&](Threads::boolThreadF f)->int {return thr_base(core::thr_ids::FUNCTIONAL, f); });

				data.has_init_once = true;
			}
			data.gmute.unlock();
		}

		void Core::addFunction(const int id, const double delta_t, const std::function<void(void)> func)
		{
			/*for (auto& i : data.funcs) {
				if (i.first == id) {
					data.funcs_m.unlock();
					throw Abort::Abort(__FUNCSIG__, "Function already set!", Abort::abort_level::GIVEUP);
					return;
				}
			}*/
			data.funcs_m.lock();
			if (data.funcs[id] != nullptr) {
				data.funcs_m.unlock();
				throw Abort::Abort(__FUNCSIG__, "Function already set!", Abort::abort_level::GIVEUP);
				return;
			}
			ALLEGRO_TIMER* t = al_create_timer(delta_t);
			data.functional_routine.routines.insert(al_get_timer_event_source(t));
			al_start_timer(t);
			data.funcs.add({ func, id, (uintptr_t)t });
			data.funcs_m.unlock();
		}

		void Core::delFunction(const int id)
		{
			data.funcs_m.lock();
			auto firstt = data.funcs.begin();
			for (size_t p = 0; p < data.funcs.size(); p++) {
				auto& k = *(firstt + p);
				if (k[id] != nullptr) {
					data.funcs.erase(p);
					data.funcs_m.unlock();
					return;
				}
			}
			data.funcs_m.unlock();
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

		/*void Core::core_data::timed_function::work()
		{
			auto now = MILLI_NOW;
			if (now - lt > dt) {
				lt = now + dt;
				if (f) f();
			}
		}*/

	}
}