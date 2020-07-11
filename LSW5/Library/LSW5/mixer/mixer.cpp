#include "mixer.h"

namespace LSW {
	namespace v5 {
		
		
		Mixer::Mixer()
		{
			lsw_al_init();
			lsw_al_init_acodec_addon();
			lsw_al_install_audio();
			SuperResource<ALLEGRO_MIXER> mixers;
			mixer = mixers.load("mixer_v&" + Tools::generateRandomUniqueStringN());
		}

		Mixer::~Mixer()
		{
			SuperResource<ALLEGRO_MIXER> mixers;
			if (mixer) mixers.remove(mixer);
		}

		void Mixer::mute(const bool m)
		{
			al_set_mixer_playing(&(*mixer), m);
		}

		bool Mixer::attach(std::shared_ptr<Mixer> s)
		{
			return al_attach_mixer_to_mixer(&(*s->getInstance()), &(*mixer));
		}

		bool Mixer::attach(std::shared_ptr<Voice> s)
		{
			return al_attach_mixer_to_voice(&(*mixer), &(*s->getInstance()));
			//return al_attach_sample_instance_to_mixer(&(*s->getInstance()), &(*mixer));
		}

		bool Mixer::autoAttach()
		{
			SuperResource<Voice> voices;
			auto mvoice = voices.getMain();
			if (mvoice) return attach(mvoice);
			return false;
		}

		std::shared_ptr<ALLEGRO_MIXER> Mixer::getInstance()
		{
			return mixer;
		}

	}
}