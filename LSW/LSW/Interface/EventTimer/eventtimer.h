#pragma once

#include "../Events/events.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace eventtimer {
				const double default_timer_delta = 1.0; // sec, used to start event
			}

			/// <summary>
			/// <para>A timer Event. This has to exist while EventHandler is tied to this.</para>
			/// </summary>
			class EventTimer : public Event {
				std::shared_ptr<ALLEGRO_TIMER> tim;
				void init_timer();
			public:
				EventTimer();

				/// <summary>
				/// <para>Start EventTimer with the time set already.</para>
				/// </summary>
				/// <param name=""></param>
				EventTimer(const double);

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{EventTimer}">EventTimer to copy reference.</param>
				EventTimer(const EventTimer&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{EventTimer}">EventTimer to move.</param>
				EventTimer(EventTimer&&);

				EventTimer(ALLEGRO_EVENT_SOURCE*) = delete;

				/// <summary>
				/// <para>Set timer delta in seconds.</para>
				/// </summary>
				/// <param name="{double}">Delta in seconds.</param>
				void set_delta(const double);

				/// <summary>
				/// <para>Start timer event.</para>
				/// </summary>
				void start();

				/// <summary>
				/// <para>Stop timer event.</para>
				/// </summary>
				void stop();

			};

		}
	}
}