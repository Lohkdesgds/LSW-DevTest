#include "entities.h"

namespace LSW {
	namespace v5 {


		void Block::draw_self()
		{
			if (bitmaps.empty()) return;

			{
				auto& delta_t = *data.chronomillis_readonly_data[entity::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION];

				if (double _dd = *data.double_data[entity::e_double::TIE_SIZE_TO_DISPLAY_PROPORTION]; _dd > 0.0 && (std::chrono::system_clock::now().time_since_epoch() > delta_t)) {
					delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + entity::default_delta_t_frame_delay);
					ALLEGRO_BITMAP* trg = al_get_target_bitmap();
					SuperResource<ALLEGRO_BITMAP> bmps;

					if (trg) {
						for (auto& i : bitmaps) {
							auto nww = bmps.customLoad(i.id, [&](ALLEGRO_BITMAP*& b) {return b = al_create_bitmap(al_get_bitmap_width(trg), al_get_bitmap_height(trg)); });
							al_set_target_bitmap(&(*nww));
							al_draw_scaled_bitmap(&(*i.ref), 0, 0, al_get_bitmap_width(&(*i.ref)), al_get_bitmap_height(&(*i.ref)), 0, 0, al_get_bitmap_width(trg), al_get_bitmap_height(trg), 0);
							i.ref.swap(nww);
							nww.reset();
						}
						al_set_target_bitmap(trg);
					}
				}
			}

			int& frame = *data.integer_data[entity::e_integer::FRAME];

