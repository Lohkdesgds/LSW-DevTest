#include "eventcustom.h"


namespace LSW {
	namespace v5 {
		namespace Interface {



			void EventCustom::start_as_custom()
			{
				core = std::shared_ptr<ALLEGRO_EVENT_SOURCE>(
					[] { auto i = new ALLEGRO_EVENT_SOURCE(); al_init_user_event_source(i); return i; }(),
					[](ALLEGRO_EVENT_SOURCE* ev) { al_destroy_user_event_source(ev); });
			}

			EventCustom::EventCustom() : Event()
			{
				Handling::init_basic();
			}

			EventCustom::EventCustom(const EventCustom& e) : Event(e)
			{
				Handling::init_basic();
				start_as_custom();
			}

			EventCustom::EventCustom(EventCustom&& e)
			{
				Handling::init_basic();
				// shh if move this have to be done in future, do the ones here before parent
				Event::Event(std::move(e));
				// last
				start_as_custom();
			}

			void EventCustom::send_custom_event(const int at, const uintptr_t a, const uintptr_t b, const uintptr_t c, const uintptr_t d)
			{
				if (!core) return;
				ALLEGRO_EVENT ev;
				ev.type = at;
				ev.user.data1 = a;
				ev.user.data2 = b;
				ev.user.data3 = c;
				ev.user.data4 = d;
				ev.user.source = core.get();
				ev.user.timestamp = al_get_time();
				al_emit_user_event(core.get(), &ev, NULL);
			}

		}
	}
}