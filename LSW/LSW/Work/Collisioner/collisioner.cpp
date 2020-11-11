#include "collisioner.h"

namespace LSW {
	namespace v5 {
		namespace Work {



			void Collisioner::task_collision()
			{
				Tools::AutoLock luck(sprites_m);

				auto diff = MILLI_NOW - _last;
				_last = MILLI_NOW;

				effective_speed = ((collisioner::speed_smoothness_calculation - 1.0) * effective_speed + (1e3 / diff.count())) / collisioner::speed_smoothness_calculation;

				for (auto& i : sprites) {
					i.get().update_and_clear(conf, true); // process positioning

					for (const auto& j : sprites) {
						if (!i.get().is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, j))
							i.get().collide(j, true);
					}
				}

			}

			Collisioner::Collisioner(const Interface::Config& c) : conf(c)
			{
				// just ref thing
			}

			Collisioner::~Collisioner()
			{
				stop();
			}

			void Collisioner::insert(Sprite_Base& s)
			{
				Tools::AutoLock luck(sprites_m);
				for (const auto& i : sprites) { if (i.get().is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, s)) return; }
				sprites.push_back(std::ref(s));
			}

			void Collisioner::remove(const Sprite_Base& s)
			{
				Tools::AutoLock luck(sprites_m);
				for (size_t p = 0; p < sprites.size(); p++) {
					if (sprites[p].get().is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, s)) {
						sprites.erase(sprites.begin() + p--);
					}
				}
			}

			void Collisioner::start(const double dt)
			{
				if (evhdl.running()) throw Handling::Abort(__FUNCSIG__, "Already running!", Handling::abort::abort_level::GIVEUP);
				if (dt <= 0.0) throw Handling::Abort(__FUNCSIG__, "Invalid delta!", Handling::abort::abort_level::GIVEUP);

				tick.set_delta(dt);
				_last = MILLI_NOW;

				evhdl.set_mode(Interface::eventhandler::handling_mode::NO_BUFFER_SKIP); // if collision is slow, give up and run as fast as it can.
				evhdl.add(tick);
				evhdl.set_run_autostart([&](const Interface::RawEvent& ev) {
					task_collision();
				});

				tick.start();
			}

			void Collisioner::set_speed(const double dt)
			{
				if (dt <= 0.0) throw Handling::Abort(__FUNCSIG__, "Invalid delta!", Handling::abort::abort_level::GIVEUP);
				tick.set_delta(dt);				
			}

			const double Collisioner::get_speed() const
			{
				return tick.get_delta();
			}

			void Collisioner::set_stop()
			{
				tick.stop();
				evhdl.set_stop();
			}

			void Collisioner::stop()
			{
				tick.stop();
				evhdl.stop();
				effective_speed = 0.0;
			}

			double Collisioner::effective_tps() const
			{
				return effective_speed;
			}

		}
	}
}