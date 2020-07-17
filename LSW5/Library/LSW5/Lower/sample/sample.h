#pragma once

// C++
#include <string>
#include <functional>
// Others
#include "..\..\allegroinitialization\allegroinitialization.h"

namespace LSW {
	namespace v5 {

		class Sample {
			std::shared_ptr<ALLEGRO_SAMPLE> file_sample;

			bool set(ALLEGRO_SAMPLE*);
		public:
			Sample();

			// path
			bool load(const std::string);

			operator bool() const;
			const bool operator!() const;
			const bool operator==(Sample&) const;
			const bool operator==(ALLEGRO_SAMPLE*&) const;

			std::shared_ptr<ALLEGRO_SAMPLE> getInstance();
		};
	}
}