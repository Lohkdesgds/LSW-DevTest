#pragma once

#include <memory>
#include <functional>

#include "../../Handling/Initialize/initialize.h"
#include "../../Tools/SuperThread/superthread.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			/// <summary>
			/// <para>The Event itself (raw).</para>
			/// </summary>
			class RawEvent {
				ALLEGRO_EVENT ev;
			public:
				RawEvent() = delete;
				RawEvent(ALLEGRO_EVENT&);

				/// <summary>
				/// <para>What event type was it? (ALLEGRO_EVENT_*).</para>
				/// </summary>
				/// <returns>{int} Event type ID.</returns>
				int type() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_DISPLAY_EVENT} The event data.</returns>
				const ALLEGRO_DISPLAY_EVENT&  display_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_JOYSTICK_EVENT} The event data.</returns>
				const ALLEGRO_JOYSTICK_EVENT& joystick_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_KEYBOARD_EVENT} The event data.</returns>
				const ALLEGRO_KEYBOARD_EVENT& keyboard_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_MOUSE_EVENT} The event data.</returns>
				const ALLEGRO_MOUSE_EVENT&    mouse_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_TIMER_EVENT} The event data.</returns>
				const ALLEGRO_TIMER_EVENT&    timer_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_TOUCH_EVENT} The event data.</returns>
				const ALLEGRO_TOUCH_EVENT&    touch_event() const;

				/// <summary>
				/// <para>If type is this one, get this information.</para>
				/// </summary>
				/// <returns>{ALLEGRO_USER_EVENT} The event data.</returns>
				const ALLEGRO_USER_EVENT&     user_event() const;
			};

			/// <summary>
			/// <para>Event is a event source from one of many known things, like keyboard, screen, timer...</para>
			/// </summary>
			class Event {
			protected:
				std::shared_ptr<ALLEGRO_EVENT_SOURCE> core;
			public:
				Event();

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{Event}">Event to copy.</param>
				Event(const Event&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{Event}">Event to move.</param>
				Event(Event&&) noexcept;

				/// <summary>
				/// <para>Direct RAW transformation to Event. This won't deinit the event source by itself!</para>
				/// </summary>
				/// <param name="{RAW EVENT SOURCE}">Event source.</param>
				Event(ALLEGRO_EVENT_SOURCE*);

				friend class EventHandler;
			};

			/// <summary>
			/// <para>Generates and creates a keyboard Event.</para>
			/// </summary>
			/// <returns>{Event} The Event.</returns>
			const Event get_keyboard_event();

			/// <summary>
			/// <para>Generates and creates a mouse Event.</para>
			/// </summary>
			/// <returns>{Event} The Event.</returns>
			const Event get_mouse_event();

			/// <summary>
			/// <para>Generates and creates a joystick Event.</para>
			/// </summary>
			/// <returns>{Event} The Event.</returns>
			const Event get_joystick_event();

			/// <summary>
			/// <para>Generates and creates a touchscreen Event.</para>
			/// </summary>
			/// <returns>{Event} The Event.</returns>
			const Event get_touchscreen_event();


			/// <summary>
			/// <para>Handle multiple Event sources with this. It will start a thread internally and call the function as events happen.</para>
			/// </summary>
			class EventHandler {
				std::shared_ptr<ALLEGRO_EVENT_QUEUE> own_queue; // trigger itself
				std::function<void(const RawEvent&)> trigger_func;
				Tools::SuperThread thr;

				void __init();
			public:
				EventHandler();
				~EventHandler();

				/// <summary>
				/// <para>Adds a Event to be handled by this (non-exclusive).</para>
				/// </summary>
				/// <param name="{Event}">A Event source.</param>
				void add(const Event&);

				/// <summary>
				/// <para>Remove a Event (if handled by this).</para>
				/// </summary>
				/// <param name="{Event}">A Event source.</param>
				void remove(const Event&);

				/// <summary>
				/// <para>Stops the internal thread and cleanup (you'll have to reset to start again).</para>
				/// </summary>
				void deinit();

				/// <summary>
				/// <para>Starts thread to handle events and call function as they happen.</para>
				/// </summary>
				/// <param name="{std::function}">A function to handle the Events.</param>
				void set_run_autostart(const std::function<void(const RawEvent&)>);

				/// <summary>
				/// <para>Wait for an event manually (ONLY VALID IF YOU DON'T CALL SET_RUN_AUTOSTART).</para>
				/// </summary>
				/// <returns>{RawEvent} The event, if any. if type == 0 it's empty.</returns>
				const RawEvent wait_event_manually(const double = -1.0);
			};
		}
	}
}