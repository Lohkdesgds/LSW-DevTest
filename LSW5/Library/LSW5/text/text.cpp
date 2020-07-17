#include "text.h"


namespace LSW {
	namespace v5 {

		void Text::__link_hard_task()
		{
			data_text->custom_tags = {
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSX) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0)));
					},
						CHAR_INIT("%pos_x%"), 						(text::e_custom_tags::T_POSX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSY) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0)));
					},
						CHAR_INIT("%pos_y%"), 						(text::e_custom_tags::T_POSY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSX) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0)));
					},
						CHAR_INIT("%screen_pos_x%"), 				(text::e_custom_tags::T_SCREEN_POSX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (*getRef(sprite::e_double_readonly::POSY) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0)));

					},
						CHAR_INIT("%screen_pos_y%"), 				(text::e_custom_tags::T_SCREEN_POSY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						return Tools::sprintf_a("%s", (follow ? "Y" : "N"));
					},
						CHAR_INIT("%is_following%"), 				(text::e_custom_tags::T_ISFOLLOWING)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().r);
					},
						CHAR_INIT("%color_r%"), 					(text::e_custom_tags::T_COLOR_R)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().g);
					},
						CHAR_INIT("%color_g%"), 					(text::e_custom_tags::T_COLOR_G)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().b);
					},
						CHAR_INIT("%color_b%"), 					(text::e_custom_tags::T_COLOR_B)
				},
				{
					[&] {
						return Tools::sprintf_a("%.3f", (*getRef(sprite::e_color::COLOR))().a);
					},
						CHAR_INIT("%color_a%"), 					(text::e_custom_tags::T_COLOR_A)
				},
				{
					[&] {
						return Tools::sprintf_a("%d", (*data_text->integer_data[text::e_integer::STRING_MODE])());
					},
						CHAR_INIT("%mode%"), 						(text::e_custom_tags::T_MODE)
				},
				{
					[&] {
						Database conf;
						return Tools::sprintf_a("%.3lf", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - *conf.getRef(database::e_chronomillis_readonly::STARTED_APP_TIME)).count() / 1000.0);
					},
						CHAR_INIT("%time%"), 						(text::e_custom_tags::T_TIME)
				},
				{
					[&] {
						Database conf;
						return Tools::sprintf_a("%s", *conf.getRef(database::e_boolean::DOUBLE_BUFFERING) ? "Y" : "N");
					},
						CHAR_INIT("%is_using_buf%"), 				(text::e_custom_tags::T_ISUSINGBUF)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_width(d) : -1));

					},
						CHAR_INIT("%curr_res_x%"), 					(text::e_custom_tags::T_GB_RESX)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_height(d) : -1));
					},
						CHAR_INIT("%curr_res_y%"), 					(text::e_custom_tags::T_GB_RESY)
				},
				{
					[&] {
						ALLEGRO_DISPLAY* d = al_get_current_display();
						return Tools::sprintf_a("%d", (d ? al_get_display_refresh_rate(d) : -1));
					},
						CHAR_INIT("%curr_refresh_rate%"), 			(text::e_custom_tags::T_REFRESHRATE)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::FRAMESPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_fps%"), 					(text::e_custom_tags::T_FPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::COLLISIONSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);

					},
						CHAR_INIT("%int_tps%"), 					(text::e_custom_tags::T_TPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::USEREVENTSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_ups%"), 					(text::e_custom_tags::T_UPS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::ADVANCEDFUNCSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_aps%"), 					(text::e_custom_tags::T_APS)
				},
				{
					[&] {
						Database conf;
						size_t t;
						conf.get(database::e_sizet::MUSICSPERSECOND, t);
						return Tools::sprintf_a("%zu", t);
					},
						CHAR_INIT("%int_mps%"), 					(text::e_custom_tags::T_MPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_fps%"), 				(text::e_custom_tags::T_I_FPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_tps%"), 				(text::e_custom_tags::T_I_TPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_ups%"), 				(text::e_custom_tags::T_I_UPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_aps%"), 				(text::e_custom_tags::T_I_APS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_MUSICSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1.0 / dt : -1);
					},
						CHAR_INIT("%instant_mps%"), 				(text::e_custom_tags::T_I_MPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_fps%"), 						(text::e_custom_tags::T_MS_FPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_tps%"), 						(text::e_custom_tags::T_MS_TPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_ups%"), 						(text::e_custom_tags::T_MS_UPS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_aps%"), 						(text::e_custom_tags::T_MS_APS)
				},
				{
					[&] {
						Database conf;
						double dt;
						conf.get(database::e_double::INSTANT_MUSICSPERSECOND, dt);
						return Tools::sprintf_a("%05.1lf", dt ? 1000.0 * dt : -1);
					},
						CHAR_INIT("%ms_aps%"), 						(text::e_custom_tags::T_MS_MPS)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0));
					},
						CHAR_INIT("%cam_x%"), 						(text::e_custom_tags::T_CAM_X)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0));
					},
						CHAR_INIT("%cam_y%"), 						(text::e_custom_tags::T_CAM_Y)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? (*cam->getRef(camera::e_double::SCALE_G) * sqrt(*cam->getRef(camera::e_double::SCALE_X) * *cam->getRef(camera::e_double::SCALE_Y))) : 0.0));
					},
						CHAR_INIT("%cam_zoom_combined%"), 			(text::e_custom_tags::T_CAM_ZOOM)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_G) : 0.0));
					},
						CHAR_INIT("%cam_zoom_g%"), 					(text::e_custom_tags::T_CAM_ZOOMG)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_X) : 0.0));
					},
						CHAR_INIT("%cam_zoom_x%"), 					(text::e_custom_tags::T_CAM_ZOOMX)
				},
				{
					[&] {
						SuperResource<Camera> cameras;
						if (cameras.size() == 0) return std::string("No camera set"); // cannot proceed
						std::shared_ptr<Camera> cam = cameras.getMain(); // get first cam as main camera
						return Tools::sprintf_a("%.3lf", (cam ? *cam->getRef(camera::e_double::SCALE_Y) : 0.0));
					},
						CHAR_INIT("%cam_zoom_y%"), 					(text::e_custom_tags::T_CAM_ZOOMY)
				},
				{
					[&] {
						Database conf;
						std::string str;
						conf.get(database::e_string::CURRENT_STRING, str);
						return str;
					},
						CHAR_INIT("%curr_string%"), 				(text::e_custom_tags::T_CURRSTRING)
				},
				{
					[&] {
						Database conf;
						std::string str;
						conf.get(database::e_string::LAST_STRING, str);
						return str;
					},
						CHAR_INIT("%last_string%"), 				(text::e_custom_tags::T_LASTSTRING)
				},
				{
					[&] {
						Database conf;
						double x;
						conf.get(database::e_double::MOUSE_X, x);
						return Tools::sprintf_a("%.3lf", x);
					},
						CHAR_INIT("%mouse_x%"), 					(text::e_custom_tags::T_MOUSE_X)
				},
				{
					[&] {
						Database conf;
						double x;
						conf.get(database::e_double::MOUSE_Y, x);
						return Tools::sprintf_a("%.3lf", x);
					},
						CHAR_INIT("%mouse_y%"), 					(text::e_custom_tags::T_MOUSE_Y)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							double val;
							follow->get(sprite::e_double_readonly::SPEED_X, val);
							return Tools::sprintf_a("%.3lf", val);
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_speed_x%"), 				(text::e_custom_tags::T_SPRITE_SPEEDX)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							double val;
							follow->get(sprite::e_double_readonly::SPEED_Y, val);
							return Tools::sprintf_a("%.3lf", val);
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_speed_y%"), 				(text::e_custom_tags::T_SPRITE_SPEEDY)
				},
				{
					[&] {
						std::shared_ptr<Sprite_Base> follow = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock();
						if (follow) {
							std::string val;
							follow->get(sprite::e_string::ID, val);
							return val;
						}
						return std::string("NULL");
					},
						CHAR_INIT("%sprite_name%"), 				(text::e_custom_tags::T_SPRITE_NAME)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							if (follow->isThisEntity()) {
								std::string nickname;
								Entity* lmao = (Entity*)follow;
								lmao->get(Constants::io__entity_string::NICKNAME, nickname);
								sprintf_s(tempstr_c, "%s", nickname.c_str());
							}
							else sprintf_s(tempstr_c, "NULL");
						}
						else sprintf_s(tempstr_c, "NULL");*/
					},
						CHAR_INIT("%entity_name%"), 				(text::e_custom_tags::T_SPRITE_ENTITY_NAME)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							if (follow->isThisEntity()) {
								double health = 0.0;
								Entity* lmao = (Entity*)follow;
								lmao->get(Constants::io__entity_double::HEALTH, health);
								sprintf_s(tempstr_c, "%.0lf%c", 100.0 * health, '%');
							}
							else sprintf_s(tempstr_c, "NULL");
						}
						else sprintf_s(tempstr_c, "NULL");*/
					},
						CHAR_INIT("%entity_health%"), 				(text::e_custom_tags::T_SPRITE_ENTITY_HEALTH)
				},
				{
					[&] {
						return std::string("NOT_AVAILABLE");
						// From older version
						/*if (follow) {
							std::string str = follow->__debug_str();

							if (str.length() >= 128) str = str.substr(0, 124) + "...";

							if (str.length() > 0) sprintf_s(tempstr_c, "%s", str.c_str());
							else sprintf_s(tempstr_c, "NO INFORMATION");
						}
						else sprintf_s(tempstr_c, "NOT_IMPLEMENTED");*/
					},
						CHAR_INIT("%sprite_debug%"), 				(text::e_custom_tags::_T_SPRITE_DEBUG)
				},
				{
					[&] {
						SuperResource<ALLEGRO_BITMAP> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_images%"), 					(text::e_custom_tags::T_TEXTURES_LOADED)
				},
				{
					[&] {
						SuperResource<ALLEGRO_FONT> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_fonts%"), 					(text::e_custom_tags::T_FONTS_LOADED)
				},
				{
					[&] {
						SuperResource<Sprite_Base> resource;
						return Tools::sprintf_a("%zu", resource.size());
					},
						CHAR_INIT("%num_sprites%"), 				(text::e_custom_tags::T_SPRITES_LOADED)
				},
				{
					[&] {
						/*ResourceOf<Track> tracks;
						sprintf_s(tempstr_c, "%zu", tracks.size());*/
						return std::string("NOT_AVAILABLE");
					},
						CHAR_INIT("%num_tracks%"), 					(text::e_custom_tags::T_TRACKS_LOADED)
				},
				{
					[&] {
						/*Mixer mixer;
						sprintf_s(tempstr_c, "%.0lf", mixer.getVolume() * 100.0);*/
						return std::string("NOT_AVAILABLE");
					},
						CHAR_INIT("%volume_perc%"), 				(text::e_custom_tags::T_VOLUME)
				},
				{
					[&] {
						return Shared::version_app;
					},
						CHAR_INIT("%version%"), 					(text::e_custom_tags::T_VERSION)
				},
				{
					[&] {
						Database conf;
						if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) {
							double d = 1.0;
							conf.get(database::e_double::RESOLUTION_BUFFER_PROPORTION, d);
							d *= 100.0;
							return Tools::sprintf_a("%.2lf", d);
						}
						return std::string("Disabled");
					},
						CHAR_INIT("%screen_buf_proportion%"), 		(text::e_custom_tags::T_RESOLUTION_PROPORTION)
				},
				{
					[&] {
						Database conf;
						if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) {
							double d = 1.0;
							conf.get(database::e_double::FX_AMOUNT, d);
							d *= 100.0;
							return Tools::sprintf_a("%.2lf", d);
						}
						return std::string("Disabled");
					},
						CHAR_INIT("%screen_chroma_fx%"), 			(text::e_custom_tags::T_CHROMA_FX)
				},
				{
					[&] {
						Database conf;
						int fpp;
						conf.get(database::e_integer::LIMIT_FPS, fpp);
						if (fpp > 0) return Tools::sprintf_a("%d", fpp);
						return std::string("Unlimited");
					},
						CHAR_INIT("%fps_cap%"), 					(text::e_custom_tags::T_FPS_CAP)
				}
			};
		}

		void Text::interpretTags(coloured_string& s)
		{
			size_t ref_p = 0;
#ifdef _DEBUG
			for (auto now_t = MILLI_NOW; MILLI_NOW - now_t < text::timeout_interpret_debugging;)
#else
			for (auto now_t = MILLI_NOW; MILLI_NOW - now_t < text::timeout_interpret;)
#endif
			{
				size_t in_p = (s.substr(ref_p).find('%'));

				if (in_p == std::string::npos || in_p + 1 >= s.size()) return; // cancel
				in_p += ref_p; // absolute position EXACTLY BEHIND % (s.substr(this) = %...

				if (in_p != 0 && s[in_p - 1].ch == '\\') {
					ref_p = in_p + 1;
					continue;
				}

				size_t en_l = (s.substr(in_p + 1).find('%'));

				if (en_l == std::string::npos || en_l >= s.size()) return; // cancel
				en_l += 2; // relative position (length) to in_p (including both %%)

				if (en_l != 0 && s[en_l - 2].ch == '\\') {
					ref_p = in_p + 1;
					continue;
				}

				std::string the_substr = s.substr(in_p, en_l).s_str();

				if (auto res = data_text->custom_tags(the_substr.c_str(), the_substr.length()); res) {
					std::string sec_res = (*res)();
					s = s.substr(0, in_p) + sec_res + s.substr(en_l + in_p);
					ref_p = in_p + 1;
				}
			}

			throw Abort::Abort(__FUNCSIG__, "Took too long interpreting '" + s.s_str() + "'! [Stopped at '" + s.s_str() + "']", Abort::abort_level::GIVEUP);
			return;

			//s = fina;
		}

		ALLEGRO_COLOR Text::hex(const int hx)
		{
			switch (hx) {
			case 0x0:
				return al_map_rgb(0, 0, 0);
			case 0x1:
				return al_map_rgb(0, 0, 170);
			case 0x2:
				return al_map_rgb(0, 170, 0);
			case 0x3:
				return al_map_rgb(0, 170, 170);
			case 0x4:
				return al_map_rgb(170, 0, 0);
			case 0x5:
				return al_map_rgb(170, 0, 170);
			case 0x6:
				return al_map_rgb(170, 170, 0);
			case 0x7:
				return al_map_rgb(170, 170, 170);
			case 0x8:
				return al_map_rgb(84, 84, 84);
			case 0x9:
				return al_map_rgb(84, 84, 255);
			case 0xA:
				return al_map_rgb(84, 255, 84);
			case 0xB:
				return al_map_rgb(84, 255, 255);
			case 0xC:
				return al_map_rgb(255, 84, 84);
			case 0xD:
				return al_map_rgb(255, 84, 255);
			case 0xE:
				return al_map_rgb(255, 255, 84);
			default:
				return al_map_rgb(255, 255, 255);
			}
		}


		void Text::draw_self()
		{
			if (!fontt) {
#ifdef _DEBUG
				throw Abort::Abort(__FUNCSIG__, "No font texture, but called draw anyway!", Abort::abort_level::GIVEUP);
#elif
				load(); // assume you have default texture set up (main)
#endif
			}

			double off_x = 0.0;
			double off_y = 0.0;
			bool use_following_color = (*data_text->boolean_data[text::e_boolean::USE_FOLLOWING_COLOR_INSTEAD])();

			// auto looks smaller, maybe good?
			auto& posx = *getRef(sprite::e_double_readonly::POSX);
			auto& posy = *getRef(sprite::e_double_readonly::POSY);
			const auto s_dist_x = (*data_text->double_data[text::e_double::SHADOW_DISTANCE_X])();
			const auto s_dist_y = (*data_text->double_data[text::e_double::SHADOW_DISTANCE_Y])();
			const auto s_col = (*data_text->color_data[text::e_color::SHADOW_COLOR])();
			auto n_col = (*getRef(sprite::e_color::COLOR))();
			auto& p_str = *data_text->string_readonly_data[text::e_string_readonly::PROCESSED_STRING];
			const auto mode = (*data_text->integer_data[text::e_integer::STRING_MODE])();
			const auto scale_g = (*getRef(sprite::e_double::SCALE_G))();
			const auto scale_x = (*getRef(sprite::e_double::SCALE_X))();
			const auto scale_y = (*getRef(sprite::e_double::SCALE_Y))();
			auto& delta_t = *data_text->chronomillis_readonly_data[text::e_chronomillis_readonly::LAST_UPDATE_STRING];
			const auto per_char = (*data_text->boolean_data[text::e_boolean::USE_PER_CHAR_COLORING])();

			double t_rotation_rad = *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			double p_rotation_rad = 0.0;

			// STRING
			if (std::chrono::system_clock::now().time_since_epoch() > delta_t) // do update string
			{
				delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + text::default_delta_t_text_update_delay);

				p_str = (*data_text->string_data[text::e_cstring::STRING])();
				if (!(*data_text->boolean_data[text::e_boolean::NO_AUTO_STRING_INTERPRETATION])()) interpretTags(p_str);
			}



			if (std::shared_ptr<Sprite_Base> fl = (*data_text->sprite_ptr_data[text::e_sprite_ptr::FOLLOWING])().lock(); fl)
			{
				fl->get(sprite::e_double_readonly::POSX, off_x);
				fl->get(sprite::e_double_readonly::POSY, off_y);
				fl->get(sprite::e_double_readonly::ROTATION, p_rotation_rad);
				p_rotation_rad *= ALLEGRO_PI / 180.0;

				if (use_following_color) {
					fl->get(sprite::e_color::COLOR, n_col);
				}
			}

			SuperResource<Camera> cameras;
			std::shared_ptr<Camera> ruler;
			if (ruler = cameras.getMain(); !ruler) throw Abort::Abort(__FUNCSIG__, "NO MAIN CAMERA HAS BEEN SET UP! Please set up a Camera! (Using SuperResource.setMain())");
			Camera preset;

			//double camx, camy, camg;
			if (*getRef(sprite::e_boolean::AFFECTED_BY_CAM)) preset = *ruler; // copy

			double rotation_rad = t_rotation_rad + p_rotation_rad;

			bool should_care_about_shadow = (s_dist_x != 0.0 || s_dist_y != 0.0);

			double pos_now[2];

			pos_now[0] = 1.0 * text::default_sharpness_font * (((posx)*cos(p_rotation_rad)) - ((posy)*sin(p_rotation_rad)) + off_x); // transformed to sprite's coords
			pos_now[1] = 1.0 * text::default_sharpness_font * (((posy)*cos(p_rotation_rad)) + ((posx)*sin(p_rotation_rad)) + off_y); // transformed to sprite's coords

			double pos_shadow_now[2] = { 0,0 };

			if (should_care_about_shadow) {
				pos_shadow_now[0] = 1.0 * text::default_sharpness_font * (((posx + s_dist_x) * cos(p_rotation_rad)) - ((posy + s_dist_y) * sin(p_rotation_rad)) + off_x);
				pos_shadow_now[1] = 1.0 * text::default_sharpness_font * (((posy + s_dist_y) * cos(p_rotation_rad)) + ((posx + s_dist_x) * sin(p_rotation_rad)) + off_y);
			}

			// not used anymore because of change there on preset.set OFFSET (this moves center to point and then rotates exactly there)

			/*double targ_draw_xy[2];
			targ_draw_xy[0] = pos_now[0];// *cos(rotation_rad) - pos_now[1] * sin(rotation_rad);
			targ_draw_xy[1] = pos_now[1];// *cos(rotation_rad) + pos_now[0] * sin(rotation_rad);
			targ_draw_xy[0] /= scale_x;
			targ_draw_xy[1] /= scale_y;


			double targ_draw_xy_shadow[2] = { 0,0 };

			if (should_care_about_shadow) {
				targ_draw_xy_shadow[0] = pos_shadow_now[0] * cos(rotation_rad) - pos_shadow_now[1] * sin(rotation_rad);
				targ_draw_xy_shadow[1] = pos_shadow_now[1] * cos(rotation_rad) + pos_shadow_now[0] * sin(rotation_rad);
				targ_draw_xy_shadow[0] /= scale_x;
				targ_draw_xy_shadow[1] /= scale_y;
			}*/

			preset.set(camera::e_double::SCALE_G, *preset.getRef(camera::e_double::SCALE_G) * scale_g * 1.0 / text::default_sharpness_font);
			preset.set(camera::e_double::SCALE_X, *preset.getRef(camera::e_double::SCALE_X) * scale_x);
			preset.set(camera::e_double::SCALE_Y, *preset.getRef(camera::e_double::SCALE_Y) * scale_y);
			preset.set(camera::e_double::OFFSET_X, (*preset.getRef(camera::e_double::OFFSET_X) * text::default_sharpness_font / scale_g) - (pos_now[0] / (scale_x * scale_g))); // offset of current cam + move center to its pos
			preset.set(camera::e_double::OFFSET_Y, (*preset.getRef(camera::e_double::OFFSET_Y) * text::default_sharpness_font / scale_g) - (pos_now[1] / (scale_y * scale_g))); // offset of current cam + move center to its pos

			preset.set(camera::e_double::ROTATION_RAD, *preset.getRef(camera::e_double::ROTATION_RAD) + rotation_rad); // now it rotates based on center
			preset.refresh();
			preset.apply();

			if (s_dist_x != 0.0 || s_dist_y != 0.0) {
				al_draw_text(&(*fontt), s_col, 0.0, 0.0, mode, p_str.s_str().c_str());
			}
			if (per_char) {
				//char minibuf[2] = { 0 };
				std::string thebuff;
				int offset_x_f = 0;
				char_c* data_ = p_str.data();

				for (size_t p = 0; p < p_str.size(); p++) {

					auto clr_now = hex(static_cast<int>(data_->cr));

					for (auto _ref = data_->cr; _ref == data_->cr && p < p_str.size();) {
						thebuff += data_->ch;
						data_++;
						p++;
					}

					al_draw_text(&(*fontt), clr_now,
						offset_x_f/* * cos(t_rotation_rad + p_rotation_rad)*/,
						/*- offset_x_f * sin(t_rotation_rad + p_rotation_rad)*/ 0.0,
						mode, thebuff.c_str());

					offset_x_f += al_get_text_width(&(*fontt), thebuff.c_str());
					thebuff.clear();
				}
			}
			else al_draw_text(&(*fontt), n_col, 0.0, 0.0, mode, p_str.s_str().c_str());

			if (isEq(sprite::e_boolean::SHOWDOT, true)) {
				al_draw_filled_circle(
					/* X1: */ 0.0,
					/* Y1: */ 0.0,
					/* SCL */ 2.0f * text::default_sharpness_font,
					al_map_rgba(0, 45, 90, 90));
			}

			ruler->apply();
		}

		Text::Text() : Sprite_Base()
		{
			custom_draw_task = [&] {draw_self(); };
			__link_hard_task();
		}

		Text::Text(Text& o) : Sprite_Base(o)
		{
			custom_draw_task = [&] {draw_self(); };
			__link_hard_task();
		}

		void Text::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_FONT> bmps;

			/*font.ref = bmps.load(id, str);
			font.source = str;
			font.id = id;*/
			fontt = bmps.load(id, str);

			if (!fontt) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");

		}

		void Text::load()
		{
			SuperResource<ALLEGRO_FONT> fonts;
			fontt = fonts.getMain();
			if (!fontt) throw Abort::Abort(__FUNCSIG__, "There's no MAIN FONT set! (See SuperResource.setMain(...))");
		}

		void Text::remove()
		{
			/*font.ref.reset();
			font.source.clear();
			font.id.clear();*/
			fontt.reset();
		}

		void Text::twinUpAttributes(const std::shared_ptr<text_data> oth)
		{
			data_text = oth;
		}

		std::shared_ptr<Text::text_data> Text::getAttributes()
		{
			return data_text;
		}

		void Text::addNewEntry(const std::string id, std::function<std::string(void)> f)
		{
			removeEntry(id);
			data_text->custom_tags.add({ f, CHAR_INIT(('%' + id + '%').c_str()) });
		}

		void Text::removeEntry(const std::string rid)
		{
			const std::string id = ('%' + rid + '%');
			for (size_t p = 0; p < data_text->custom_tags.size(); p++) {
				auto& u = (*(data_text->custom_tags.begin() + p));
				if (u(id.c_str(), id.length())) {
					if (u.hasType<text::e_custom_tags>()) {
						throw Abort::Abort(__FUNCSIG__, "User tried to replace/delete internal tag!", Abort::abort_level::FATAL_ERROR);
						return;
					}
					data_text->custom_tags.erase(p);
					break;
				}
			}
		}

		void Text::set(const text::e_cstring e, coloured_string v)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_double e, double v)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_color e, ALLEGRO_COLOR v)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_integer e, int v)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_sprite_ptr e, std::shared_ptr<Sprite_Base> v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const text::e_boolean e, bool v)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Text::set(const std::string e, coloured_string v)
		{
			auto* ptr = data_text->string_data(e.c_str(), e.length());
			if (!ptr) data_text->string_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, double v)
		{
			auto* ptr = data_text->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, ALLEGRO_COLOR v)
		{
			auto* ptr = data_text->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, int v)
		{
			auto* ptr = data_text->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, std::shared_ptr<Sprite_Base> v)
		{
			auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length());
			if (!ptr) data_text->sprite_ptr_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Text::set(const std::string e, bool v)
		{
			auto* ptr = data_text->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = [=] {return v; };
		}



		void Text::set(const text::e_cstring e, std::function<coloured_string(void)> v)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_color e, std::function<ALLEGRO_COLOR(void)> v)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_integer e, std::function<int(void)> v)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_sprite_ptr e, std::function<std::shared_ptr<Sprite_Base>(void)> v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_boolean e, std::function<bool(void)> v)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const std::string e, std::function<coloured_string(void)> v)
		{
			auto* ptr = data_text->string_data(e.c_str(), e.length());
			if (!ptr) data_text->string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_text->double_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<ALLEGRO_COLOR(void)> v)
		{
			auto* ptr = data_text->color_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<int(void)> v)
		{
			auto* ptr = data_text->integer_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<std::shared_ptr<Sprite_Base>(void)> v)
		{
			auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length());
			if (!ptr) data_text->sprite_ptr_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, std::function<bool(void)> v)
		{
			auto* ptr = data_text->boolean_data(e.c_str(), e.length());
			if (!ptr) static_cast<Sprite_Base*>(this)->set(e, v);
			else *ptr = v;
		}

		bool Text::get(const text::e_string_readonly e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_cstring e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_double e, double& v)
		{
			if (auto* ptr = data_text->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_text->color_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_integer e, int& v)
		{
			if (auto* ptr = data_text->integer_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_sprite_ptr e, std::shared_ptr<Sprite_Base>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data[e]; ptr)
			{
				v = (*ptr)().lock();
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_boolean e, bool& v)
		{
			if (auto* ptr = data_text->boolean_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, coloured_string& v)
		{
			if (auto* ptr = data_text->string_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, double& v)
		{
			if (auto* ptr = data_text->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data_text->color_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, int& v)
		{
			if (auto* ptr = data_text->integer_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::shared_ptr<Sprite_Base>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)().lock();
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, bool& v)
		{
			if (auto* ptr = data_text->boolean_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}



		bool Text::get(const text::e_cstring e, std::function<coloured_string(void)>& v)
		{
			if (auto* ptr = data_text->string_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_text->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_color e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_text->color_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_integer e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_text->integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_sprite_ptr e, std::function<std::weak_ptr<Sprite_Base>(void)>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_boolean e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_text->boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<coloured_string(void)>& v)
		{
			if (auto* ptr = data_text->string_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_text->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<ALLEGRO_COLOR(void)>& v)
		{
			if (auto* ptr = data_text->color_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_text->integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		bool Text::get(const std::string e, std::function<std::weak_ptr<Sprite_Base>(void)>& v)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_text->boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return static_cast<Sprite_Base*>(this)->get(e, v);
		}

		std::function<coloured_string(void)>* Text::getRef(const text::e_cstring e)
		{
			if (auto* ptr = data_text->string_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<double(void)>* Text::getRef(const text::e_double e)
		{
			if (auto* ptr = data_text->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<ALLEGRO_COLOR(void)>* Text::getRef(const text::e_color e)
		{
			if (auto* ptr = data_text->color_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<int(void)>* Text::getRef(const text::e_integer e)
		{
			if (auto* ptr = data_text->integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<std::weak_ptr<Sprite_Base>(void)>* Text::getRef(const text::e_sprite_ptr e)
		{
			if (auto* ptr = data_text->sprite_ptr_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<bool(void)>* Text::getRef(const text::e_boolean e)
		{
			if (auto* ptr = data_text->boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const coloured_string* Text::getRef(const text::e_string_readonly e) const
		{
			if (auto* ptr = data_text->string_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::chrono::milliseconds* Text::getRef(const text::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data_text->chronomillis_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

	}
}