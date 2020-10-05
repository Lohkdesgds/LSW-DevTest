#pragma once

// C++
#include <string>
#include <functional>

// Others
#include "..\..\Handling\Initialize\initialize.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			class Sample {
				/*
				* Friend class Track: because Track should link to/with a Sample. get_instance is just another friend-like.
				*/
				friend class Track;

				std::shared_ptr<ALLEGRO_SAMPLE> file_sample;

				bool set(ALLEGRO_SAMPLE*);
			public:
				Sample();

				// path
				bool load(const std::string);

				operator bool() const;
				bool operator!() const;
				bool operator==(Sample&) const;
				bool operator==(ALLEGRO_SAMPLE*&) const;

				bool exists() const;

			};
		}
	}
}