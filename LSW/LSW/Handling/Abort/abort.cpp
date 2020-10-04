#include "abort.h"

namespace LSW {
	namespace v5 {
		namespace Handling {


			Abort::Abort(std::string fw, std::string dt, abort::abort_level lv)
			{
				s_from_where = fw;
				s_details = dt;
				abort_level = lv;
				std::exception::exception(("From: " + s_from_where + ";\nDetails: " + s_details + ";\nLevel: " + std::to_string(static_cast<int>(abort_level))).c_str(), static_cast<int>(abort_level));
			}
			Abort::Abort(const Abort& abor)
			{
				s_from_where = abor.s_from_where;
				s_details = abor.s_details;
				abort_level = abor.abort_level;
				std::exception::exception(("From: " + s_from_where + ";\nDetails: " + s_details + ";\nLevel: " + std::to_string(static_cast<int>(abort_level))).c_str(), static_cast<int>(abort_level));
			}
			const std::string& Abort::get_from() const
			{
				return s_from_where;
			}
			const std::string& Abort::get_details() const
			{
				return s_details;
			}
			const abort::abort_level& Abort::get_level() const
			{
				return abort_level;
			}

			Abort::operator std::string() const
			{
				std::string lvl;
				switch (get_level()) {
				case abort::abort_level::FATAL_ERROR:
					lvl = "FATAL_ERROR";
					break;
				case abort::abort_level::GIVEUP:
					lvl = "GIVEUP";
					break;
				case abort::abort_level::OTHER:
					lvl = "OTHER";
					break;
				}
				return "From: " + get_from() + ";\nDetails: " + get_details() + ";\nLevel: " + lvl;
			}


			AutoAbort::AutoAbort(const unsigned dt, std::function<void(void)> f)
			{
				t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + std::chrono::milliseconds(dt);
				call_death = f;
				autodrop = [&]() {
					auto cpy = (t.count() < 1000 ? 100 : t.count() / 50);
					cpy = cpy > 250 ? 250 : cpy;
					while (std::chrono::system_clock::now().time_since_epoch() < t) {
						if (drop) return;
						std::this_thread::sleep_for(std::chrono::milliseconds(cpy));
					}
					if (drop) return;
					if (call_death) call_death();
					exit(EXIT_FAILURE);
				};
				thr = std::thread(autodrop);
			}
			void AutoAbort::abort()
			{
				drop = true;
				if (thr.joinable()) thr.join();
			}


			std::string operator+(const Abort& a) {
				return static_cast<std::string>(a);
			}
		}
	}
}