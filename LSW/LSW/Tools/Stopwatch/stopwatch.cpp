#include "stopwatch.h"

namespace LSW {
	namespace v5 {
		namespace Tools {



			void Stopwatch::prepare(const size_t res)
			{
				stops.resize(res);
				reserved = res;
			}

			void Stopwatch::start()
			{
				point = 0;
				click_one();
			}

			void Stopwatch::click_one()
			{
				if (point < reserved) stops[point++] = std::chrono::high_resolution_clock::now();
			}

			std::chrono::nanoseconds Stopwatch::get_nanosec_between(const size_t a, const size_t b)
			{
				if (a >= reserved || b >= reserved) return std::chrono::nanoseconds(0);
				return std::chrono::duration_cast<std::chrono::nanoseconds>(stops[b] - stops[a]);
			}

			std::chrono::microseconds Stopwatch::get_microsec_between(const size_t a, const size_t b)
			{
				if (a >= reserved || b >= reserved) return std::chrono::microseconds(0);
				return std::chrono::duration_cast<std::chrono::microseconds>(stops[b] - stops[a]);
			}

			std::chrono::milliseconds Stopwatch::get_millisec_between(const size_t a, const size_t b)
			{
				if (a >= reserved || b >= reserved) return std::chrono::milliseconds(0);
				return std::chrono::duration_cast<std::chrono::milliseconds>(stops[b] - stops[a]);
			}

			std::chrono::seconds Stopwatch::get_seconds_between(const size_t a, const size_t b)
			{
				if (a >= reserved || b >= reserved) return std::chrono::seconds(0);
				return std::chrono::duration_cast<std::chrono::seconds>(stops[b] - stops[a]);
			}

			std::string Stopwatch::get_string_between(const size_t a, const size_t b)
			{
				if (a >= reserved || b >= reserved) return "";
				std::string gen;

				long long sec = get_seconds_between(a, b).count();
				long long ms = get_millisec_between(a, b).count();
				long long us = get_microsec_between(a, b).count();
				long long ns = get_nanosec_between(a, b).count();

				if (sec > 999) {
					gen = Tools::sprintf_a("%lld s", sec);
				}
				else if (sec > 99) {
					gen = Tools::sprintf_a("%lld.%lld s", sec, ms % 10);
				}
				else if (sec > 9) {
					gen = Tools::sprintf_a("%lld.%lld s", sec, ms % 100);
				}
				else if (sec > 0) {
					gen = Tools::sprintf_a("%lld.%lld s", sec, ms % 1000);
				}
				else if (ms > 99) { // ms must be <  1000
					gen = Tools::sprintf_a("%lld.%lld ms", ms, us % 10);
				}
				else if (ms > 9) {
					gen = Tools::sprintf_a("%lld.%lld ms", ms, us % 100);
				}
				else if (ms > 0) {
					gen = Tools::sprintf_a("%lld.%lld ms", ms, us % 1000);
				}
				else if (us > 99) { // ms must be <  1000
					gen = Tools::sprintf_a("%lld.%lld us", us, ns % 10);
				}
				else if (us > 9) {
					gen = Tools::sprintf_a("%lld.%lld us", us, ns % 100);
				}
				else if (us > 0) {
					gen = Tools::sprintf_a("%lld.%lld us", us, ns % 1000);
				}
				else {
					gen = Tools::sprintf_a("%04lld ns", ns);
				}
				return std::move(gen);
			}

			size_t Stopwatch::last_point_valid()
			{
				return reserved ? reserved - 1 : 0;
			}

			std::string Stopwatch::generate_table()
			{
				std::string gen;

				for (size_t p = 0; p < reserved - 1; p++)
				{
					gen += "[" + Tools::sprintf_a("%02zu", p) + " -> " + Tools::sprintf_a("%02zu", p+1) + "]: " + get_string_between(p, p+1) + "\n";
				}
				if (gen.length()) gen.pop_back(); // '\n'

				return std::move(gen);
			}

		}
	}
}