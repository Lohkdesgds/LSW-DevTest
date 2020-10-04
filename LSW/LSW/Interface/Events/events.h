#pragma once

#include <memory>
#include <functional>

#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Tools\SuperThread\superthread.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			// maybe change later
			class RawEvent {
				ALLEGRO_EVENT ev;
			public:
				RawEvent() = delete;
				RawEvent(ALLEGRO_EVENT&);

				// what is it?
				int type();

				ALLEGRO_DISPLAY_EVENT&  display_event();
				ALLEGRO_JOYSTICK_EVENT& joystick_event();
				ALLEGRO_KEYBOARD_EVENT& keyboard_event();
				ALLEGRO_MOUSE_EVENT&    mouse_event();
				ALLEGRO_TIMER_EVENT&    timer_event();
				ALLEGRO_TOUCH_EVENT&    touch_event();
				ALLEGRO_USER_EVENT&     user_event();

			};

			class Event {
				std::shared_ptr<ALLEGRO_EVENT_SOURCE> core;
				std::shared_ptr<bool> is_custom = std::make_shared<bool>(false);
			public:
				Event() = default;
				Event(const Event&);
				Event(Event&&);
				Event(ALLEGRO_EVENT_SOURCE*); // won't "die"

				void init_as_custom();
				void send_custom_event(const int, const uintptr_t, const uintptr_t, const uintptr_t, const uintptr_t);

				friend class EventHandler;
			};

			
			const Event get_keyboard_event();
			const Event get_mouse_event();
			const Event get_joystick_event();
			const Event get_touchscreen_event();




			class EventHandler {
				std::shared_ptr<ALLEGRO_EVENT_QUEUE> own_queue; // trigger itself
				std::function<void(RawEvent&)> trigger_func;
				Tools::SuperThread thr;

				void __init();
			public:
				EventHandler();
				~EventHandler();

				// adds an existing event source
				void add(const Event);
				// removes an event from queue
				void remove(const Event);

				void deinit();

				void set_run_autostart(const std::function<void(RawEvent&)>);
			};
		}
	}
}