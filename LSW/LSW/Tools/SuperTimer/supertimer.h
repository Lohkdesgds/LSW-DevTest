#pragma once

#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"


namespace LSW {
	namespace v5 {
		namespace Tools {

			/*************************************************************************************

				# Advanced timer template:
				> Multiple times at once, automatic creation and fast use

			**************************************************************************************/

			template <size_t... Args>
			class SuperTimer {
				const unsigned d_t_t_avg = 10; // to template timers (internal)

				const size_t num_args = sizeof...(Args);
				const size_t calls_per_sec_pos = sizeof...(Args);

				size_t timers_t[sizeof...(Args)] = { Args... };
				ALLEGRO_TIMER* timers_p[sizeof...(Args)] = { nullptr };

				ALLEGRO_TIMER* eachsec = nullptr;
				double eachsec_doubleverif = 0;

				size_t benchmark[sizeof...(Args) + 2] = { 0 };
				double benchmark_m[sizeof...(Args) + 2] = { 0.0 };
				size_t rawcount[sizeof...(Args) + 2] = { 0 };
				double quickmark[sizeof...(Args) + 2][2] = { {0.0,0.0} }; // [0] -> diff since last call, [1] -> time itself


				ALLEGRO_EVENT_QUEUE* queue = nullptr;
				ALLEGRO_EVENT lastev = ALLEGRO_EVENT();


