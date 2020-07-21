#pragma once

// C
#define _CRT_RAND_S
// C++
#include <numeric>
#include <functional>
// Others
#include "..\lower\bitmap\bitmap.h"
#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\database\database.h"
#include "..\tools\tools.h"
#include "..\shared\shared.h"

namespace LSW {
	namespace v5 {

		namespace shinefx {

			enum class e_double { SPEED_ROTATION_T, EACH_SIZE_RAD, FRAMES_PER_SECOND_UPDATE };
			enum class e_color { BACKGROUND, FOREGROUND };
			//enum class e_boolean_readonly { SHOULD_UPDATE_SCREEN };
			enum class e_chronomillis_readonly { LAST_DRAW, LAST_TIE_FRAME_VERIFICATION };

			const SuperMap<std::function<double(void)>>								e_double_defaults = {
				{[] {return 1.0; },								(e_double::SPEED_ROTATION_T),								CHAR_INIT("speed_rotation_t")},
				{[] {return 0.1 * ALLEGRO_PI;},					(e_double::EACH_SIZE_RAD),									CHAR_INIT("each_size_rad")},
				{[] {return 60.0;},							    (e_double::FRAMES_PER_SECOND_UPDATE),						CHAR_INIT("frames_per_second_update")}
			};
			const SuperMap<std::function<ALLEGRO_COLOR(void)>>						e_color_defaults = {
				{[] {return ALLEGRO_COLOR(); },					(e_color::BACKGROUND),										CHAR_INIT("background")},
				{[] {return ALLEGRO_COLOR(); },					(e_color::FOREGROUND),										CHAR_INIT("foreground")}
			};
			const SuperMap<std::function<std::chrono::milliseconds(void)>>			e_chronomillis_readonly_defaults = {
				{[]{return MILLI_NOW;},							(e_chronomillis_readonly::LAST_DRAW),						CHAR_INIT("last_draw")},
				{[] {return MILLI_NOW;},						(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),		CHAR_INIT("last_tie_verification")}
			};

			constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast (see also block::...)
		}

		class ShineFX : public Sprite_Base, public Workaround<std::chrono::milliseconds> {

			struct _bitmap {
				std::shared_ptr<Bitmap> ref;
				std::string id;
			} bmp;


			struct shinefx_data : Sprite_Base::sprite_base_data {
				std::shared_ptr<SuperMap<std::function<std::chrono::milliseconds(void)>>>	chronomillis_readonly_data;
			};

			//std::shared_ptr<shinefx_data> data_shinefx = std::make_shared<shinefx_data>();


			// creates new bitmap if needed with these dimensions and clean up old bmp if needed
			void createSwap(const int, const int);
			// checks resolution match with screen
			void _checkInternalBMP();


			std::shared_ptr<Bitmap> reference;
			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getDirect;
			using Workaround<std::chrono::milliseconds>::set;
			using Workaround<std::chrono::milliseconds>::get;
			using Workaround<std::chrono::milliseconds>::getDirect;

			ShineFX();
			ShineFX(ShineFX&);

			// set to blindly use this comrade's attributes as theirs (this must be Block)
			void twinUpAttributes(const std::shared_ptr<shinefx_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<shinefx_data> getAttributes();
		};

	}
}