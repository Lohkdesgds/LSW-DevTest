#pragma once

// C++
#include <memory>
#include <string>

// Others
#include "..\superresource\superresource.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\voice\voice.h"


namespace LSW {
	namespace v5 {

		/*namespace mixer {

		}*/

		class Mixer {
			std::shared_ptr<ALLEGRO_MIXER> mixer;
		public:
			Mixer();
			~Mixer();

			// mute/stop mixer
			void mute(const bool);

			// attaches a mixer into the mixer
			bool attach(std::shared_ptr<Mixer>);
			// attach Track to mixer
			bool attach(std::shared_ptr<Voice>);

			// attach to a Voice (master voice) (see SuperResource.setMain(...))
			bool autoAttach();

			std::shared_ptr<ALLEGRO_MIXER> getInstance();
		};

	}
}