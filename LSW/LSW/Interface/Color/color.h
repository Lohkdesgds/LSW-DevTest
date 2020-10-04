#pragma once

#include "..\..\Handling\Initialize\initialize.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			class Color : public ALLEGRO_COLOR {
			public:
				// default constructor
				Color() = default;
				// initialize with float with values in range of [0.0..1.0]
				Color(const float, const float, const float, const float = 1.0);
				// initialize with 8 bit values [0-255]
				Color(const unsigned char, const unsigned char, const unsigned char, const unsigned char = 255);
				// initialize with 8 bit values [0-255] (auto limits itself)
				Color(const int, const int, const int, const int = 255);
				// copy another Color
				Color(const Color&);
				// copy from a ALLEGRO_COLOR
				Color(ALLEGRO_COLOR);

				// gets red color
				float get_r() const;
				// gets green color
				float get_g() const;
				// gets blue color
				float get_b() const;
				// gets alpha channel
				float get_a() const;

				// aplies the values of the Color
				Color& operator=(Color&&);
				// aplies the values of the Color
				Color& operator=(const Color& c);
			};
		}
	}
}