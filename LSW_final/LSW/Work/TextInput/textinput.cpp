#include "textinput.h"

namespace LSW {
	namespace v5 {
		namespace Work {

			void TextInput::_apply_latest()
			{
				Text* here_i_am = ((Text*)this);
				auto fin = here_i_am->get_direct<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER).refresh();
				here_i_am->set<Tools::Cstring>(text::e_cstring::STRING, fin);
				if (auto f = here_i_am->get_direct<sprite::functional>(textinput::e_tie_functional::SAVED_STRING); f) f(fin);
			}

			void TextInput::_rec_string()
			{
				Text* here_i_am = ((Text*)this);
				auto _res = here_i_am->get_direct<Tools::Cstring>(text::e_cstring::STRING);
				here_i_am->set<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER, _res);
				here_i_am->set<Tools::Cstring>(text::e_cstring::STRING, [_res]{ return _res + ((int)al_get_time() % 2 ? "_" : ""); });
			}
			
			void TextInput::handle_event(const Interface::RawEvent& ev)
			{
				Text* here_i_am = ((Text*)this);
				if (!here_i_am->get_direct<bool>(textinput::e_boolean_readonly::SELECTED)) return;

				const auto enter_newline = here_i_am->get_direct<bool>(textinput::e_boolean::ENTER_BREAK_LINE);

				switch (ev.type()) {
				case ALLEGRO_EVENT_KEY_CHAR:
				{
					const bool was_enter = ev.keyboard_event().keycode == ALLEGRO_KEY_ENTER || ev.keyboard_event().keycode == ALLEGRO_KEY_PAD_ENTER;

					if (ev.keyboard_event().unichar >= 32)
					{
						char multibyte[8] = { 0 };

						auto len = al_utf8_encode(multibyte, ev.keyboard_event().unichar <= 32 ? ' ' : ev.keyboard_event().unichar);
						std::string cpyh;
						for (size_t g = 0; g < len && g < 8; g++) cpyh += multibyte[g];

						Tools::Cstring now = here_i_am->get_direct<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER);
						here_i_am->set<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER, now + cpyh);
						here_i_am->set<Tools::Cstring>(text::e_cstring::STRING, [_res = now + cpyh] {return _res + ((int)al_get_time() % 2 ? "_" : ""); });
					}
					else if (ev.keyboard_event().keycode == ALLEGRO_KEY_BACKSPACE)
					{
						Tools::Cstring now = here_i_am->get_direct<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER);
						now.pop_utf8();
						here_i_am->set<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER, now);
						here_i_am->set<Tools::Cstring>(text::e_cstring::STRING, [_res = now]{ return _res + ((int)al_get_time() % 2 ? "_" : ""); });
					}
					else if ((was_enter && !enter_newline) || ev.keyboard_event().keycode == ALLEGRO_KEY_ESCAPE)
					{
						here_i_am->set(textinput::e_boolean_readonly::SELECTED, false);
						_apply_latest();
					}
					else if (was_enter && enter_newline)
					{
						Tools::Cstring now = here_i_am->get_direct<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER);
						here_i_am->set<Tools::Cstring>(textinput::e_cstring_readonly::BUFFER, now + '\n');
						here_i_am->set<Tools::Cstring>(text::e_cstring::STRING, [_res = now + '\n']{ return _res + ((int)al_get_time() % 2 ? "_" : ""); });
					}
				}
				break;
				}
			}

			void TextInput::mouse_event(const sprite::e_tie_functional ev_type, const Tools::Any& data)
			{
				Text* here_i_am = ((Text*)this);

				switch (ev_type) {
				case sprite::e_tie_functional::COLLISION_MOUSE_CLICK:
					if (!here_i_am->get_direct<bool>(textinput::e_boolean_readonly::SELECTED)) {
						_rec_string();
						here_i_am->set(textinput::e_boolean_readonly::SELECTED, true);
					}
					break;
				case sprite::e_tie_functional::COLLISION_MOUSE_OFF:
					if (here_i_am->get_direct<bool>(textinput::e_boolean_readonly::SELECTED)) {
						here_i_am->set(textinput::e_boolean_readonly::SELECTED, false);
						_apply_latest();
					}
					break;
				}
			}

			Text& TextInput::main()
			{
				return *((Text*)this);
			}
			
			const Text& TextInput::main() const
			{
				return *((Text*)this);
			}

			TextInput::TextInput()
				: Button(), keyboard_event(Interface::get_keyboard_event())
			{
				get_text().set<bool>(textinput::e_boolean_defaults);
				get_text().set<Tools::Cstring>(textinput::e_cstring_defaults);
				get_text().set<sprite::functional>(textinput::e_functional_defaults);

				event_handler.add(keyboard_event);
				event_handler.set_run_autostart([&](const Interface::RawEvent& e) {handle_event(e);});
			}

		}
	}
}