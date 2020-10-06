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
			// not a good idea
			/*void Mixer::update(void* a, unsigned b, void* c)
			{
				Mixer* thus = (Mixer*)c;

			}*/
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
				al_set_mixer_playing(mixer.get(), m);
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

			float Mixer::get_gain()
			{
				return al_get_mixer_gain(mixer.get());
			}

			void Mixer::set_gain(const float f)
			{
				al_set_mixer_gain(mixer.get(), f);
			}

			bool Mixer::exists() const
			{
				return mixer.get();
			}

			/*bool Mixer::postprocess_callback(std::function<void(float*, unsigned)> f)
			{
				return al_set_mixer_postprocess_callback(mixer.get(), &update, this);
			}*/

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