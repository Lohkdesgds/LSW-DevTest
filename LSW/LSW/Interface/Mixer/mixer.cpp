#include "mixer.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			bool Mixer::set(ALLEGRO_MIXER* s)
			{
				if (!s) return false;
				mixer = std::shared_ptr<ALLEGRO_MIXER>(s, [](ALLEGRO_MIXER*& b) { if (al_is_system_installed() && b) { al_destroy_mixer(b); b = nullptr; } });
				return mixer.get();
			}
			Mixer::Mixer()
			{
				Handling::init_basic();
				Handling::init_audio();
			}

			bool Mixer::load(const int frequency, const ALLEGRO_AUDIO_DEPTH depth, const ALLEGRO_CHANNEL_CONF conf)
			{
				return set(al_create_mixer(frequency, depth, conf));
			}

			void Mixer::mute(const bool m)
			{
				al_set_mixer_playing(&(*mixer), m);
			}

			bool Mixer::attach_to(std::shared_ptr<Mixer> s)
			{
				return al_attach_mixer_to_mixer(s->mixer.get(), mixer.get());
			}

			bool Mixer::attach_to(std::shared_ptr<Voice> s)
			{
				return al_attach_mixer_to_voice(mixer.get(), s->device.get());
				//return al_attach_sample_instance_to_mixer(&(*s->get_instance()), &(*mixer));
			}

			bool Mixer::exists() const
			{
				return mixer.get();
			}

			/*bool Mixer::auto_attach()
			{
				SuperResource<Voice> voices;
				auto mvoice = voices.getMain();
				if (mvoice) return attach(mvoice);
				return false;
			}*/

			/*std::shared_ptr<ALLEGRO_MIXER> Mixer::get_instance()
			{
				return mixer;
			}*/

		}
	}
}