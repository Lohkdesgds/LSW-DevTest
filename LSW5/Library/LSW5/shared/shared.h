#pragma once

// C++
#include <string>

namespace LSW {
	namespace v5 {
		namespace Shared {

			// this have to be global, come on																																* 100 eq 1.0							      * 1000 eq 1.0
			enum class my_events { CUSTOM_EVENT_LOG_STRING = 512, /*CUSTOM_EVENT_SHARING_NEW_DISPLAY_SIZE,*/ CUSTOM_EVENT_CALL_FULLSCREEN, CUSTOM_EVENT_EXTERNAL_EXIT_CALL, CUSTOM_EVENT_DISPLAY_UPDATE_RESOLUTION_SCALE, CUSTOM_EVENT_DISPLAY_CHROMA_FX /* <-- TO BE DONE */ };

			/* VERSIONING */
			inline const std::string __interpret_date();
			inline const std::string version_app = std::string("B") + __interpret_date();


			// very useful stuff
			constexpr size_t NoSolution = static_cast<size_t>(-1);

			// game timing
			const int game_timing_tps = 10;

			// not actually template, but shared
			struct super_char {
				std::string ch;
				size_t size() { return ch.length(); }
				super_char& operator=(char a) { super_char c; c = a; return c; }
				super_char& operator=(const char* s) { super_char c; c = s; return c; }
				super_char& operator=(const std::string s) { super_char c; c = s; return c; }
			};
			using super_string = std::basic_string<super_char>; // multibyte allegro stuff lmao

			// don't use this too much!
			template<typename T> inline T& nullReference() { void* nil = nullptr; return (T&)*(int*)nil; }

			// shh 
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