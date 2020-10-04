#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>

namespace LSW {
	namespace v5 {
		namespace Handling {

			bool init_basic();
			bool init_audio();
			bool init_font();
			bool init_graphics();
			bool init_keyboard();
			bool init_mouse();
			bool init_joypad();
			bool init_touch();
		}

	}
}