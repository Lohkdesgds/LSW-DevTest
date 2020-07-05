#include "voice.h"


namespace LSW {
	namespace v5 {



		Voice::Voice()
		{
			lsw_al_init();
			lsw_al_init_acodec_addon();
			lsw_al_install_audio();

			SuperResource<ALLEGRO_VOICE> devices;
			device = devices.load("voice_v&" + Tools::generateRandomUniqueStringN());
		}

		Voice::~Voice()
		{
			SuperResource<ALLEGRO_VOICE> devices;
			if (device) devices.remove(device);
		}

		void Voice::mute(const bool m)
		{
			al_set_voice_playing(&(*device), m);
		}

		bool Voice::attach(std::shared_ptr<Mixer> s)
		{
			return al_attach_mixer_to_voice(&(*s->getInstance()), &(*device));
		}

		std::shared_ptr<ALLEGRO_VOICE> Voice::getInstance()
		{
			return device;
		}

	}
}