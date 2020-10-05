#pragma once

// C++
#include <memory>
#include <string>

// Others
#include "..\..\Handling\Initialize\initialize.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			/*
			A voice is the device itself. You may want to link ONE mixer to it, then have other mixes linked to the main mixer, and them SAMPLE_INSTANCEs into mixers
			*/

			class Voice {
				/*
				* Friend class Mixer: because Mixer should link to a Voice, and it's better if you just set(Voice). get_instance is just another friend-like.
				*/
				friend class Mixer;

				std::shared_ptr<ALLEGRO_VOICE> device;

				bool set(ALLEGRO_VOICE*);
			public:
				Voice();

				// creates/load a mixer (frequency, depth, channel configuration)
				bool load(const int = 48000, const ALLEGRO_AUDIO_DEPTH = ALLEGRO_AUDIO_DEPTH_INT16, const ALLEGRO_CHANNEL_CONF = ALLEGRO_CHANNEL_CONF_2);

				// true = muted
				void mute(const bool);

				bool exists() const;

				// attaches 
				//bool attach(std::shared_ptr<Mixer>);

				//std::shared_ptr<ALLEGRO_VOICE> get_instance();
			};

		}
	}
}