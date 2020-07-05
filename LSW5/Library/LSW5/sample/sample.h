#pragma once

// C++
#include <memory>
#include <string>

// Others
#include "..\superresource\superresource.h"
#include "..\allegroinitialization\allegroinitialization.h"
#include "..\tools\tools.h"

namespace LSW {
	namespace v5 {

		/*namespace sample {

		}*/

		/*
		SAMPLE is the file itself, you have to create an ALLEGRO_SAMPLE_INSTANCE to use an ALLEGRO_SAMPLE and play it 
		*/

		class Sample {
			std::shared_ptr<ALLEGRO_SAMPLE> file_sample;
			std::string id;
		public:
			Sample();
			~Sample();

			// path
			void load(const std::string);

			// true if has sample loaded
			bool isLoaded();

			std::shared_ptr<ALLEGRO_SAMPLE> getInstance();
		};
	}
}