				void calQ(const size_t p) {
					auto tt = al_get_time();
					quickmark[p][0] = tt - quickmark[p][1];
					quickmark[p][1] = tt;
				}
			public:
				SuperTimer()
				{
					Handling::init_basic();

					queue = al_create_event_queue();

					eachsec = al_create_timer(1.0);
					al_register_event_source(queue, al_get_timer_event_source(eachsec));

					for (size_t p = 0; p < num_args; p++) {
						timers_p[p] = al_create_timer(1.0 / timers_t[p]);
						al_register_event_source(queue, al_get_timer_event_source(timers_p[p]));
					}
				}
				~SuperTimer()
				{
					stop();
					for (size_t p = 0; p < num_args; p++) al_destroy_timer(timers_p[p]);
					al_destroy_timer(eachsec);
					al_destroy_event_queue(queue);
				}
				// start timers and events
				void start()
				{
					for (size_t p = 0; p < num_args; p++) al_start_timer(timers_p[p]);
					al_start_timer(eachsec);
				}
				// stop timers and events
				void stop()
				{
					for (size_t p = 0; p < num_args; p++) al_stop_timer(timers_p[p]);
					al_stop_timer(eachsec);
				}
				// insert new event source
				void insert(ALLEGRO_EVENT_SOURCE* evs)
				{
					al_register_event_source(queue, evs);
				}
				// remove event source from list
				void remove(ALLEGRO_EVENT_SOURCE* evs)
				{
					al_unregister_event_source(queue, evs);
				}
				// how many calls per second
				size_t get_report(const size_t t)
				{
					assert(t < num_args);
					return benchmark[t];
				}
				// delta t instant in seconds
				double get_instant_report(const size_t t)
				{
					assert(t < num_args);
					return quickmark[t][0];
				}
				// expected times per second to run
				size_t get_report_expected(const size_t t)
				{
					assert(t < num_args);
					return timers_t[t];
				}
				// total calls this second
				size_t get_num_full_calls()
				{
					return benchmark[calls_per_sec_pos];
				}
				// delta t last event in seconds
				double get_instant_report_all()
				{
					return quickmark[calls_per_sec_pos][0];
				}
				// how many non-timer calls per second
				size_t get_report_no_timer()
				{
					return benchmark[calls_per_sec_pos + 1];
				}
				// delta t non timer events
				double get_report_instant_no_timer() // if you wait, the amount of non-internal events (aka not internal timer). If you don't, every not-event return and local events
				{
					return quickmark[calls_per_sec_pos + 1][0];
				}
				// waits for new event
				void has_event_wait()
				{

					for (bool can_leave = false; !can_leave;) {
						rawcount[calls_per_sec_pos]++;
						calQ(calls_per_sec_pos);

						al_wait_for_event(queue, &lastev);

						for (size_t u = 0; u < num_args; u++) {
							if (is_this_it(u)) {
								rawcount[u]++;
								calQ(u);
							}
						}

						if (lastev.type == ALLEGRO_EVENT_TIMER && lastev.timer.source == eachsec) {

							bool throw_warn = false;

							double timee = al_get_time();
							if (eachsec_doubleverif == 0) eachsec_doubleverif = timee - 1.0;
							double diff = timee - eachsec_doubleverif;

							if (diff > 5.0) {
								al_flush_event_queue(queue); // there's something lagging, so clear and refresh
								// cast warn on log?
								eachsec_doubleverif = timee - 1.0;

								throw_warn = true;
								/*gfile logg;
								logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "Can't keep up! Somewhere is having some trouble keeping the loops! Running " << (diff - 1.0) << " second(s) behind." << L::ELF;*/
								//printf_s("[WARN] Can't keep up! Somewhere is having some trouble keeping the loops!\n");
							}

							eachsec_doubleverif += 1.0;

							for (size_t u = 0; u < num_args + 2; u++) { // + 1 because calls_per_sec_pos is one ahead
								benchmark[u] = rawcount[u];
								benchmark_m[u] = ((d_t_t_avg - 1.0) / d_t_t_avg) * benchmark_m[u] + (1.0 / d_t_t_avg) * benchmark[u];
								rawcount[u] = 0;
							}

							if (throw_warn)
							{
								std::string details;
								for (size_t p = 0; p < num_args; p++) details += std::to_string(timers_t[p]) + "@" + std::to_string(benchmark_m[p]) + "; ";

								throw Handling::Abort(__FUNCSIG__, "Can't keep up! Somewhere is having some trouble keeping the loops! Running " + std::to_string(diff - 1.0) + " second(s) behind. - Detailed debug: " + details, Handling::abort::abort_level::GIVEUP);
							}
						}
						else {
							can_leave = true;
							rawcount[calls_per_sec_pos + 1]++; // now has event outside, go
							calQ(calls_per_sec_pos + 1);
						}
					}
				}
				// returns true if there's an event
				bool has_event()
				{
					rawcount[calls_per_sec_pos]++;
					calQ(calls_per_sec_pos);

					if (al_get_next_event(queue, &lastev)) {

						for (size_t u = 0; u < num_args; u++) {
							if (is_this_it(u)) {
								rawcount[u]++;
								calQ(u);
							}
						}

						if (lastev.type == ALLEGRO_EVENT_TIMER && lastev.timer.source == eachsec) {

							bool throw_warn = false;

							double timee = al_get_time();
							if (eachsec_doubleverif == 0) eachsec_doubleverif = timee - 1.0;
							double diff = timee - eachsec_doubleverif;

							if (diff > 5.0) {
								al_flush_event_queue(queue); // there's something lagging, so clear and refresh
								// cast warn on log?

								//gfile logg;

								eachsec_doubleverif = timee - 1.0;
								throw_warn = true;
								//logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "Can't keep up! Somewhere is having some trouble keeping the loops! Running " << (diff - 1.0) << " second(s) behind." << L::ELF;
							}

							eachsec_doubleverif += 1.0;

							for (size_t u = 0; u < num_args + 2; u++) { // + 1 because calls_per_sec_pos is one ahead
								benchmark[u] = rawcount[u];
								benchmark_m[u] = ((d_t_t_avg - 1.0) / d_t_t_avg) * benchmark_m[u] + (1.0 / d_t_t_avg) * benchmark[u];
								rawcount[u] = 0;
							}


							if (throw_warn)
							{
								std::string details;
								for (size_t p = 0; p < num_args; p++) details += std::to_string(timers_t[p]) + "@" + std::to_string(benchmark_m[p]) + "; ";

								throw Handling::Abort(__FUNCSIG__, "Can't keep up! Somewhere is having some trouble keeping the loops! Running " + std::to_string(diff - 1.0) + " second(s) behind. - Detailed debug: " + details, Handling::abort::abort_level::GIVEUP);
							}

							return false;
						}
						rawcount[calls_per_sec_pos + 1]++; // not timer event, so count
						calQ(calls_per_sec_pos + 1);
						return true;
					}

					rawcount[calls_per_sec_pos + 1]++; // has no event, so count
					calQ(calls_per_sec_pos + 1);
					return false;
				}
				// check if event was a specific internal timer
				bool is_this_it(const size_t t)
				{
					if (!(lastev.type == ALLEGRO_EVENT_TIMER)) return false;
					return (lastev.timer.source == timers_p[t]);
				}
				// get raw event for other event types
				ALLEGRO_EVENT get_last_event_raw()
				{
					return lastev;
				}
			};
		}
	}
}