#pragma once

#include <memory>
#include <functional>

#include "..\allegroinitialization\allegroinitialization.h"
#include "..\superthread\superthread.h"

namespace LSW {
	namespace v5 {

		class IndependentEventHandler {
			struct custom_event {
				std::shared_ptr<ALLEGRO_EVENT_SOURCE> evsrc; // custom user event source
				std::shared_ptr<ALLEGRO_EVENT_QUEUE> own_queue; // trigger itself
				std::function<void(ALLEGRO_EVENT&)> trigger_func;
				Threads::CustomThread<int> thr;
			};
			std::shared_ptr<custom_event> event_handle;

			void __init_event_handle_custom();
			void __check_event_source();
		public:
			IndependentEventHandler();
			IndependentEventHandler(IndependentEventHandler&);
			~IndependentEventHandler();

			// creates and adds internal user event type so you can call it somewhere else if you want to
			void add();
			// adds an existing event source
			void add(ALLEGRO_EVENT_SOURCE*);
			void deinit();

			void startAutoEventHandlerWith(const std::function<void(ALLEGRO_EVENT&)>);

			void sendCustomEvent(const int, const uintptr_t, const uintptr_t, const uintptr_t, const uintptr_t);

			std::shared_ptr<custom_event> getInstance();

		};
	}
}