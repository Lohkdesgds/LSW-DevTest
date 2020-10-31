#include "eventtimer.h"


namespace LSW {
	namespace v5 {
		namespace Interface {



			void EventTimer::init_timer()
			{
				tim = std::shared_ptr<ALLEGRO_TIMER>(
					[] { ALLEGRO_TIMER* i = al_create_timer(eventtimer::default_timer_delta); return i; }(),
					[](ALLEGRO_TIMER* ev) { al_destroy_timer(ev); });

				core = std::shared_ptr<ALLEGRO_EVENT_SOURCE>(al_get_timer_event_source(tim.get()), [](ALLEGRO_EVENT_SOURCE* e) {});
			}

			EventTimer::EventTimer() : Event()
			{
				Handling::init_basic();
				init_timer(); // calls Event() to set timer as event
			}

			EventTimer::EventTimer(const double d) : Event()
			{
				Handling::init_basic();
				init_timer(); // calls Event() to set timer as event
				set_delta(d);
			}

			EventTimer::EventTimer(const EventTimer& e) : Event(e)
			{
				if (!e.tim) throw Handling::Abort(__FUNCSIG__, "Incompatible event to copy!");
				tim = e.tim;
			}

			EventTimer::EventTimer(EventTimer&& e) : Event(e)
			{
				if (!e.tim) throw Handling::Abort(__FUNCSIG__, "Incompatible event to copy!");
				tim = std::move(e.tim);
			}

			void EventTimer::set_delta(const double d)
			{
				if (d <= 0.0) throw Handling::Abort(__FUNCSIG__, "Incompatible time!");
				al_set_timer_speed(tim.get(), d);
			}

			void EventTimer::start()
			{
				al_start_timer(tim.get());
			}

			void EventTimer::stop()
			{
				al_stop_timer(tim.get());
			}

		}
	}
}