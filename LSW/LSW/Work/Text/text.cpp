#include "text.h"

namespace LSW {
	namespace v5 {
		namespace Work {

			void Text::_draw_text(Interface::Camera& ruler)
			{
				double off_x = 0.0;
				double off_y = 0.0;

				// auto looks smaller, maybe good?
				const auto s_dist_x = get_direct<double>(text::e_double::SHADOW_DISTANCE_X);
				const auto s_dist_y = get_direct<double>(text::e_double::SHADOW_DISTANCE_Y);
				const auto s_col = get_direct<Interface::Color>(text::e_color::SHADOW_COLOR);
				const auto mode = get_direct<int>(text::e_integer::STRING_MODE);
				const auto scale_g = get_direct<double>(sprite::e_double::SCALE_G);
				const auto scale_x = get_direct<double>(sprite::e_double::SCALE_X);
				const auto scale_y = get_direct<double>(sprite::e_double::SCALE_Y);
				const auto posx = get_direct<double>(sprite::e_double_readonly::POSX);
				const auto posy = get_direct<double>(sprite::e_double_readonly::POSY);
				const auto font_siz = get_direct<int>(text::e_integer::FONT_SIZE);
				const auto lineadj = get_direct<double>(text::e_double::LINE_ADJUST);

				const double t_rotation_rad = get_direct<double>(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
				double p_rotation_rad = 0.0;

				if (font_siz <= 0) throw Handling::Abort(__FUNCSIG__, "Invalid font size! Please fix text::e_integer::FONT_SIZE.", Handling::abort::abort_level::GIVEUP);

				{
					auto fl = get_direct<Sprite_Base>(text::e_sprite_ref::FOLLOWING);
					fl.get(sprite::e_double_readonly::POSX, off_x);
					fl.get(sprite::e_double_readonly::POSY, off_y);
					fl.get(sprite::e_double_readonly::ROTATION, p_rotation_rad);
					p_rotation_rad *= ALLEGRO_PI / 180.0;
				}

				const double rotation_rad = t_rotation_rad + p_rotation_rad;

				const bool should_care_about_shadow = (s_dist_x != 0.0 || s_dist_y != 0.0);

				double pos_now[2];

				pos_now[0] = 1.0/* * text::default_sharpness_font*/ * (((posx)*cos(p_rotation_rad)) - ((posy)*sin(p_rotation_rad)) + off_x); // transformed to sprite's coords
				pos_now[1] = 1.0/* * text::default_sharpness_font*/ * (((posy)*cos(p_rotation_rad)) + ((posx)*sin(p_rotation_rad)) + off_y); // transformed to sprite's coords



				auto& cls = ruler.get_classic();
				ruler.classic_transform((cls.x - pos_now[0]), (cls.y - pos_now[1]), cls.sx * scale_g * scale_x / font_siz, cls.sy * scale_g * scale_y / font_siz, cls.rot + rotation_rad);
				ruler.apply();


				const double height = lineadj * fontt.get_line_height();

				for (size_t o = 0; o < _buf_lines.size(); o++) {
					const auto& i = _buf_lines[o];

					if (should_care_about_shadow) fontt.draw(s_col, s_dist_x * 1.0 / ruler.get_classic().sx, height * o +  s_dist_y * 1.0 / ruler.get_classic().sy, mode, i.s_str().c_str());
					fontt.draw(0.0, height * o, mode, i);
				}


				if (is_eq(sprite::e_boolean::SHOWDOT, true)) {
					al_draw_filled_circle(
						/* X1: */ 0.0,
						/* Y1: */ 0.0,
						/* SCL */ 2.0f * font_siz,
						al_map_rgba(0, 45, 90, 90));
				}
			}
			
			void Text::draw_task(Interface::Camera& ruler)
			{
				if (!fontt) throw Handling::Abort(__FUNCSIG__, "No Font texture!", Handling::abort::abort_level::GIVEUP);

				const auto delta_t = get_direct<std::chrono::milliseconds>(text::e_chronomillis_readonly::LAST_UPDATE_STRING);
				const auto use_buffer = get_direct<bool>(text::e_boolean::USE_BITMAP_BUFFER);
				const auto scale_buff = get_direct<double>(text::e_double::BUFFER_SCALE_RESOLUTION);
				const auto ups_val = get_direct<double>(text::e_double::UPDATES_PER_SECOND);

				if (ups_val <= 0.0)	throw Handling::Abort(__FUNCSIG__, "Invalid UPS value! (UPDATES_PER_SECOND must be >= 0.0)", Handling::abort::abort_level::GIVEUP);

				// STRING
				if (std::chrono::system_clock::now().time_since_epoch() > delta_t) // do update string
				{
					set(text::e_chronomillis_readonly::LAST_UPDATE_STRING, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + std::chrono::milliseconds((unsigned long long)(1000.0 / ups_val)));


					if (use_buffer) {
						if (scale_buff <= 0.0 || !buff.copy_reference_attributes(true, scale_buff)) { // ensure loaded
							throw Handling::Abort(__FUNCSIG__, "Failed to copy reference's attributes and generate bitmap.", Handling::abort::abort_level::GIVEUP);
						}
					}


					auto p_str = get_direct<Tools::Cstring>(text::e_cstring::STRING);
					_buf_lines.clear();

					size_t pos = 0;
					while ((pos = p_str.find('\n')) != std::string::npos) {
						_buf_lines.push_back(std::move(p_str.substr(0, pos)));
						if (pos + 1 >= p_str.size()) break;
						p_str = std::move(p_str.substr(pos + 1));
					}
					if (p_str.size() > 0) _buf_lines.push_back(std::move(p_str));


					if (use_buffer) {
						if (buff.empty()) return;

						buff.set_as_target();
						al_clear_to_color(Interface::Color(0, 0, 0, 0));
						auto cpy = ruler;
						cpy.classic_update(buff);
						_draw_text(cpy);
						buff.set_reference_as_target();
					}
					else buff.reset();
				}

				if (!use_buffer) {
					_draw_text(ruler);
				}
				else if (!buff.empty()){ // same as Block
					ruler.classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);
					ruler.apply();

					buff.draw(0, 0, buff.get_width(), buff.get_height(), -1.0f, -1.0f, 2.0f, 2.0f);
				}

			}

