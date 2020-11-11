#pragma once

#include "../../Tools/SuperThread/superthread.h"
#include "../../Tools/SuperMutex/supermutex.h"
#include "../../Interface/Events/events.h"
#include "../../Interface/EventTimer/eventtimer.h"
#include "../../Interface/Config/config.h"
#include "../Sprite/sprite.h"


namespace LSW {
	namespace v5 {
		namespace Work {

			namespace collisioner {
				constexpr double speed_smoothness_calculation = 10.0; // >= 2.0, greater is smoother, but slower
			}

			/// <summary>
			/// <para>Collisioner handles collision and update of Sprite_Base classes. You can have multiple of this if you want a multilayer-like experience.</para>
			/// </summary>
			class Collisioner {
				Tools::SuperMutex sprites_m;
				std::vector<Sprite_Base> sprites;

				double effective_speed = 0.0;
				std::chrono::milliseconds _last{}; // for effective speed calculation

				Interface::EventHandler evhdl;
				Interface::EventTimer tick;
				const Interface::Config& conf;

				void task_collision();
			public:
				/// <summary>
				/// <para>Start Collisioner. You need a Config so mouse information can be gathered.</para>
				/// </summary>
				/// <param name="{Config}">Const reference for read operations.</param>
				Collisioner(const Interface::Config&);
				Collisioner(const Collisioner&) = delete;
				Collisioner(Collisioner&&) = delete;
				~Collisioner();
				
				/// <summary>
				/// <para>Insert a object to collide internally.</para>
				/// </summary>
				/// <param name="{Sprite_Base}">A Sprite_Base class type.</param>
				void insert(const Sprite_Base&);
				/// <summary>
				/// <para>Remove a object based on its sprite::e_uintptrt::DATA_FROM.</para>
				/// </summary>
				/// <param name="{Sprite_Base}">A Sprite_Base class type.</param>
				void remove(const Sprite_Base&);

				/// <summary>
				/// <para>Start the collision thread with this amount of Ticks Per Second.</para>
				/// </summary>
				/// <param name="{double}">TPS.</param>
				void start(const double);

				/// <summary>
				/// <para>Change TPS (if running).</para>
				/// </summary>
				/// <param name="{double}">New TPS.</param>
				void set_speed(const double);
				
				/// <summary>
				/// <para>Get current expected TPS (the one set).</para>
				/// </summary>
				/// <returns>{double} TPS expected.</returns>
				const double get_speed() const;
				
				/// <summary>
				/// <para>Set to stop collision thread (no lock, call stop() later to sync if you want to).</para>
				/// </summary>
				void set_stop();
				
				/// <summary>
				/// <para>Stop collision thread.</para>
				/// </summary>
				void stop();

				/// <summary>
				/// <para>The actual TPS (average).</para>
				/// </summary>
				/// <returns>{double} Approx. TPS.</returns>
				double effective_tps() const;
			};


		}
	}
}