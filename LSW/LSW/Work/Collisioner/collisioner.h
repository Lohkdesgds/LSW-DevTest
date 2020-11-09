#pragma once

#include "../../Tools/SuperThread/superthread.h"
#include "../../Tools/SuperMutex/supermutex.h"
#include "../../Interface/Events/events.h"
#include "../../Interface/EventTimer/eventtimer.h"
#include "../../Interface/Config/config.h"
#include "../Sprite/sprite.h"


//vector of layers, being each layer a thread?


namespace LSW {
	namespace v5 {
		namespace Work {

			namespace collisioner {
				constexpr double speed_smoothness_calculation = 10.0; // >= 2.0, greater is smoother, but slower
			}

			class Collisioner {
				Tools::SuperMutex sprites_m;
				std::vector<Sprite_Base> sprites;

				double effective_speed = 0.0;
				std::chrono::milliseconds _last; // for effective speed calculation

				Interface::EventHandler evhdl;
				Interface::EventTimer tick;
				const Interface::Config& conf;

				void task_collision();
			public:
				Collisioner(const Interface::Config&);
				Collisioner(const Collisioner&) = delete;
				Collisioner(Collisioner&&) = delete;
				~Collisioner();
				
				void insert(const Sprite_Base&);
				// based on sprite::e_uintptrt::DATA_FROM
				void remove(const Sprite_Base&);

				// tick interval, seconds
				void start(const double);
				// change internal speed, if running
				void set_speed(const double);
				// get current expected tps
				const double get_speed() const;
				// stop
				void stop();

				double effective_tps() const;
			};


		}
	}
}