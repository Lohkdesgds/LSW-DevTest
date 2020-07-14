#include "independenteventhandler.h"

namespace LSW {
	namespace v5 {
		void IndependentEventHandler::__init_event_handle_custom()
		{
			event_handle = std::shared_ptr<custom_event>(new custom_event(), [](custom_event* ev) {
				// destructor
				ev->thr.stop();
				// send null package to event list so it will unlock it
				{
					ALLEGRO_EVENT e;
					e.type = 0; // no event
					al_emit_user_event(&(*ev->evsrc), &e, NULL);
				}
				ev->thr.join(); // sync and clear
				ev->evsrc.reset();
				ev->own_queue.reset();
				ev->trigger_func = std::function<void(ALLEGRO_EVENT&)>();
				});
		}
		void IndependentEventHandler::__check_event_source()
		{
			if (event_handle->own_queue) return;
			event_handle->own_queue = std::shared_ptr<ALLEGRO_EVENT_QUEUE>(
				[=] { auto i = al_create_event_queue(); return i; }(),
				[](ALLEGRO_EVENT_QUEUE* ev) { al_destroy_event_queue(ev); ev = nullptr; });
		}
		IndependentEventHandler::IndependentEventHandler()
		{
			lsw_al_init();
			__init_event_handle_custom();
			add();
		}
		IndependentEventHandler::IndependentEventHandler(IndependentEventHandler& ev)
		{
			lsw_al_init();
			event_handle = ev.getInstance();
			add();
		}
		IndependentEventHandler::~IndependentEventHandler()
		{
			deinit();
		}
		void IndependentEventHandler::add()
		{
			if (event_handle->evsrc) return;
			event_handle->evsrc = std::shared_ptr<ALLEGRO_EVENT_SOURCE>(
				[] { auto i = new ALLEGRO_EVENT_SOURCE(); al_init_user_event_source(i); return i; }(),
				[](ALLEGRO_EVENT_SOURCE* ev) { al_destroy_user_event_source(ev); ev = nullptr; });
			add(&(*event_handle->evsrc));
		}
		void IndependentEventHandler::add(ALLEGRO_EVENT_SOURCE* ev)
		{
			if (!ev) return;
			__check_event_source();
			al_register_event_source(&(*event_handle->own_queue), ev);
		}
		void IndependentEventHandler::deinit()
		{
			event_handle.reset();
		}
		void IndependentEventHandler::startAutoEventHandlerWith(const std::function<void(ALLEGRO_EVENT&)> f)
		{
			if (!event_handle->thr.existThread() && f) {

				event_handle->trigger_func = f;
				event_handle->thr.setFunc([=](Threads::boolThreadF keep) {
					while (keep()) {
						ALLEGRO_EVENT ev;
						al_wait_for_event(&(*event_handle->own_queue), &ev);
						if (ev.type) event_handle->trigger_func(ev);
					}
					return 0;
				});
				event_handle->thr.start();
			}
		}
		void IndependentEventHandler::sendCustomEvent(const int at, const uintptr_t a, const uintptr_t b, const uintptr_t c, const uintptr_t d)
		{
			if (!event_handle->evsrc) return;
			ALLEGRO_EVENT ev;
			ev.type = at;
			ev.user.data1 = a;
			ev.user.data2 = b;
			ev.user.data3 = c;
			ev.user.data4 = d;			
			ev.user.source = &(*event_handle->evsrc);
			ev.user.timestamp = al_get_time();
			al_emit_user_event(&(*event_handle->evsrc), &ev, NULL);
		}
		std::shared_ptr<IndependentEventHandler::custom_event> IndependentEventHandler::getInstance()
		{
			return event_handle;
		}
	}
}
