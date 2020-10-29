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

			/// <summary>
			/// <para>An advanced combo of timers within one event handler.</para>
			/// <para>If you need a lot of things synced with time, but you don't want many variables around, use this.</para>
			/// <para>It does support adding new non-timer events (or any event, actually), but you have to use raw event to manage them.</para>
			/// </summary>
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


				/// <summary>
				/// <para>Updates times and last call for a specific timer.</para>
				/// </summary>
				/// <param name="{size_t}">The timer one.</param>
				void calQ(const size_t);
			public:
				SuperTimer();
				~SuperTimer();
				/// <summary>
				/// <para>Start timers and events.</para>
				/// </summary>
				void start();

				/// <summary>
				/// <para>Stop timers and events.</para>
				/// </summary>
				void stop();

				/// <summary>
				/// <para>Insert new event source.</para>
				/// </summary>
				/// <param name="{RAW EVENT SOURCE}">A new event source.</param>
				void _insert(ALLEGRO_EVENT_SOURCE*);

				/// <summary>
				/// <para>Remove event source from list.</para>
				/// </summary>
				/// <param name="{RAW EVENT SOURCE}">Event source to remove.</param>
				void _remove(ALLEGRO_EVENT_SOURCE*);

				/// <summary>
				/// <para>How many calls per second.</para>
				/// </summary>
				/// <param name="{size_t}">What time statistics you want?</param>
				/// <returns>{size_t} Number of calls in one second.</returns>
				size_t get_report(const size_t);
				

				/// <summary>
				/// <para>Delta t instant in seconds.</para>
				/// </summary>
				/// <param name="{size_t}">What time statistics you want?</param>
				/// <returns>{double} Delta in seconds.</returns>
				double get_instant_report(const size_t);
				

				/// <summary>
				/// <para>Expected times per second to run.</para>
				/// </summary>
				/// <param name="{size_t}">What time statistics you want?</param>
				/// <returns>{size_t} Number of calls expected in one second.</returns>
				size_t get_report_expected(const size_t);

				/// <summary>
				/// <para>Total calls this second.</para>
				/// </summary>
				/// <returns>{size_t} Number of calls in one second.</returns>
				size_t get_num_full_calls();

				/// <summary>
				/// <para>Delta t last event in seconds.</para>
				/// </summary>
				/// <returns>{double} Delta in seconds.</returns>
				double get_instant_report_all();

				/// <summary>
				/// <para>how many non-timer calls per second (external added).</para>
				/// </summary>
				/// <returns>{size_t} Number of calls in one second.</returns>
				size_t get_report_not_timer();

				/// <summary>
				/// <para>Delta t non-timer events.</para>
				/// </summary>
				/// <returns>{double} Delta in seconds.</returns>
				double get_report_instant_not_timer();

				/// <summary>
				/// <para>Waits for new event.</para>
				/// </summary>
				void has_event_wait();

				/// <summary>
				/// <para>Checks if has new event.</para>
				/// </summary>
				/// <returns>{bool} True if has events.</returns>
				bool has_event();

				/// <summary>
				/// <para>Directly checks if event was a internal timer.</para>
				/// </summary>
				/// <param name="{size_t}">Timer index to check.</param>
				/// <returns>{bool} True if event source is from this timer.</returns>
				bool is_this_it(const size_t);

				/// <summary>
				/// <para>Gets the RAW event.</para>
				/// </summary>
				/// <returns>{RAW EVENT} Raw event itself.</returns>
				ALLEGRO_EVENT get_last_event_raw();
			};



			template<size_t ...Args>
			void SuperTimer<Args...>::calQ(const size_t p) {
				auto tt = al_get_time();
				quickmark[p][0] = tt - quickmark[p][1];
				quickmark[p][1] = tt;
			}

			template<size_t ...Args>
			SuperTimer<Args...>::SuperTimer()
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

			template<size_t ...Args>
			SuperTimer<Args...>::~SuperTimer()
			{
				stop();
				for (size_t p = 0; p < num_args; p++) al_destroy_timer(timers_p[p]);
				al_destroy_timer(eachsec);
				al_destroy_event_queue(queue);
			}

			template<size_t ...Args>
			void SuperTimer<Args...>::start()
			{
				for (size_t p = 0; p < num_args; p++) al_start_timer(timers_p[p]);
				al_start_timer(eachsec);
			}

			template<size_t ...Args>
			void SuperTimer<Args...>::stop()
			{
				for (size_t p = 0; p < num_args; p++) al_stop_timer(timers_p[p]);
				al_stop_timer(eachsec);
			}

			template<size_t ...Args>
			void SuperTimer<Args...>::_insert(ALLEGRO_EVENT_SOURCE* evs)
			{
				al_register_event_source(queue, evs);
			}

			template<size_t ...Args>
			void SuperTimer<Args...>::_remove(ALLEGRO_EVENT_SOURCE* evs)
			{
				al_unregister_event_source(queue, evs);
			}

			template<size_t ...Args>
			size_t SuperTimer<Args...>::get_report(const size_t t)
			{
				assert(t < num_args);
				return benchmark[t];
			}

			template<size_t ...Args>
			double SuperTimer<Args...>::get_instant_report(const size_t t)
			{
				assert(t < num_args);
				return quickmark[t][0];
			}

			template<size_t ...Args>
			size_t SuperTimer<Args...>::get_report_expected(const size_t t)
			{
				assert(t < num_args);
				return timers_t[t];
			}

			template<size_t ...Args>
			size_t SuperTimer<Args...>::get_num_full_calls()
			{
				return benchmark[calls_per_sec_pos];
			}

			template<size_t ...Args>
			double SuperTimer<Args...>::get_instant_report_all()
			{
				return quickmark[calls_per_sec_pos][0];
			}

			template<size_t ...Args>
			size_t SuperTimer<Args...>::get_report_not_timer()
			{
				return benchmark[calls_per_sec_pos + 1];
			}

			template<size_t ...Args>
			double SuperTimer<Args...>::get_report_instant_not_timer() // if you wait, the amount of non-internal events (aka not internal timer). If you don't, every not-event return and local events
			{
				return quickmark[calls_per_sec_pos + 1][0];
			}

			template<size_t ...Args>
			void SuperTimer<Args...>::has_event_wait()
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

			template<size_t ...Args>
			bool SuperTimer<Args...>::has_event()
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

			template<size_t ...Args>
			bool SuperTimer<Args...>::is_this_it(const size_t t)
			{
				if (!(lastev.type == ALLEGRO_EVENT_TIMER)) return false;
				return (lastev.timer.source == timers_p[t]);
			}

			template<size_t ...Args>
			ALLEGRO_EVENT SuperTimer<Args...>::get_last_event_raw()
			{
				return lastev;
			}
}
	}
}