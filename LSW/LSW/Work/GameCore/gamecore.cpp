#include "gamecore.h"

namespace LSW {
	namespace v5 {
		namespace Work {



			GameCore::GameCore(const std::string& logg_p, const std::string& conf_p)
			{
				Tools::AutoLock luck(share->m);
				if (!share->loaded) {
					share->closed = false;
					share->loaded = true;

					// - - - - IMPORTANT LOAD - - - - //

					try {
						share->logg.init(logg_p);
					}
					catch (const Handling::Abort& ab) {
						debug(std::string(ab));
					}

					if (!share->conf.load(conf_p)) {
						share->conf.save_path(conf_p);
					}

					share->conf.set(gamecore::conf_versioning, Interface::config::config_section_mode::SAVE);
					share->conf.set(gamecore::conf_versioning, "automatic version", version_app);

					// display settings
					share->conf.ensure(gamecore::conf_displaying,	"screen_width",			1280,	Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"screen_height",		720,	Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"display_flags",		0,		Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"limit_framerate_to",	0,		Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"hidemouse",			true,	Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"vsync",				false,	Interface::config::config_section_mode::SAVE);

					// audio settings
					share->conf.ensure(gamecore::conf_audio,		"volume",				0.5f,	Interface::config::config_section_mode::SAVE);

					// debug settings
					share->conf.ensure(gamecore::conf_debug,		"debug_to_file",		true,	Interface::config::config_section_mode::SAVE);

					// mouse (memory) ### TIED TO SPRITE!
					share->conf.ensure(gamecore::conf_mouse_memory, "x",					0.0f,	Interface::config::config_section_mode::MEMORY_ONLY);
					share->conf.ensure(gamecore::conf_mouse_memory, "y",					0.0f,	Interface::config::config_section_mode::MEMORY_ONLY);
					share->conf.ensure(gamecore::conf_mouse_memory, "rx",					0.0f,	Interface::config::config_section_mode::MEMORY_ONLY);
					share->conf.ensure(gamecore::conf_mouse_memory, "ry",					0.0f,	Interface::config::config_section_mode::MEMORY_ONLY);
					share->conf.ensure(gamecore::conf_mouse_memory, "press_count",			0u,		Interface::config::config_section_mode::MEMORY_ONLY);
					share->conf.ensure(gamecore::conf_mouse_memory, "down_latest",			-1,		Interface::config::config_section_mode::MEMORY_ONLY);
					// set "b0" ... as mouse buttons (bool)


					share->conf.flush();

					// - - - - START - - - - //

					share->logg.debug_write_to_file(share->conf.get_as<bool>(gamecore::conf_debug, "debug_to_file"));

					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "screen_width"); k != 0)		share->display.set_width(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "screen_height"); k != 0)		share->display.set_height(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "display_flags"); k != 0)		share->display.set_new_flags(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "limit_framerate_to"); k != 0)	share->display.set_fps_cap(k); // to do

					share->display.hide_mouse(share->conf.get_as<bool>(gamecore::conf_displaying, "hidemouse"));
					share->display.set_vsync(share->conf.get_as<bool>(gamecore::conf_displaying, "vsync"));


					share->display.init();

					while (!share->display.display_ready()) std::this_thread::sleep_for(std::chrono::milliseconds(250));

					share->check_sources.set_delta(gamecore::delta_checking_s);
					share->update_mouse.set_delta(gamecore::delta_mouse_s);
					share->latest_display_source = share->display.get_event_source();

					// SHHHH share->events.set_mode(Interface::eventhandler::handling_mode::NO_BUFFER_SKIP);
					share->events.add(share->check_sources);
					share->events.add(share->update_mouse);
					share->events.add(share->latest_display_source);
					share->events.add(Interface::get_mouse_event());
					share->events.set_run_autostart([&](const Interface::RawEvent& ev) {
						switch (ev.type())
						{
						case ALLEGRO_EVENT_TIMER:
						{
							if (ev.timer_event().source == share->update_mouse && share->_m_newdata) {
								share->_m_newdata = false;
								float x = share->_m_x;
								float y = share->_m_y;
								float rx = share->_m_x;
								float ry = share->_m_y;
								if (auto camnow = share->display.get_current_camera(); camnow) {
									Interface::Camera inv = *camnow;
									inv.invert();
									inv.transform(x, y);
									Interface::Camera cninv = *camnow;
									cninv.classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);
									cninv.invert();
									cninv.transform(rx, ry);
									//logg << L::SL << fsr() << "Mouse axis: [" << ev.mouse_event().x << ";" << ev.mouse_event().y << "] " << FormatAs("%.4f") << x << ";" << FormatAs("%.4f") << y << L::EL;

									share->conf.set(gamecore::conf_mouse_memory, Interface::config::config_section_mode::MEMORY_ONLY);
									share->conf.set(gamecore::conf_mouse_memory, "x", x);
									share->conf.set(gamecore::conf_mouse_memory, "y", y);
									share->conf.set(gamecore::conf_mouse_memory, "rx", rx);
									share->conf.set(gamecore::conf_mouse_memory, "ry", ry);
								}
							}
							else if (ev.timer_event().source == share->check_sources) {
								if (share->latest_display_source != share->display.get_event_source()) {
									Interface::Logger logg;

									debug("Display event registering refresh! Display has changed/updated.");

									share->events.remove(share->latest_display_source);
									share->latest_display_source = share->display.get_event_source();
									share->events.add(share->latest_display_source);
								}
							}
						}
							break;
						case ALLEGRO_EVENT_MOUSE_AXES:
						{
							share->_m_x = ev.mouse_event().x;
							share->_m_y = ev.mouse_event().y;
							share->_m_newdata = true;
						}
							break;
						case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
						{
							std::string bb = "b" + std::to_string(ev.mouse_event().button);
							share->conf.set(gamecore::conf_mouse_memory, "down_latest", (int)(1 + ev.mouse_event().button));
							auto _press = share->conf.get_as<unsigned>(gamecore::conf_mouse_memory, "press_count");
							share->conf.set(gamecore::conf_mouse_memory, "press_count", ++_press);
							share->conf.set(gamecore::conf_mouse_memory, bb, true);
						}
							break;
						case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
						{
							std::string bb = "b" + std::to_string(ev.mouse_event().button);
							share->conf.set(gamecore::conf_mouse_memory, "down_latest", -(int)(1 + ev.mouse_event().button));
							auto _press = share->conf.get_as<unsigned>(gamecore::conf_mouse_memory, "press_count");
							share->conf.set(gamecore::conf_mouse_memory, "press_count", _press ? (_press - 1) : 0u);
							share->conf.set(gamecore::conf_mouse_memory, bb, true);
						}
							break;
						case ALLEGRO_EVENT_DISPLAY_CLOSE:
						{
							Interface::Logger logg;
							debug("Close app called.");

							share->closed = true;
						}
							break;
						}
						});


					share->update_mouse.start();
					share->check_sources.start();


					if (!share->audio.load() || !share->mixer.load()) throw Handling::Abort(__FUNCSIG__, "Cannot setup AUDIO!");

					share->mixer.attach_to(share->audio);

					if (auto k = share->conf.get_as<float>(gamecore::conf_audio, "volume"); k >= 0.0)	share->mixer.set_gain(k);
				}
			}

			GameCore::GameCore(const GameCore& b)
			{
				*this = b;
			}

			GameCore::GameCore(GameCore&& m)
			{
				*this = std::move(m);
			}

			void GameCore::shutdown()
			{
				// ensure saving res etc
				Tools::AutoLock luck(share->m);

				if (!share->loaded) return;

				share->events.reset();
				share->check_sources.stop();
				share->update_mouse.stop();

				share->conf.set(gamecore::conf_audio, "volume", share->mixer.get_gain());
				share->mixer.mute(true);

				share->conf.set(gamecore::conf_displaying, "screen_width", share->display.get_width());
				share->conf.set(gamecore::conf_displaying, "screen_height", share->display.get_height());
				share->conf.set(gamecore::conf_displaying, "display_flags", share->display.get_flags());
				share->conf.set(gamecore::conf_displaying, "limit_framerate_to", share->display.get_fps_cap());
				share->conf.set(gamecore::conf_displaying, "vsync", share->display.get_vsync());

				share->conf.flush();

				share->display.deinit();

				share->closed = true;
				share->loaded = false;
			}

			void GameCore::yield()
			{
				while (!share->closed) std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}

			bool GameCore::running() const
			{
				return !share->closed;
			}

			void GameCore::operator=(const GameCore& b)
			{
				if (!share->closed) shutdown();
				share = b.share;
			}

			void GameCore::operator=(GameCore&& m)
			{
				if (!share->closed) shutdown();
				share = std::move(m.share);
			}

			Interface::Config& GameCore::get_config()
			{
				return share->conf;
			}

			Interface::Mixer& GameCore::get_mixer()
			{
				return share->mixer;
			}

			Interface::Display& GameCore::get_display()
			{
				return share->display;
			}

			const Interface::Config& GameCore::get_config() const
			{
				return share->conf;
			}

			const Interface::Mixer& GameCore::get_mixer() const
			{
				return share->mixer;
			}

			const Interface::Display& GameCore::get_display() const
			{
				return share->display;
			}

		}
	}
}