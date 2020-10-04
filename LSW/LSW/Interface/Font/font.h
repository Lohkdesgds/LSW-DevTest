#pragma once

// C++
#include <string>
#include <functional>
// Others
#include "..\..\Handling\Initialize\initialize.h"
#include "..\..\Tools\CString\cstring.h"
#include "..\Color\color.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			class Font {
				std::shared_ptr<ALLEGRO_FONT> font;

				bool set(ALLEGRO_FONT*);
				ALLEGRO_FONT* quick() const;
				ALLEGRO_FONT* quick();
				Color hex(const int);
			public:
				Font();

				// load common font, with size and flags
				bool load(const char*, const int = 75, const int = 0);
				// load ttf font, with size and flags
				bool load_ttf(const char*, const int = 75, const int = 0);

				operator bool() const;
				const bool operator!() const;
				const bool operator==(Font&) const;
				const bool operator==(ALLEGRO_FONT*&) const;
				ALLEGRO_FONT* operator*();

				// top part of font height
				int get_line_ascent();
				// bottom part of font height
				int get_line_descent();
				// sum of ascent and descent basically
				int get_line_height();

				// gets width of the string
				int get_width(const char*);
				// gets width of the string
				int get_width(Tools::Cstring);

				// faster direct drawing one color
				void draw(Color, const float, const float, const int, const char*);
				// slower, Cstring has colors itself, so no need to tell the color. It knows.
				void draw(const float, const float, const int, Tools::Cstring);

				// todo
				//void draw(ALLEGRO_COLOR, const float, const float, const float, const int, Cstring);

			};

		}
	}
}