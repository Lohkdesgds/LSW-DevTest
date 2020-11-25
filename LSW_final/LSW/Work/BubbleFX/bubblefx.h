#pragma once

#include "../../Tools/Common/common.h"
#include "../../Interface/Target/target.h"
#include "../Sprite/sprite.h"

namespace LSW {
	namespace v5 {
		namespace Work {

			namespace bubblefx {
				enum class e_uintptrt_readonly { VECTOR_POSITION_DRAWING, LAST_TIE_FRAME_VERIFICATION }; // SAME AS UINTPTR_T!
				enum class e_uintptrt { DOTS_PER_FRAME_MAX };
				enum class e_chronomillis_readonly { LAST_FRAME, LAST_TIE_FRAME_VERIFICATION };
				enum class e_double { FRAMES_PER_SECOND, DOTS_SIZE, DOTS_VARIATION_PLUS, BLUR_INTENSITY, TIE_SIZE_TO_DISPLAY_PROPORTION };

				const Tools::SuperMap<Tools::FastFunction<uintptr_t>>						e_uintptrt_defaults = {
					{(size_t)0,																						(e_uintptrt_readonly::VECTOR_POSITION_DRAWING),			"vector_position_drawing"},
					{(size_t)250,																					(e_uintptrt::DOTS_PER_FRAME_MAX),						"dots_per_frame_max"}
				};
				const Tools::SuperMap<Tools::FastFunction<std::chrono::milliseconds>>		e_chronomillis_defaults = {
					{std::chrono::milliseconds(0),																	(e_chronomillis_readonly::LAST_FRAME),					"last_frame"},
					{std::chrono::milliseconds(0),																	(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	"last_tie_verification"}
				};
				const Tools::SuperMap<Tools::FastFunction<double>>							e_double_defaults = {
					{60.0,																							(e_double::FRAMES_PER_SECOND),							"frames_per_second"},
					{0.055,																							(e_double::DOTS_SIZE),									"dots_size"},
					{0.025,																							(e_double::DOTS_VARIATION_PLUS),						"dots_variation_plus"}, // sum with size, [size, size+this)
					{0.93,																							(e_double::BLUR_INTENSITY),								"blur_intensity"},
					{1.0,																							(e_double::TIE_SIZE_TO_DISPLAY_PROPORTION),				"tie_size_to_display_proportion"}
				};

				struct particle {
					double posx = 0.0, posy = 0.0;
					double lastsize = 1.0;
					double lastpositionscalculated[2] = { 0.0, 0.0 };
				};

				constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast (see also block::...)
			}

			class BubbleFX : public Sprite_Base, public Tools::SuperFunctionalMap<std::chrono::milliseconds> {
				Interface::Bitmap buffer;

				// as this is random, it might not be copyable or settable
				std::vector<bubblefx::particle> positions;

				void check_internal();

				void draw_task(Interface::Camera&);
			public:
				using Sprite_Base::set;
				using Sprite_Base::get;
				using Sprite_Base::get_direct;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::set;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get_direct;

				/// <summary>
				/// <para>Empty default start.</para>
				/// </summary>
				BubbleFX();

				/// <summary>
				/// <para>Constructor referencing a BubbleFX (not a copy).</para>
				/// </summary>
				/// <param name="{BubbleFX}">The one to reference attributes from (no copy).</param>
				BubbleFX(const BubbleFX&);

				/// <summary>
				/// <para>Constructor to move a BubbleFX to this (move).</para>
				/// </summary>
				/// <param name="{BubbleFX}">The one to move attributes from.</param>
				BubbleFX(BubbleFX&&);

				/// <summary>
				/// <para>Reference a BubbleFX (not a copy).</para>
				/// </summary>
				/// <param name="{BubbleFX}">The one to reference attributes from (no copy).</param>
				void operator=(const BubbleFX&);

				/// <summary>
				/// <para>Move a BubbleFX to this (move).</para>
				/// </summary>
				/// <param name="{BubbleFX}">The one to move attributes from.</param>
				void operator=(BubbleFX&&);

				/// <summary>
				/// <para>Clone other BubbleFX attributes.</para>
				/// <para>You can also clone a specific type by doing set(*get()).</para>
				/// </summary>
				/// <param name="{BubbleFX}">Other to copy from.</param>
				void clone(const BubbleFX&);

				/// <summary>
				/// <para>Resets/initialize positions (again) with given amount of positions (dots).</para>
				/// </summary>
				void reset_positions(const size_t&);
			};
		}
	}
}