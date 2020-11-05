#pragma once

#include "../../Tools/SuperResource/superresource.h"
#include "../../Interface/Logger/logger.h"
#include "../../Interface/Config/config.h"
#include "../../Interface/Voice/voice.h"
#include "../../Interface/Mixer/mixer.h"
#include "../../Interface/Display/display.h"
// include layering


namespace LSW {
	namespace v5 {
		namespace Work {

			/// <summary>
			/// <para>Generates a string based on build date.</para>
			/// </summary>
			/// <returns>{std::string} A string used for versioning.</returns>
			inline const std::string __interpret_date();



			inline const std::string version_app = std::string("B") + __interpret_date();

			namespace gamecore {
				const std::string conf_versioning		= "versioning";
				const std::string conf_displaying		= "display";
				const std::string conf_audio			= "audio";
				const std::string conf_debug			= "debug";
				const std::string conf_mouse_memory		= "mouse"; // in memory only
				// add other common always used section names

				const double delta_checking_s = 5.0; // sec, check registered events like display
				const double delta_mouse_s = 1.0 / 120; // sec
			}

			/// <summary>
			/// <para>This is the quickest way to handle common tasks. Work classes will work with this.</para>
			/// <para>If you want custom, you can do the same creating a GameCore-like class via inheritance.</para>
			/// </summary>
			class GameCore {
			protected:
				struct shared {
					Interface::Logger logg;
					Interface::Config conf;
					Interface::Voice audio;
					Interface::Mixer mixer;
					Interface::Display display; // start this

					Interface::EventHandler events; // just display close event and mouse registration for now

					Interface::EventTimer check_sources; // like when you reload screen the source change.
					Interface::EventTimer update_mouse;
					bool _m_newdata = false;
					float _m_x, _m_y;

					Interface::Event latest_display_source; // copy

					bool closed = false; // if app is closed, true
					bool loaded = false; // loaded once
					Tools::SuperMutex m;
				};
				std::shared_ptr<shared> share = std::make_shared<shared>();
			public:
				/// <summary>
				/// <para>Start Logger and Config directly.</para>
				/// <para>As Logger is a global logging system, if it has been set earlier, this won't overwrite its settings.</para>
				/// </summary>
				/// <param name="{std::string}">Logger logging output.</param>
				/// <param name="{std::string}">Configuration path.</param>
				GameCore(const std::string&, const std::string&);

				/// <summary>
				/// <para>Copy reference constructor.</para>
				/// <para>Shutdown itself if anything is set.</para>
				/// </summary>
				/// <param name="{GameCore}">GameCore to reference.</param>
				GameCore(const GameCore&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// <para>Shutdown itself if anything is set.</para>
				/// </summary>
				/// <param name="{GameCore}">GameCore to move from.</param>
				GameCore(GameCore&&);

				/// <summary>
				/// <para>Ends all tasks and shutdown everything. You'll have to restart via constructor.</para>
				/// </summary>
				void shutdown();

				/// <summary>
				/// <para>Waits for display close or close app call.</para>
				/// </summary>
				void yield();

				/// <summary>
				/// <para>Is the app up?</para>
				/// </summary>
				/// <returns>{bool} True if it's not set as closed.</returns>
				bool running() const;


				/// <summary>
				/// <para>Copy reference operator.</para>
				/// <para>Shutdown itself if anything is set.</para>
				/// </summary>
				/// <param name="{GameCore}">GameCore to reference.</param>
				void operator=(const GameCore&);

				/// <summary>
				/// <para>Move operator.</para>
				/// <para>Shutdown itself if anything is set.</para>
				/// </summary>
				/// <param name="{GameCore}">GameCore to move from.</param>
				void operator=(GameCore&&);

				/// <summary>
				/// <para>Gets configuration.</para>
				/// </summary>
				/// <returns>{Config} Config here set.</returns>
				Interface::Config& get_config();

				/// <summary>
				/// <para>Gets main Mixer.</para>
				/// </summary>
				/// <returns>{Mixer} Main Mixer.</returns>
				Interface::Mixer& get_mixer();

				/// <summary>
				/// <para>Gets this core's display.</para>
				/// </summary>
				/// <returns>{Display} Display.</returns>
				Interface::Display& get_display();


				/// <summary>
				/// <para>Gets configuration.</para>
				/// </summary>
				/// <returns>{Config} Config here set.</returns>
				const Interface::Config& get_config() const;

				/// <summary>
				/// <para>Gets main Mixer.</para>
				/// </summary>
				/// <returns>{Mixer} Main Mixer.</returns>
				const Interface::Mixer& get_mixer() const;

				/// <summary>
				/// <para>Gets this core's display.</para>
				/// </summary>
				/// <returns>{Display} Display.</returns>
				const Interface::Display& get_display() const;
			};


			inline const std::string __interpret_date()
			{
				std::string date = __DATE__;
				std::string time = __TIME__;

				char monthr[16] = { '\0' };
				int year, monthn, day, hour, min, sec;

				sscanf_s(date.c_str(), "%s %d %d", monthr, 16, &day, &year);
				sscanf_s(time.c_str(), "%d:%d:%d", &hour, &min, &sec);

				std::string month = monthr;

				if (month == "Jan") {
					monthn = 1;
				}
				else if (month == "Feb") {
					monthn = 2;
				}
				else if (month == "Mar") {
					monthn = 3;
				}
				else if (month == "Apr") {
					monthn = 4;
				}
				else if (month == "May") {
					monthn = 5;
				}
				else if (month == "Jun") {
					monthn = 6;
				}
				else if (month == "Jul") {
					monthn = 7;
				}
				else if (month == "Aug") {
					monthn = 8;
				}
				else if (month == "Sep") {
					monthn = 9;
				}
				else if (month == "Oct") {
					monthn = 10;
				}
				else if (month == "Nov") {
					monthn = 11;
				}
				else if (month == "Dec") {
					monthn = 12;
				}
				else {
					monthn = 0;
				}

				char outtt[64];
				sprintf_s(outtt, "%04d%02d%02d%02d%02d", year, monthn, day, min, hour);

				return outtt;//std::to_string(year) + std::to_string(monthn) + std::to_string(day) + std::to_string(hour) + std::to_string(min);
			}
		}
	}
}