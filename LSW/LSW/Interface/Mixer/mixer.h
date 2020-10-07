#pragma once

// C++
#include <memory>
#include <string>

// Others
#include "..\..\Handling\Initialize\initialize.h"
#include "..\Voice\voice.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			class Mixer {
				/*
				* Friend class Track: because Track should link to a Mixer, and it's better if you just set(Mixer). get_instance is just another friend-like.
				*/
				friend class Track;
				std::shared_ptr<ALLEGRO_MIXER> mixer;

				bool set(ALLEGRO_MIXER*);

				// not a good idea
				//static void update(void*, unsigned, void*);
			public:
				Mixer();

				// creates/load a mixer (frequency, depth, channel configuration)
				bool load(const int = 48000, const ALLEGRO_AUDIO_DEPTH = ALLEGRO_AUDIO_DEPTH_INT16, const ALLEGRO_CHANNEL_CONF = ALLEGRO_CHANNEL_CONF_2);

				// mute/stop mixer
				void mute(const bool);

				// attaches a mixer into the mixer
				bool attach_to(std::shared_ptr<Mixer>);
				// attach Voice to mixer
				bool attach_to(std::shared_ptr<Voice>);

				float get_gain() const;
				void set_gain(const float);

				bool exists() const;

				// not a good idea
				//bool postprocess_callback(std::function<void(float*, unsigned)>);
				// attach to a Voice (master voice) (see SuperResource.setMain(...))
				//bool auto_attach();

				//std::shared_ptr<ALLEGRO_MIXER> get_instance();
			};

		}
	}
}