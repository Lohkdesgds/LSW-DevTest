#pragma once

namespace LSW {
	namespace v5 {
		namespace Work {

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

			template<size_t u>
			GameCoreT<u>::GameCoreT(const std::string& a, const std::string& b) : GameCore(true)
			{
				auto sr = std::make_shared<GameCore::shared>();
				sr->display = Interface::generate_display<u>();
				sr->_force_logg_path = a;
				sr->_force_conf_path = b;
				sr->_force_after_load = true;
				_assert(std::move(sr));
			}

			template<size_t u>
			GameCore generate_gamecore(const std::string& lo, const std::string& co)
			{
				GameCoreT<u> ho(lo, co); // set Display by itself
				return std::move(ho); // no worry about forgetting the virtual func.
			}
		}
	}
}