			Text::Text() : Sprite_Base()
			{
				set<std::chrono::milliseconds>(text::e_chronomillis_readonly_defaults);
				set<Tools::Cstring>(text::e_string_defaults);
				set<Sprite_Base>(text::e_sprite_ref_defaults);
				set<double>(text::e_double_defaults);
				set<Interface::Color>(text::e_color_defaults);
				set<int>(text::e_integer_defaults);
				set<bool>(text::e_boolean_defaults);

				set(Work::text::e_color::SHADOW_COLOR, Interface::Color(0, 0, 0));
				set(text::e_chronomillis_readonly::LAST_UPDATE_STRING, MILLI_NOW);
				set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));
			}

			Text::Text(const Text& other) : Sprite_Base(other)
			{
				*this = other;
			}
			
			Text::Text(Text&& other) : Sprite_Base(std::move(other))
			{
				*this = std::move(other);
			}

			void Text::operator=(const Text& oth)
			{
				this->Sprite_Base::operator=(oth);

				fontt = oth.fontt;

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(oth.get<std::chrono::milliseconds>());
				set<Tools::Cstring>(oth.get<Tools::Cstring>());
				set<Sprite_Base>(oth.get<Sprite_Base>());
			}

			void Text::operator=(Text&& oth)
			{
				this->Sprite_Base::operator=(std::move(oth));

				fontt = std::move(oth.fontt);

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(std::move(oth.get<std::chrono::milliseconds>()));
				set<Tools::Cstring>(std::move(oth.get<Tools::Cstring>()));
				set<Sprite_Base>(std::move(oth.get<Sprite_Base>()));
			}

			void Text::clone(const Text& oth)
			{
				this->Sprite_Base::clone(oth);

				fontt = oth.fontt;

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(*oth.get<std::chrono::milliseconds>());
				set<Tools::Cstring>(*oth.get<Tools::Cstring>());
				set<Sprite_Base>(*oth.get<Sprite_Base>());
			}

			void Text::set(const Interface::Font& f)
			{
				fontt = f;
			}

		}
	}
}