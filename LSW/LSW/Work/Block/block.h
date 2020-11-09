#pragma once

#include "../../Tools/Common/common.h"
#include "../../Interface/Bitmap/bitmap.h"
#include "../Sprite/sprite.h"

#undef max

namespace LSW {
	namespace v5 {
		namespace Work {

			
			namespace block {

				enum class e_chronomillis_readonly { LAST_FRAME, LAST_TIE_FRAME_VERIFICATION };
				enum class e_integer { FRAME };
				enum class e_boolean { SET_FRAME_VALUE_READONLY };
				enum class e_double { FRAMES_PER_SECOND, TIE_SIZE_TO_DISPLAY_PROPORTION };

				const Tools::SuperMap<Tools::sfm_safe_feature<std::chrono::milliseconds>>		e_chronomillis_readonly_defaults = {
					{std::chrono::milliseconds(0),																		(e_chronomillis_readonly::LAST_FRAME),					("last_frame")},
					{std::chrono::milliseconds(0),																		(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	("last_tie_verification")}
				};
				const Tools::SuperMap<Tools::sfm_safe_feature<int>>			e_integer_defaults = {
					{0,																									(e_integer::FRAME),										("frame")}
				};
				const Tools::SuperMap<Tools::sfm_safe_feature<bool>>		e_boolean_defaults = {
					{false,																								(e_boolean::SET_FRAME_VALUE_READONLY),					("set_frame_readonly")}
				};
				const Tools::SuperMap<Tools::sfm_safe_feature<double>>		e_double_defaults = {
					{10.0,																								(e_double::FRAMES_PER_SECOND),							("frames_per_second")},
					{0.0,																								(e_double::TIE_SIZE_TO_DISPLAY_PROPORTION),				("tie_size_to_display_proportion")}
				};

				constexpr size_t maximum_bitmap_amount = static_cast<size_t>(std::numeric_limits<int>::max());
				constexpr size_t static_frame = static_cast<size_t>(-1); // static frame X?
				constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast (see also bubblefx::...)
			}

			class Block : public Sprite_Base, public Tools::SuperFunctionalMap<std::chrono::milliseconds> { // Sprite_Base already has others included!!!!!!!

				//std::shared_ptr<block_data> data_block = std::make_shared<block_data>();

				// bitmap internally uses shared_ptr so no worries!
				std::vector<Interface::Bitmap> bitmaps;


				//Bitmap* bmp = nullptr;

				Interface::Bitmap reference;
				void draw_self();
			public:
				using Sprite_Base::set;
				using Sprite_Base::get;
				using Sprite_Base::get_direct;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::set;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get_direct;

				Block();
				Block(const Block&);
				Block(Block&&);


				void operator=(const Block&);
				void operator=(Block&&);
				//~Block(); // bitmaps are handled by SuperResource, there might be some other people around using the same resource


				// set to blindly use this comrade's attributes as theirs (this must be Block)
				void clone(const Block&);


				// add new copy
				void insert(const Interface::Bitmap&);
				// removes from its list all matches
				void remove(const std::function<bool(const Interface::Bitmap&)>);
			};


		}
	}
}