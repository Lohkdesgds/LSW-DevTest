#pragma once

// C
#define _CRT_RAND_S
// C++
#include <numeric>
#include <functional>
// Others
#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\database\database.h"
#include "..\tools\tools.h"
#include "..\shared\shared.h"

namespace LSW {
	namespace v5 {
		namespace bubblefx {
			enum class e_sizet_readonly { VECTOR_POSITION_DRAWING };
			enum class e_chronomillis_readonly { LAST_FRAME, LAST_TIE_FRAME_VERIFICATION };
			enum class e_boolean_readonly { SHOULD_UPDATE_SCREEN };
			enum class e_double { FRAMES_PER_SECOND, BLUR_INTENSITY };

			const SuperMap<size_t>							e_sizet_readonly_defaults{
				{0,																												(e_sizet_readonly::VECTOR_POSITION_DRAWING),			CHAR_INIT("vector_position_drawing")}
			};
			const SuperMap<std::chrono::milliseconds>		e_chronomillis_readonly_defaults{
				{MILLI_NOW,																										(e_chronomillis_readonly::LAST_FRAME),					CHAR_INIT("last_frame")},
				{MILLI_NOW,																										(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	CHAR_INIT("last_tie_verification")}
			};
			const SuperMap<bool>							e_boolean_readonly_defaults{
				{false,																											(e_boolean_readonly::SHOULD_UPDATE_SCREEN),				CHAR_INIT("should_update_screen")}
			};

			const SuperMap<std::function<double(void)>>		e_double_defaults = {
				{[] {return 25.0; },																							(e_double::FRAMES_PER_SECOND),							CHAR_INIT("frames_per_second")},
				{[] {return 0.92; },																							(e_double::BLUR_INTENSITY),								CHAR_INIT("blur_intensity")}
			};


			constexpr size_t default_bubbles_amount = 80;
			constexpr size_t default_bubbles_amount_draw_per_tick_max = 10;
			constexpr double max_bubbles_framerate = 25.0;
			constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast (see also block::...)
		}

		// have posx, posy, doesn't have collision (shouldn't have, but can have)...
		class BubbleFX : public Sprite_Base {
			struct particle {
				double posx = 0.0, posy = 0.0;
				double lastsize = 1.0;
				double lastpositionscalculated[2] = { 0.0, 0.0 };
			};

			struct _bitmap {
				std::shared_ptr<ALLEGRO_BITMAP> ref;
				std::string id;
			} bmp;

			std::vector<particle> positions;
			int siz[2] = { 1280, 720 };


			struct bubblefx_data {
				SuperMap<std::function<double(void)>>			double_data = bubblefx::e_double_defaults;
				SuperMap<std::chrono::milliseconds>				chronomillis_readonly_data = bubblefx::e_chronomillis_readonly_defaults;
				SuperMap<bool>									boolean_readonly_data = bubblefx::e_boolean_readonly_defaults;
				SuperMap<size_t>								sizet_readonly_data = bubblefx::e_sizet_readonly_defaults;
			};

			std::shared_ptr<bubblefx_data> data_bubblefx = std::make_shared<bubblefx_data>();

			// creates new bitmap if needed with these dimensions and clean up old bmp if needed
			void createSwap(const int, const int);
			// checks resolution match with screen
			void _checkInternalBMP();
			// checks update if it has to redraw internally or not
			void _checkUpdateBMP();
			void think();
			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getRef;

			BubbleFX(const size_t = bubblefx::default_bubbles_amount, const double = bubblefx::max_bubbles_framerate);
			BubbleFX(BubbleFX&);

			// set to blindly use this comrade's attributes as theirs (this must be Block)
			void twinUpAttributes(const std::shared_ptr<bubblefx_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<bubblefx_data> getAttributes();

			// reset and create N bubbles. Searching for FPS? set(bubblefx::e_double::FRAMES_PER_SECOND, ...);
			void reset(const size_t);

			void set(const bubblefx::e_double, double);
			void set(const bubblefx::e_double, std::function<double(void)>);
			void set(const std::string, double);
			void set(const std::string, std::function<double(void)>);

			bool get(const bubblefx::e_double, double&);
			bool get(const bubblefx::e_double, std::function<double(void)>&);
			bool get(const std::string, double&);
			bool get(const std::string, std::function<double(void)>&);

			bool get(const bubblefx::e_chronomillis_readonly, std::chrono::milliseconds&);
			bool get(const bubblefx::e_boolean_readonly, bool&);
			bool get(const bubblefx::e_sizet_readonly, size_t&);

			std::function<double(void)>* getRef(const bubblefx::e_double);
			const std::chrono::milliseconds* getRef(const bubblefx::e_chronomillis_readonly) const;
			const bool* getRef(const bubblefx::e_boolean_readonly) const;
			const size_t* getRef(const bubblefx::e_sizet_readonly) const;
		};
	}
}