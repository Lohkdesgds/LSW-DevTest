#pragma once

#include "../../Tools/SuperFunctionalMap/superfunctionalmap.h"
#include "../../Interface/Events/events.h"
#include "../../Interface/EventTimer/eventtimer.h"
#include "../Button/button.h"

#undef max

namespace LSW {
	namespace v5 {
		namespace Work {

			namespace textinput {
				enum class e_boolean_readonly { SELECTED };
				enum class e_cstring_readonly { BUFFER };

				enum class e_boolean { ENTER_BREAK_LINE };

				enum class e_tie_functional {
					SAVED_STRING = 0 // expect Cstring
				};

				const Tools::SuperMap<Tools::FastFunction<bool>>		e_boolean_defaults = {
					{false,							(e_boolean_readonly::SELECTED),					("selected")},
					{false,							(e_boolean::ENTER_BREAK_LINE),					("enter_break_line")}
				};
				const Tools::SuperMap<Tools::FastFunction<Tools::Cstring>>	e_cstring_defaults = {
					{Tools::Cstring(),				(e_cstring_readonly::BUFFER),					("buffer")}
				};
				const Tools::SuperMap<Tools::FastFunction<sprite::functional>> e_functional_defaults = {
					{sprite::functional(),			(e_tie_functional::SAVED_STRING)}
				};

			}

			/// <summary>
			/// <para>TextInput is a Button with a keyboard and mouse events already linked so you just click and type, or click somewhere else.</para>
			/// <para>The Block will handle the events, while Text shows what you type.</para>
			/// <para>Because of the multithread feature, you cannot reference or move this.</para>
			/// <para>The textinput values are saved into the TEXT parent part because of the CSTRING data. Access it with no problem using main()!</para>
			/// <para>———————————————————————————————————————————————————————</para>
			/// <para># Things you SHOULD NOT change:</para>
			/// <para>- text::e_sprite_ref::FOLLOWING (Text part)</para>
			/// <para>- text::e_cstring::STRING (Text part, it will be overwritten)</para>
			/// </summary>
			class TextInput : protected Button {
				Interface::Event keyboard_event;
				Interface::EventHandler event_handler;

				void _apply_latest();
				void _rec_string();

				void handle_event(const Interface::RawEvent&);

				void mouse_event(const sprite::e_tie_functional, const Tools::Any&) override;
			public:
				using Button::clone;
				using Block::collide;
				using Button::draw;
				using Button::get_block;
				using Button::get_text;
				using Block::update_and_clear;
				
				Text& main();
				const Text& main() const;

				TextInput();
				TextInput(TextInput&&) = delete;
				TextInput(const TextInput&&) = delete;
				void operator=(TextInput&&) = delete;
				void operator=(const TextInput&&) = delete;
			};

		}
	}
}