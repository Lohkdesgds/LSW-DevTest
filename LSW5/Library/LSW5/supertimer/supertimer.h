#pragma once

// C
#define _CRT_RAND_S
// Others
#include "..\allegroinitialization\allegroinitialization.h"


namespace LSW {
	namespace v5 {
		
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
				lsw_init();

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
			void start()
			{
				for (size_t p = 0; p < num_args; p++) al_start_timer(timers_p[p]);
				al_start_timer(eachsec);
			}
			void stop()
			{
				for (size_t p = 0; p < num_args; p++) al_stop_timer(timers_p[p]);
				al_stop_timer(eachsec);
			}
			void insert(ALLEGRO_EVENT_SOURCE* evs)
			{
				al_register_event_source(queue, evs);
			}
			void remove(ALLEGRO_EVENT_SOURCE* evs)
			{
				al_unregister_event_source(queue, evs);
			}
			size_t getReport(const size_t t)
			{
				assert(t < num_args);
				return benchmark[t];
			}
			double getInstantSReport(const size_t t)
			{
				assert(t < num_args);
				return quickmark[t][0];
			}
			size_t getReportExpected(const size_t t)
			{
				assert(t < num_args);
				return timers_t[t];
			}
			size_t getNumFullCalls() // this is EVERY SINGLE event being watched
			{
				return benchmark[calls_per_sec_pos];
			}
			double getNumFullInstantSCalls()
			{
				return quickmark[calls_per_sec_pos][0];
			}
			size_t getNumCallsDefault() // if you wait, the amount of non-internal events (aka not internal timer). If you don't, every not-event return and local events
			{
				return benchmark[calls_per_sec_pos + 1];
			}
			double getNumInstantSCallsDefault() // if you wait, the amount of non-internal events (aka not internal timer). If you don't, every not-event return and local events
			{
				return quickmark[calls_per_sec_pos][0];
			}
			void hasEventWait()
			{

				for (bool can_leave = false; !can_leave;) {
					rawcount[calls_per_sec_pos]++;
					calQ(calls_per_sec_pos);

					al_wait_for_event(queue, &lastev);

					for (size_t u = 0; u < num_args; u++) {
						if (isThisThis(u)) {
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

							throw Abort::Abort(__FUNCSIG__, "Can't keep up! Somewhere is having some trouble keeping the loops! Running " + std::to_string(diff - 1.0) + " second(s) behind. - Detailed debug: " + details, Abort::abort_level::GIVEUP);
						}
					}
					else {
						can_leave = true;
						rawcount[calls_per_sec_pos + 1]++; // now has event outside, go
						calQ(calls_per_sec_pos + 1);
					}
				}
			}
			bool hasEvent()
			{
				rawcount[calls_per_sec_pos]++;
				calQ(calls_per_sec_pos);

				if (al_get_next_event(queue, &lastev)) {

					for (size_t u = 0; u < num_args; u++) {
						if (isThisThis(u)) {
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

							throw Abort::Abort(__FUNCSIG__, "Can't keep up! Somewhere is having some trouble keeping the loops! Running " + std::to_string(diff - 1.0) + " second(s) behind. - Detailed debug: " + details, Abort::abort_level::GIVEUP);
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
			bool isThisThis(const size_t t)
			{
				if (!(lastev.type == ALLEGRO_EVENT_TIMER)) return false;
				return (lastev.timer.source == timers_p[t]);
			}
			ALLEGRO_EVENT getEventRaw()
			{
				return lastev;
			}
		};

	}
}