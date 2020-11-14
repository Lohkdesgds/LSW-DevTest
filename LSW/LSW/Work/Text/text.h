#pragma once

#include "../../Tools/Common/common.h"
#include "../../Tools/CString/cstring.h"
#include "../../Tools/Resource/resource.h"
#include "../../Interface/Color/color.h"
#include "../../Interface/Bitmap/bitmap.h"
#include "../../Interface/Target/target.h"
#include "../../Interface/Font/font.h"
#include "../Sprite/sprite.h"

#undef max

namespace LSW {
	namespace v5 {
		namespace Work {

			namespace text {

				enum class e_chronomillis_readonly { LAST_UPDATE_BITMAP, LAST_UPDATE_STRING };

				enum class e_cstring { STRING };
				enum class e_double { SHADOW_DISTANCE_X, SHADOW_DISTANCE_Y, TEXT_UPDATE_TIME, BUFFER_SCALE_RESOLUTION, UPDATES_PER_SECOND, LINE_ADJUST };
				enum class e_color { SHADOW_COLOR };
				enum class e_integer { STRING_MODE, FONT_SIZE };
				enum class e_boolean { USE_BITMAP_BUFFER, USE_COLOR_INSTEAD_OF_AUTO /*auto: &6gold&1blue*/ };
				enum class e_sprite_ref { FOLLOWING };

				const Tools::SuperMap<Tools::FastFunction<std::chrono::milliseconds>>		e_chronomillis_readonly_defaults = {
					{std::chrono::milliseconds(0),																				(e_chronomillis_readonly::LAST_UPDATE_BITMAP),			("last_update_bitmap")},
					{std::chrono::milliseconds(0),																				(e_chronomillis_readonly::LAST_UPDATE_STRING),			("last_update_string")}
				};
				const Tools::SuperMap<Tools::FastFunction<Tools::Cstring>>					e_string_defaults = {
					{Tools::Cstring(),																							(e_cstring::STRING),									("string")}
				};
				const Tools::SuperMap<Tools::FastFunction<double>>							e_double_defaults = {
					{0.0,																										(e_double::SHADOW_DISTANCE_X),							("shadow_distance_x")},
					{0.0,																										(e_double::SHADOW_DISTANCE_Y),							("shadow_distance_y")},
					{1.0,																										(e_double::BUFFER_SCALE_RESOLUTION),					("buffer_scale_resolution")},
					{2.0,																										(e_double::UPDATES_PER_SECOND),							("updates_per_second")},
					{1.0,																										(e_double::LINE_ADJUST),								("line_adjust")}
				};
				const Tools::SuperMap<Tools::FastFunction<Interface::Color>>				e_color_defaults = {
					{Interface::Color(),																						(e_color::SHADOW_COLOR),								("shadow_color")}
				};
				const Tools::SuperMap<Tools::FastFunction<int>>								e_integer_defaults = {
					{0,																											(e_integer::STRING_MODE),								("string_mode")},
					{75,																										(e_integer::FONT_SIZE),									("font_size")}
				};
				const Tools::SuperMap<Tools::FastFunction<bool>>							e_boolean_defaults = {
					{false,																										(e_boolean::USE_BITMAP_BUFFER),							("use_bitmap_buffer")},
					{false,																										(e_boolean::USE_COLOR_INSTEAD_OF_AUTO),					("use_color_instead_of_auto")}
				};
				const Tools::SuperMap<Tools::FastFunction<Sprite_Base>>						e_sprite_ref_defaults = {
					{Sprite_Base(),																								(e_sprite_ref::FOLLOWING),								("following")}
				};

				enum class e_text_modes { LEFT, CENTER, RIGHT };
			}

			/// <summary>
			/// <para>Text is a Sprite_Base + Font with various features.</para>
			/// <para>If you use custom index, please change the sprite::e_uintptr_t::INDEX_TARGET_IN_USE.</para>
			/// </summary>
			class Text : public Sprite_Base, public Tools::SuperFunctionalMap<std::chrono::milliseconds>, public Tools::SuperFunctionalMap<Tools::Cstring>, public Tools::SuperFunctionalMap<Sprite_Base> {

				Interface::Font fontt;
				Interface::Bitmap buff;

				std::vector<Tools::Cstring> _buf_lines;

				void _think_lines();
				void _draw_text(Interface::Camera&);

				void draw_task(Interface::Camera&) override;
				void think_task(const int) override;
			public:
				using Sprite_Base::set;
				using Sprite_Base::get;
				using Sprite_Base::get_direct;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::set;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get;
				using Tools::SuperFunctionalMap<std::chrono::milliseconds>::get_direct;
				using Tools::SuperFunctionalMap<Tools::Cstring>::set;
				using Tools::SuperFunctionalMap<Tools::Cstring>::get;
				using Tools::SuperFunctionalMap<Tools::Cstring>::get_direct;
				using Tools::SuperFunctionalMap<Sprite_Base>::set;
				using Tools::SuperFunctionalMap<Sprite_Base>::get;
				using Tools::SuperFunctionalMap<Sprite_Base>::get_direct;

				/// <summary>
				/// <para>Empty default start.</para>
				/// </summary>
				Text();

				/// <summary>
				/// <para>Constructor referencing a Text (not a copy).</para>
				/// </summary>
				/// <param name="{Text}">The one to reference attributes from (no copy).</param>
				Text(const Text&);

				/// <summary>
				/// <para>Constructor to move a Text to this (move).</para>
				/// </summary>
				/// <param name="{Text}">The one to move attributes from.</param>
				Text(Text&&);

				/// <summary>
				/// <para>Reference a Text (not a copy).</para>
				/// </summary>
				/// <param name="{Text}">The one to reference attributes from (no copy).</param>
				void operator=(const Text&);

				/// <summary>
				/// <para>Move a Text to this (move).</para>
				/// </summary>
				/// <param name="{Text}">The one to move attributes from.</param>
				void operator=(Text&&);

				/// <summary>
				/// <para>Clone other Text attributes.</para>
				/// <para>You can also clone a specific type by doing set(*get()).</para>
				/// </summary>
				/// <param name="{Text}">Other to copy from.</param>
				void clone(const Text&);

				/// <summary>
				/// <para>Set a Font to be used here.</para>
				/// </summary>
				/// <param name="{Font}">A Font.</param>
				void set(const Interface::Font&);

			};

		}
	}
}