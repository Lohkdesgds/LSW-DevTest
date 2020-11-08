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
					i.update_and_clear(conf, true); // process positioning

					for (auto& j : sprites) {
						if (i.is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, j)) i.collide(j, true);
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

			void Collisioner::insert(const Sprite_Base& s)
			{
				Tools::AutoLock luck(sprites_m);
				for (auto& i : sprites) { if (i.is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, s)) return; }
				Sprite_Base cpy;
				cpy.twin_up_attributes(s);
				sprites.push_back(std::move(cpy));
			}

			void Collisioner::remove(const Sprite_Base& s)
			{
				Tools::AutoLock luck(sprites_m);
				for (size_t p = 0; p < sprites.size(); p++) {
					if (sprites[p].is_eq<uintptr_t>(sprite::e_uintptrt::DATA_FROM, s)) {
						sprites.erase(sprites.begin() + p--);
					}
				}
			}

			void Collisioner::start(const double dt)
			{
				if (thr.running()) throw Handling::Abort(__FUNCSIG__, "Already running!", Handling::abort::abort_level::GIVEUP);
				if (dt <= 0.0) throw Handling::Abort(__FUNCSIG__, "Already running!", Handling::abort::abort_level::GIVEUP);

				tick.set_delta(dt);
				_last = MILLI_NOW;

				evhdl.set_mode(Interface::eventhandler::handling_mode::NO_BUFFER_SKIP); // if collision is slow, give up and run as fast as it can.
				evhdl.add(tick);
				evhdl.set_run_autostart([&](const Interface::RawEvent& ev) {
					task_collision();
				});

				tick.start();
			}

			void Collisioner::stop()
			{
				if (thr.running()) {
					thr.stop();
					thr.join();

					tick.stop();
					evhdl.reset();
					effective_speed = 0.0;
				}
			}

			double Collisioner::effective_tps() const
			{
				return effective_speed;
			}

		}
	}
}