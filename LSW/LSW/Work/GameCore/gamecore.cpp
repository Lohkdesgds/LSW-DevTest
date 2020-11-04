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
						share->logg.debug(ab, Interface::E::WARN);
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
					share->conf.ensure(gamecore::conf_displaying,	"force_refresh_rate",	0,		Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"limit_framerate_to",	0,		Interface::config::config_section_mode::SAVE);
					share->conf.ensure(gamecore::conf_displaying,	"hidemouse",			true,	Interface::config::config_section_mode::SAVE);

					// audio settings
					share->conf.ensure(gamecore::conf_audio,		"volume",				0.5f,	Interface::config::config_section_mode::SAVE);


					share->conf.flush();

					// - - - - START - - - - //

					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "screen_width"); k != 0)			share->display.set_width(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "screen_height"); k != 0)		share->display.set_height(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "display_flags"); k != 0)		share->display.set_new_flags(k);
					if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "force_refresh_rate"); k != 0)	share->display.set_new_refresh_rate(k);
					//if (auto k = share->conf.get_as<int>(gamecore::conf_displaying, "limit_framerate_to"); k != 0)	share->display.set_fps_cap(k); // to do
					if (auto k = share->conf.get_as<bool>(gamecore::conf_displaying, "hidemouse"); k != 0)			share->display.hide_mouse(k);

					share->display.init();

					while (!share->display.display_ready()) std::this_thread::sleep_for(std::chrono::milliseconds(250));

					share->events.add(share->display.get_event_source());
					share->events.add(Interface::get_mouse_event());
					share->events.set_run_autostart([&](const Interface::RawEvent& ev) {
						switch (ev.type())
						{
						case ALLEGRO_EVENT_MOUSE_AXES:
						{
							float x, y;
							x = ev.mouse_event().x;
							y = ev.mouse_event().y;
							if (auto camnow = share->display.get_current_camera(); camnow) {
								Interface::Camera inv = *camnow;
								inv.invert();
								inv.transform(x, y);
								//logg << L::SL << fsr(__FUNCSIG__) << "Mouse axis: [" << ev.mouse_event().x << ";" << ev.mouse_event().y << "] " << FormatAs("%.4f") << x << ";" << FormatAs("%.4f") << y << L::EL;

								share->conf.set(gamecore::conf_mouse_memory, Interface::config::config_section_mode::MEMORY_ONLY);
								share->conf.set(gamecore::conf_mouse_memory, "x", x);
								share->conf.set(gamecore::conf_mouse_memory, "y", y);
							}
						}
						break;
						case ALLEGRO_EVENT_DISPLAY_CLOSE:
						{
							Interface::Logger logg;
							logg.debug("Close app called.");

							share->closed = true;
						}
							break;
						}
						});

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

				share->conf.set(gamecore::conf_audio, "volume", share->mixer.get_gain());
				share->mixer.mute(true);

				share->conf.set(gamecore::conf_displaying, "screen_width", share->display.get_width());
				share->conf.set(gamecore::conf_displaying, "screen_height", share->display.get_height());
				share->conf.set(gamecore::conf_displaying, "display_flags", share->display.get_flags());

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