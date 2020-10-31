#pragma once

#include "../Events/events.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			/// <summary>
			/// <para>A custom User Event. This has to exist while EventHandler is tied to this.</para>
			/// </summary>
			class EventCustom : public Event {
				void start_as_custom();
			public:
				EventCustom();

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{EventCustom}">EventCustom to copy reference.</param>
				EventCustom(const EventCustom&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{EventCustom}">EventCustom to move.</param>
				EventCustom(EventCustom&&);

				EventCustom(ALLEGRO_EVENT_SOURCE*) = delete;

				/// <summary>
				/// <para>Send some custom data.</para>
				/// <para>You can send up to 32 bytes of data per event (via up to 4 unsigned long long).</para>
				/// </summary>
				/// <param name="{int}">Event ID (DON'T USE 0) (you can use this to do many event types in custom event source(s)).</param>
				/// <param name="{uintptr_t}">Data slot.</param>
				/// <param name="{uintptr_t}">Data slot.</param>
				/// <param name="{uintptr_t}">Data slot.</param>
				/// <param name="{uintptr_t}">Data slot.</param>
				void send_custom_event(const int, const uintptr_t, const uintptr_t, const uintptr_t, const uintptr_t);
			};

		}
	}
}