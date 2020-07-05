#pragma once

// C++
#include <memory>
#include <string>

// Others
#include "..\superresource\superresource.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\mixer\mixer.h"
#include "..\tools\tools.h"

namespace LSW {
	namespace v5 {

		/*namespace voice {

		}*/


		/*
		A voice is the device itself. You may want to link ONE mixer to it, then have other mixes linked to the main mixer, and them SAMPLE_INSTANCEs into mixers
		*/

		class Voice {
			std::shared_ptr<ALLEGRO_VOICE> device;
		public:
			Voice();
			~Voice();

			// true = muted
			void mute(const bool);
			// attaches 
			bool attach(std::shared_ptr<Mixer>);

			std::shared_ptr<ALLEGRO_VOICE> getInstance();
		};

	}
}