			{
				double& delta = *data.double_data[entity::e_double::FRAMES_PER_SECOND]; // delta t, 1/t = sec
				std::chrono::milliseconds& last_time = *data.chronomillis_readonly_data[entity::e_chronomillis_readonly::LAST_FRAME];

				if (delta >= 0.0 && frame >= 0) { // if delta <= 0 or frame < 0, static
					std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));
					if (delta_tr.count() > 0) {
						while (std::chrono::system_clock::now().time_since_epoch() > last_time) {
							last_time += delta_tr;
							if (++frame >= bitmaps.size()) frame = 0;
						}
					}
				}
				else {
					frame = frame > 0 ? frame : -frame;
					if (frame >= bitmaps.size()) frame = bitmaps.size() - 1;
				}
			}

			ALLEGRO_BITMAP* rn = &(*bitmaps[frame].ref);
			if (!rn) throw Abort::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


			float cx, cy, px, py, dsx, dsy, rot_rad;
			int bmpx, bmpy;
			bmpx = al_get_bitmap_width(rn);
			bmpy = al_get_bitmap_height(rn);
			if (bmpx <= 0 || bmpy <= 0) {
				throw Abort::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
			}

			cx = 1.0f * bmpx * ((*getRef(sprite::e_double::CENTER_X) + 1.0) * 0.5);
			cy = 1.0f * bmpy * ((*getRef(sprite::e_double::CENTER_Y) + 1.0) * 0.5);
			rot_rad = 1.0f * *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			/*px = 1.0f * data.dval[+Constants::io__sprite_double::POSX] * cos(rot_rad) + data.dval[+Constants::io__sprite_double::POSY] * sin(rot_rad);
			py = 1.0f * data.dval[+Constants::io__sprite_double::POSY] * cos(rot_rad) - data.dval[+Constants::io__sprite_double::POSX] * sin(rot_rad);*/
			px = *getRef(sprite::e_double_readonly::POSX);
			py = *getRef(sprite::e_double_readonly::POSY);
			dsx = 1.0f * *getRef(sprite::e_double::SCALE_X) * *getRef(sprite::e_double::SCALE_G) * (1.0 / bmpx);
			dsy = 1.0f * *getRef(sprite::e_double::SCALE_Y) * *getRef(sprite::e_double::SCALE_G) * (1.0 / bmpy);


			if (*getRef(sprite::e_boolean::USE_COLOR)) {
				al_draw_tinted_scaled_rotated_bitmap(rn, *getRef(sprite::e_color::COLOR), cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}
			else {
				al_draw_scaled_rotated_bitmap(rn, cx, cy, px, py, dsx, dsy, rot_rad, 0);
			}

		}
		Block::Block() : Sprite_Base()
		{
			custom_draw_task = [&] {draw_self(); };
		}
		Block::Block(Block& o) : Sprite_Base(o)
		{
			custom_draw_task = [&] {draw_self(); };
		}
		/*Block::~Block()
		{
			if (bmp) {
				al_destroy_bitmap(bmp);
				bmp = nullptr;
			}
		}*/
		void Block::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			_bitmap binfo;

			binfo.ref = bmps.load(id, str);
			binfo.is_sub_bitmap = false;
			binfo.source = str;
			binfo.id = id;

			if (!binfo.ref) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");

			bitmaps.push_back(binfo);

		}
		void Block::loadCut(const std::string id, const std::string parent_id, const int x, const int y, const int w, const int h)
		{
			SuperResource<ALLEGRO_BITMAP> bmps;
			std::shared_ptr<ALLEGRO_BITMAP> parent;
			if (bmps.get(parent_id, parent))
			{
				_bitmap binfo;

				binfo.source = parent_id;
				binfo.is_sub_bitmap = true;
				binfo.id = id;
				binfo.ref = bmps.customLoad(id, [=](ALLEGRO_BITMAP*& ret) {return ret = al_create_sub_bitmap(&(*parent), x, y, w, h); });

				if (binfo.ref)
				{
					bitmaps.push_back(binfo);
					return;
				}
			}
			throw Abort::Abort(__FUNCSIG__, "Cannot get sub part of '" + parent_id + "'!");
		}
		void Block::remove(const std::string id)
		{
			for (size_t p = 0; p < bitmaps.size(); p++) {
				auto& i = bitmaps[p];
				if (i.id == id) {
					bitmaps.erase(bitmaps.begin() + p);
				}
			}
		}
		void Block::set(const entity::e_integer e, int v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}
		void Block::set(const entity::e_double e, double v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Block::set(const std::string e, int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}
		void Block::set(const std::string e, double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Block::get(const entity::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}
		bool Block::get(const entity::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Block::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}
		bool Block::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Block::get(const entity::e_chronomillis_readonly e, std::chrono::milliseconds& v)
		{
			if (auto* ptr = data.chronomillis_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		int* Block::getRef(const entity::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		double* Block::getRef(const entity::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::chrono::milliseconds* Block::getRef(const entity::e_chronomillis_readonly e) const
		{
			if (auto* ptr = data.chronomillis_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}


		void Text::interpretTags(std::string& s)
		{
			std::chrono::milliseconds timing = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			std::string fina = s;

			for (bool b = true; b;)
			{
				b = false;
				for (size_t q = 0; q < static_cast<size_t>(text::tags_e::size); q++)
				{
					size_t poss = 0;
					if ((poss = fina.find(text::tags[q].s())) != std::string::npos)
					{
						b = true;
						Camera gcam;
						Camera* cam = gcam.getLastCameraApply();
						Sprite_Base* follow = *data.sprite_ptr_data[text::e_sprite_ptr::FOLLOWING];
						Database conf;
						ALLEGRO_DISPLAY* d = al_get_current_display();
						char tempstr_c[128];
						//gfile logg;

						switch (q)
						{
						case static_cast<size_t>(text::tags_e::T_POSX):
							sprintf_s(tempstr_c, "%.3lf", (*getRef(sprite::e_double_readonly::POSX) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0)));
							break;
						case static_cast<size_t>(text::tags_e::T_POSY):
							sprintf_s(tempstr_c, "%.3lf", (*getRef(sprite::e_double_readonly::POSY) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) + (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0)));
							break;
						case static_cast<size_t>(text::tags_e::T_SCREEN_POSX):
							sprintf_s(tempstr_c, "%.3lf", (*getRef(sprite::e_double_readonly::POSX) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSX) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_X)) : 1.0)));
							break;
						case static_cast<size_t>(text::tags_e::T_SCREEN_POSY):
							sprintf_s(tempstr_c, "%.3lf", (*getRef(sprite::e_double_readonly::POSY) * (*getRef(sprite::e_boolean::AFFECTED_BY_CAM) ? (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0) : 1.0) + (follow ? *follow->getRef(sprite::e_double_readonly::POSY) : 0.0) * (cam ? (*cam->getRef(camera::e_double::SCALE_G) * *cam->getRef(camera::e_double::SCALE_Y)) : 1.0)));
							break;

						case static_cast<size_t>(text::tags_e::T_CAM_X):
							sprintf_s(tempstr_c, "%.3f", (cam ? *cam->getRef(camera::e_double::OFFSET_X) : 0.0));
							break;
						case static_cast<size_t>(text::tags_e::T_CAM_Y):
							sprintf_s(tempstr_c, "%.3f", (cam ? *cam->getRef(camera::e_double::OFFSET_Y) : 0.0));
							break;
						case static_cast<size_t>(text::tags_e::T_CAM_ZOOM):
							sprintf_s(tempstr_c, "%.3f", (cam ? (*cam->getRef(camera::e_double::SCALE_G) * sqrt(*cam->getRef(camera::e_double::SCALE_X) * *cam->getRef(camera::e_double::SCALE_Y))) : 0.0));
							break;
						case static_cast<size_t>(text::tags_e::T_CAM_ZOOMG):
							sprintf_s(tempstr_c, "%.3f", (cam ? *cam->getRef(camera::e_double::SCALE_G) : 0.0));
							break;
						case static_cast<size_t>(text::tags_e::T_CAM_ZOOMX):
							sprintf_s(tempstr_c, "%.3f", (cam ? *cam->getRef(camera::e_double::SCALE_X) : 0.0));
							break;
						case static_cast<size_t>(text::tags_e::T_CAM_ZOOMY):
							sprintf_s(tempstr_c, "%.3f", (cam ? *cam->getRef(camera::e_double::SCALE_Y) : 0.0));
							break;

						case static_cast<size_t>(text::tags_e::T_ISFOLLOWING):
							sprintf_s(tempstr_c, "%s", (follow ? "Y" : "N"));
							break;
						case static_cast<size_t>(text::tags_e::T_COLOR_R):
							sprintf_s(tempstr_c, "%.3f", getRef(sprite::e_color::COLOR)->r);
							break;
						case static_cast<size_t>(text::tags_e::T_COLOR_G):
							sprintf_s(tempstr_c, "%.3f", getRef(sprite::e_color::COLOR)->g);
							break;
						case static_cast<size_t>(text::tags_e::T_COLOR_B):
							sprintf_s(tempstr_c, "%.3f", getRef(sprite::e_color::COLOR)->b);
							break;
						case static_cast<size_t>(text::tags_e::T_COLOR_A):
							sprintf_s(tempstr_c, "%.3f", getRef(sprite::e_color::COLOR)->a);
							break;
						case static_cast<size_t>(text::tags_e::T_MODE):
							sprintf_s(tempstr_c, "%d", *data.integer_data[text::e_integer::STRING_MODE]);
							break;
						case static_cast<size_t>(text::tags_e::T_TIME):
							sprintf_s(tempstr_c, "%.3lf", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - *conf.getRef(database::e_chronomillis_readonly::STARTED_APP_TIME)).count() / 1000.0);
							break;

						case static_cast<size_t>(text::tags_e::T_GB_RESX):
							sprintf_s(tempstr_c, "%d", (d ? al_get_display_width(d) : -1));
							break;
						case static_cast<size_t>(text::tags_e::T_GB_RESY):
							sprintf_s(tempstr_c, "%d", (d ? al_get_display_height(d) : -1));
							break;
						case static_cast<size_t>(text::tags_e::T_REFRESHRATE):
							sprintf_s(tempstr_c, "%d", ((d) ? al_get_display_refresh_rate(d) : -1));
							break;


						case static_cast<size_t>(text::tags_e::T_FPS):
						{
							size_t t;
							conf.get(database::e_sizet::FRAMESPERSECOND, t);
							sprintf_s(tempstr_c, "%zu", t);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_TPS):
						{
							size_t t;
							conf.get(database::e_sizet::COLLISIONSPERSECOND, t);
							sprintf_s(tempstr_c, "%zu", t);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_UPS):
						{
							size_t t;
							conf.get(database::e_sizet::USEREVENTSPERSECOND, t);
							sprintf_s(tempstr_c, "%zu", t);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_APS):
						{
							size_t t;
							conf.get(database::e_sizet::ADVANCEDFUNCSPERSECOND, t);
							sprintf_s(tempstr_c, "%zu", t);
						}
						break;

						case static_cast<size_t>(text::tags_e::T_I_FPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
							sprintf_s(tempstr_c, "%05.1lf", 1.0 / dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_I_TPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
							sprintf_s(tempstr_c, "%05.1lf", 1.0 / dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_I_UPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
							sprintf_s(tempstr_c, "%05.1lf", 1.0 / dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_I_APS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
							sprintf_s(tempstr_c, "%05.1lf", 1.0 / dt);
						}
						break;

						case static_cast<size_t>(text::tags_e::T_MS_FPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_FRAMESPERSECOND, dt);
							sprintf_s(tempstr_c, "%04.3lf", 1000.0 * dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_MS_TPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_COLLISIONSPERSECOND, dt);
							sprintf_s(tempstr_c, "%04.3lf", 1000.0 * dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_MS_UPS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_USEREVENTSPERSECOND, dt);
							sprintf_s(tempstr_c, "%04.3lf", 1000.0 * dt);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_MS_APS):
						{
							double dt;
							conf.get(database::e_double::INSTANT_ADVANCEDFUNCSPERSECOND, dt);
							sprintf_s(tempstr_c, "%04.3lf", 1000.0 * dt);
						}
						break;


						/*case static_cast<size_t>(text::tags_e::T_SPRITE_FRAME):
							if (follow) {
								size_t f;
								follow->get(Constants::io__sprite_sizet::FRAME, f);
								sprintf_s(tempstr_c, "%zu", f);
							}
							else sprintf_s(tempstr_c, "NULL");
							break;*/
						case static_cast<size_t>(text::tags_e::T_MOUSE_X):
						{
							double x;
							conf.get(database::e_double::MOUSE_X, x);
							sprintf_s(tempstr_c, "%.3f", x);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_MOUSE_Y):
						{
							double y;
							conf.get(database::e_double::MOUSE_Y, y);
							sprintf_s(tempstr_c, "%.3f", y);
						}
						break;
						case static_cast<size_t>(text::tags_e::T_LASTSTRING):
						{
							std::string str;
							size_t p = 0;
							conf.get(database::e_string::LAST_STRING, str);
							for (p = 0; p < 128 && p < str.length(); p++) tempstr_c[p] = str[p];
							if (p < 128) tempstr_c[p] = '\0';
						}
						break;
						case static_cast<size_t>(text::tags_e::T_CURRSTRING):
						{
							std::string str;
							size_t p = 0;
							conf.get(database::e_string::CURRENT_STRING, str);
							for (p = 0; p < 128 && p < str.length(); p++) tempstr_c[p] = str[p];
							if (p < 128) tempstr_c[p] = '\0';
						}
						break;
						case static_cast<size_t>(text::tags_e::T_SPRITE_SPEEDX):
							if (follow) {
								double val;
								follow->get(sprite::e_double_readonly::SPEED_X, val);
								sprintf_s(tempstr_c, "%.3lf", val);
							}
							else sprintf_s(tempstr_c, "NULL");
							break;
						case static_cast<size_t>(text::tags_e::T_SPRITE_SPEEDY):
							if (follow) {
								double val;
								follow->get(sprite::e_double_readonly::SPEED_Y, val);
								sprintf_s(tempstr_c, "%.3lf", val);
							}
							else sprintf_s(tempstr_c, "NULL");
							break;
						case static_cast<size_t>(text::tags_e::T_SPRITE_NAME):
							if (follow) {
								std::string temp;
								follow->get(sprite::e_string::ID, temp);
								sprintf_s(tempstr_c, "%s", temp.c_str());
							}
							else sprintf_s(tempstr_c, "NULL");
							break;
						case static_cast<size_t>(text::tags_e::T_SPRITE_ENTITY_HEALTH):
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
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
							break;
						case static_cast<size_t>(text::tags_e::T_SPRITE_ENTITY_NAME):
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
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
							break;
						case static_cast<size_t>(text::tags_e::_T_SPRITE_DEBUG):
							/*if (follow) {
								std::string str = follow->__debug_str();

								if (str.length() >= 128) str = str.substr(0, 124) + "...";

								if (str.length() > 0) sprintf_s(tempstr_c, "%s", str.c_str());
								else sprintf_s(tempstr_c, "NO INFORMATION");
							}
							else sprintf_s(tempstr_c, "NOT_IMPLEMENTED");*/
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
							break;
						case static_cast<size_t>(text::tags_e::T_TEXTURES_LOADED):
						{
							SuperResource<ALLEGRO_BITMAP> textures;
							sprintf_s(tempstr_c, "%zu", textures.size());
						}
						break;
						case static_cast<size_t>(text::tags_e::T_FONTS_LOADED):
						{
							SuperResource<ALLEGRO_FONT> fonts;
							sprintf_s(tempstr_c, "%zu", fonts.size());
						}
						break;
						case static_cast<size_t>(text::tags_e::T_SPRITES_LOADED):
						{
							SuperResource<Sprite_Base> sprites;
							sprintf_s(tempstr_c, "%zu", sprites.size());
						}
						break;
						/*case static_cast<size_t>(text::tags_e::T_TEXTS_LOADED):
						{
							ResourceOf<Text> texts;
							sprintf_s(tempstr_c, "%zu", texts.size());
						}
						break;*/
						case static_cast<size_t>(text::tags_e::T_TRACKS_LOADED):
						{
							/*ResourceOf<Track> tracks;
							sprintf_s(tempstr_c, "%zu", tracks.size());*/
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
						}
						break;
						/*case static_cast<size_t>(text::tags_e::T_SPRITE_STATE):
							if (follow) {
								Constants::io__sprite_tie_func_to_state stat;
								follow->get(Constants::ro__sprite_state::STATE, stat);
								switch (stat) {
								case Constants::io__sprite_tie_func_to_state::COLLISION_MOUSE_ON):
									sprintf_s(tempstr_c, "MOUSE ON");
									break;
								case Constants::io__sprite_tie_func_to_state::COLLISION_MOUSE_CLICK):
									sprintf_s(tempstr_c, "MOUSE CLICK");
									break;
								case Constants::io__sprite_tie_func_to_state::COLLISION_COLLIDED_OTHER):
									sprintf_s(tempstr_c, "OTHER COLLIDING");
									break;
								case Constants::io__sprite_tie_func_to_state::COLLISION_NONE):
									sprintf_s(tempstr_c, "NONE");
									break;
								}
							}
							else sprintf_s(tempstr_c, "NULL");
							break;*/
						case static_cast<size_t>(text::tags_e::T_VOLUME):
						{
							/*Mixer mixer;
							sprintf_s(tempstr_c, "%.0lf", mixer.getVolume() * 100.0);*/
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
						}
						break;
						case static_cast<size_t>(text::tags_e::T_VERSION):
						{
							/*Mixer mixer;
							sprintf_s(tempstr_c, "%s", Constants::version_app.c_str());*/
							sprintf_s(tempstr_c, "NOT_AVAILABLE");
						}
						break;
						case static_cast<size_t>(text::tags_e::T_RESOLUTION_PROPORTION):
						{
							double d = 1.0;
							conf.get(database::e_double::RESOLUTION_BUFFER_PROPORTION, d);
							d *= 100.0;
							if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) sprintf_s(tempstr_c, "%.2lf", d);
							else sprintf_s(tempstr_c, "Disabled");
						}
						break;
						case static_cast<size_t>(text::tags_e::T_CHROMA_FX):
						{
							double d = 1.0;
							conf.get(database::e_double::FX_AMOUNT, d);
							d *= 100.0;
							if (conf.isEq(database::e_boolean::DOUBLE_BUFFERING, true)) sprintf_s(tempstr_c, "%.2lf", d);
							else sprintf_s(tempstr_c, "Disabled");
						}
						break;
						case static_cast<size_t>(text::tags_e::T_FPS_CAP):
						{
							int fpp;
							conf.get(database::e_integer::LIMIT_FPS, fpp);
							if (fpp > 0) sprintf_s(tempstr_c, "%d", fpp);
							else sprintf_s(tempstr_c, "Unlimited");
						}
						break;
						}


						if (fina.length() > (poss + text::tags[q].s().length()))  fina = fina.substr(0, poss) + tempstr_c + fina.substr(poss + text::tags[q].s().length());
						else													  fina = fina.substr(0, poss) + tempstr_c;
					}
				}
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - timing > text::timeout_interpret)
				{
					throw Abort::Abort(__FUNCSIG__, "Took too long interpreting '" + s + "'! [Stopped at '" + fina + "']", Abort::abort_level::GIVEUP);
					return;
				}
			}
			s = fina;
		}


		void Text::draw_self()
		{
			if (!font.ref) throw Abort::Abort(__FUNCSIG__, "No font texture, but called draw anyway!", Abort::abort_level::GIVEUP);

			double off_x = 0.0;
			double off_y = 0.0;
			bool use_following_color = *data.boolean_data[text::e_boolean::USE_FOLLOWING_COLOR_INSTEAD];

			// auto looks smaller, maybe good?
			auto& posx		= *getRef(sprite::e_double_readonly::POSX);
			auto& posy		= *getRef(sprite::e_double_readonly::POSY);
			auto& s_dist_x	= *data.double_data[text::e_double::SHADOW_DISTANCE_X];
			auto& s_dist_y	= *data.double_data[text::e_double::SHADOW_DISTANCE_Y];
			auto& s_col		= *data.color_data[text::e_color::SHADOW_COLOR];
			auto n_col		= *getRef(sprite::e_color::COLOR);
			auto& p_str		= *data.string_readonly_data[text::e_string_readonly::PROCESSED_STRING];
			auto& mode		= *data.integer_data[text::e_integer::STRING_MODE];
			auto& scale_g	= *getRef(sprite::e_double::SCALE_G);
			auto& scale_x	= *getRef(sprite::e_double::SCALE_X);
			auto& scale_y	= *getRef(sprite::e_double::SCALE_Y);
			auto& delta_t	= *data.chronomillis_readonly_data[text::e_chronomillis_readonly::LAST_UPDATE_STRING];

			double t_rotation_rad = *getRef(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
			double p_rotation_rad = 0.0;

			// STRING
			if (std::chrono::system_clock::now().time_since_epoch() > delta_t) // do update string
			{
				delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + entity::default_delta_t_frame_delay);

				p_str = *data.string_data[text::e_string::STRING];
				interpretTags(p_str);
			}



			if (Sprite_Base* fl = *data.sprite_ptr_data[text::e_sprite_ptr::FOLLOWING]; fl)
			{
				fl->get(sprite::e_double_readonly::POSX, off_x);
				fl->get(sprite::e_double_readonly::POSY, off_y);
				fl->get(sprite::e_double_readonly::ROTATION, p_rotation_rad);
				p_rotation_rad *= ALLEGRO_PI / 180.0;

				if (use_following_color) {
					fl->get(sprite::e_color::COLOR, n_col);
				}
			}


			double rotation_rad = t_rotation_rad + p_rotation_rad;
			if (rotation_rad > ALLEGRO_PI * 2) rotation_rad -= ALLEGRO_PI * 2;
			if (rotation_rad < 0) rotation_rad += ALLEGRO_PI * 2;

			bool should_care_about_shadow = (s_dist_x != 0.0 || s_dist_y != 0.0);

			double pos_now[2];

			pos_now[0] = 1.0 * text::default_sharpness_font *  (((posx) * cos(p_rotation_rad)) - ((posy) * sin(p_rotation_rad)) + off_x);
			pos_now[1] = 1.0 * text::default_sharpness_font *  (((posy) * cos(p_rotation_rad)) - ((posx) * sin(p_rotation_rad)) + off_y);

			double pos_shadow_now[2] = { 0,0 };

			if (should_care_about_shadow) {
				pos_shadow_now[0] = 1.0 * text::default_sharpness_font * (((posx + s_dist_x) * cos(p_rotation_rad)) - ((posy + s_dist_y) * sin(p_rotation_rad)) + off_x);
				pos_shadow_now[0] = 1.0 * text::default_sharpness_font * (((posy + s_dist_y) * cos(p_rotation_rad)) - ((posx + s_dist_x) * sin(p_rotation_rad)) + off_y);
			}


			double targ_draw_xy[2];
			targ_draw_xy[0] = pos_now[0] * cos(rotation_rad) + pos_now[1] * sin(rotation_rad);
			targ_draw_xy[1] = pos_now[1] * cos(rotation_rad) - pos_now[0] * sin(rotation_rad);
			targ_draw_xy[0] /= scale_x;
			targ_draw_xy[1] /= scale_y;


			double targ_draw_xy_shadow[2] = { 0,0 };

			if (should_care_about_shadow) {
				targ_draw_xy_shadow[0] = pos_shadow_now[0] * cos(rotation_rad) + pos_shadow_now[1] * sin(rotation_rad);
				targ_draw_xy_shadow[1] = pos_shadow_now[1] * cos(rotation_rad) - pos_shadow_now[0] * sin(rotation_rad);
				targ_draw_xy_shadow[0] /= scale_x;
				targ_draw_xy_shadow[1] /= scale_y;
			}

			Camera preset;
			Camera* ruler = preset.getLastCameraApply();

			//double camx, camy, camg;
			if (*getRef(sprite::e_boolean::AFFECTED_BY_CAM)) preset = *ruler; // copy

			preset.set(camera::e_double::SCALE_G,  *preset.getRef(camera::e_double::SCALE_G)  * scale_g * 1.0 / text::default_sharpness_font);
			preset.set(camera::e_double::SCALE_X,  *preset.getRef(camera::e_double::SCALE_X)  * scale_x);
			preset.set(camera::e_double::SCALE_Y,  *preset.getRef(camera::e_double::SCALE_Y)  * scale_y);
			preset.set(camera::e_double::OFFSET_X, *preset.getRef(camera::e_double::OFFSET_X) * text::default_sharpness_font / scale_g);
			preset.set(camera::e_double::OFFSET_Y, *preset.getRef(camera::e_double::OFFSET_Y) * text::default_sharpness_font / scale_g);

			preset.set(camera::e_double::ROTATION_RAD, *preset.getRef(camera::e_double::ROTATION_RAD) + rotation_rad);
			preset.refresh();
			preset.apply();

			if (s_dist_x != 0.0 || s_dist_y != 0.0) {
				al_draw_text(&(*font.ref), s_col, 1.0 * targ_draw_xy_shadow[0] / (scale_g), 1.0 * targ_draw_xy_shadow[1] / (scale_g), mode, p_str.c_str());
			}
			al_draw_text(&(*font.ref), n_col, 1.0 * targ_draw_xy[0] / (scale_g), 1.0 * targ_draw_xy[1] / (scale_g), mode, p_str.c_str());

			ruler->apply();
		}

		Text::Text() : Sprite_Base()
		{
			custom_draw_task = [&] {draw_self(); };
		}

		Text::Text(Text& o) : Sprite_Base(o)
		{
			custom_draw_task = [&] {draw_self(); };
		}

		void Text::load(const std::string id, const std::string str)
		{
			SuperResource<ALLEGRO_FONT> bmps;

			font.ref = bmps.load(id, str);
			font.source = str;
			font.id = id;

			if (!font.ref) throw Abort::Abort(__FUNCSIG__, "Cannot load '" + id + "'!");

		}

		void Text::remove()
		{
			font.ref.reset();
			font.source.clear();
			font.id.clear();
		}

		void Text::set(const text::e_string e, std::string v)
		{
			if (auto* ptr = data.string_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_double e, double v)
		{
			if (auto* ptr = data.double_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_color e, ALLEGRO_COLOR v)
		{
			if (auto* ptr = data.color_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_integer e, int v)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_sprite_ptr e, Sprite_Base* v)
		{
			if (auto* ptr = data.sprite_ptr_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const text::e_boolean e, bool v)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				*ptr = v;
		}

		void Text::set(const std::string e, std::string v)
		{
			auto* ptr = data.string_data(e.c_str(), e.length());
			if (!ptr) data.string_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, double v)
		{
			auto* ptr = data.double_data(e.c_str(), e.length());
			if (!ptr) data.double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, ALLEGRO_COLOR v)
		{
			auto* ptr = data.color_data(e.c_str(), e.length());
			if (!ptr) data.color_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, int v)
		{
			auto* ptr = data.integer_data(e.c_str(), e.length());
			if (!ptr) data.integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, Sprite_Base* v)
		{
			auto* ptr = data.sprite_ptr_data(e.c_str(), e.length());
			if (!ptr) data.sprite_ptr_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Text::set(const std::string e, bool v)
		{
			auto* ptr = data.boolean_data(e.c_str(), e.length());
			if (!ptr) data.boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Text::get(const text::e_string_readonly e, std::string& v)
		{
			if (auto* ptr = data.string_readonly_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_string e, std::string& v)
		{
			if (auto* ptr = data.string_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_double e, double& v)
		{
			if (auto* ptr = data.double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_color e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_integer e, int& v)
		{
			if (auto* ptr = data.integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_sprite_ptr e, Sprite_Base*& v)
		{
			if (auto* ptr = data.sprite_ptr_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const text::e_boolean e, bool& v)
		{
			if (auto* ptr = data.boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, std::string& v)
		{
			if (auto* ptr = data.string_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, double& v)
		{
			if (auto* ptr = data.double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, ALLEGRO_COLOR& v)
		{
			if (auto* ptr = data.color_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, int& v)
		{
			if (auto* ptr = data.integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, Sprite_Base*& v)
		{
			if (auto* ptr = data.sprite_ptr_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Text::get(const std::string e, bool& v)
		{
			if (auto* ptr = data.boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}




		std::string* Text::getRef(const text::e_string e)
		{
			if (auto* ptr = data.string_data(e); ptr)
				return ptr;
			return nullptr;
		}

		double* Text::getRef(const text::e_double e)
		{
			if (auto* ptr = data.double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		ALLEGRO_COLOR* Text::getRef(const text::e_color e)
		{
			if (auto* ptr = data.color_data(e); ptr)
				return ptr;
			return nullptr;
		}

		int* Text::getRef(const text::e_integer e)
		{
			if (auto* ptr = data.integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		Sprite_Base** Text::getRef(const text::e_sprite_ptr e)
		{
			if (auto* ptr = data.sprite_ptr_data(e); ptr)
				return ptr;
			return nullptr;
		}

		bool* Text::getRef(const text::e_boolean e)
		{
			if (auto* ptr = data.boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		const std::string* Text::getRef(const text::e_string_readonly e) const
		{
			if (auto* ptr = data.string_readonly_data(e); ptr)
				return ptr;
			return nullptr;
		}

}
}