#include "events.h"

namespace LSW {
	namespace v5 {
		namespace Interface {


			RawEvent::RawEvent(ALLEGRO_EVENT& evx)
			{
				ev = evx;
			}

			int RawEvent::type() const
			{
				return ev.type;
			}

			const ALLEGRO_DISPLAY_EVENT&  RawEvent::display_event() const {
				return ev.display;
			}
			const ALLEGRO_JOYSTICK_EVENT& RawEvent::joystick_event() const {
				return ev.joystick;
			}
			const ALLEGRO_KEYBOARD_EVENT& RawEvent::keyboard_event() const {
				return ev.keyboard;
			}
			const ALLEGRO_MOUSE_EVENT&    RawEvent::mouse_event() const {
				return ev.mouse;
			}
			const ALLEGRO_TIMER_EVENT&    RawEvent::timer_event() const {
				return ev.timer;
			}
			const ALLEGRO_TOUCH_EVENT&    RawEvent::touch_event() const {
				return ev.touch;
			}
			const ALLEGRO_USER_EVENT&     RawEvent::user_event() const {
				return ev.user;
			}

			Event::Event(const Event& ev)
			{
				Handling::init_basic();
				core = ev.core;
				is_custom = ev.is_custom;
			}

			Event::Event(Event&& ev)
			{
				Handling::init_basic();
				core = std::move(ev.core);
				is_custom = std::move(ev.is_custom);
			}

			Event::Event(ALLEGRO_EVENT_SOURCE* ev)
			{
				Handling::init_basic();
				core = std::shared_ptr<ALLEGRO_EVENT_SOURCE>(ev, [](ALLEGRO_EVENT_SOURCE* e) {});
				*is_custom = false;
			}

			void Event::init_as_custom()
			{
				*is_custom = true;
				core = std::shared_ptr<ALLEGRO_EVENT_SOURCE>(
					[] { auto i = new ALLEGRO_EVENT_SOURCE(); al_init_user_event_source(i); return i; }(),
					[](ALLEGRO_EVENT_SOURCE* ev) { al_destroy_user_event_source(ev); });
			}

			void Event::send_custom_event(const int at, const uintptr_t a, const uintptr_t b, const uintptr_t c, const uintptr_t d)
			{
				if (!*is_custom || !core) return;
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


			const Event get_keyboard_event()
			{
				Handling::init_basic();
				Handling::init_keyboard();
				return Event(al_get_keyboard_event_source());
			}
			const Event get_mouse_event()
			{
				Handling::init_basic();
				Handling::init_mouse();
				return Event(al_get_mouse_event_source());
			}

			const Event get_joystick_event()
			{
				Handling::init_basic();
				Handling::init_joypad();
				return Event(al_get_joystick_event_source());
			}

			const Event get_touchscreen_event()
			{
				Handling::init_basic();
				Handling::init_touch();
				return Event(al_get_touch_input_event_source());
			}


			void EventHandler::__init()
			{
				own_queue = std::shared_ptr<ALLEGRO_EVENT_QUEUE>(al_create_event_queue(),
					[](ALLEGRO_EVENT_QUEUE* ev) { al_destroy_event_queue(ev); ev = nullptr; });
			}
			EventHandler::EventHandler()
			{
				Handling::init_basic();
				__init();
			}
			EventHandler::~EventHandler()
			{
				deinit();
			}
			void EventHandler::add(const Event& ev)
			{
				if (!ev.core) return;
				if (!al_is_event_source_registered(own_queue.get(), ev.core.get())) {
					al_register_event_source(own_queue.get(), ev.core.get());
				}
			}
			void EventHandler::remove(const Event& ev)
			{
				if (!ev.core) return;
				if (al_is_event_source_registered(own_queue.get(), ev.core.get())) {
					al_unregister_event_source(own_queue.get(), ev.core.get());
				}
			}
			void EventHandler::deinit()
			{
				thr.stop();
				thr.join(); // sync and clear (may take up to 1 sec)
				own_queue.reset();
				trigger_func = std::function<void(const RawEvent&)>();
			}
			void EventHandler::set_run_autostart(const std::function<void(const RawEvent&)> f)
			{
				if (!thr.running() && f) {
					trigger_func = f;
					thr.set([=](Tools::boolThreadF keep) {
						while (keep()) {
							ALLEGRO_EVENT ev;
							if (!al_wait_for_event_timed(own_queue.get(), &ev, 1.0)) continue;
							if (ev.type) {
								RawEvent re(ev);
								trigger_func(re);
							}
						}
						return 0;
					});
					thr.start();
				}
			}

		}
	}
}