#include "../LSW_final/LSW/LSWv5.h"

// this is just a stable copy, not the original main

#include <iostream>

using namespace LSW::v5;
using namespace LSW::v5::Interface;

const std::string logpath = "%appdata%/Lohk's Studios/TheBlast/log/log.log";
const std::string configpath = "%appdata%/Lohk's Studios/TheBlast/config/config.lohk";
const std::string datapath = "%appdata%/Lohk's Studios/TheBlast/data/data.zip";

const std::string resource_url = "https://www.dropbox.com/s/tuc0mu4eotoh7ay/data.zip?dl=1";

const std::string music_test_inner_path = "musics/music_alt.ogg";
const int font_size_set = 200;
const bool need_texture_update = false;// if I change something, set to true


void check_file_download(Work::GameCore&);
void multitask_test(Work::GameCore&);
void socketsys_test(Work::GameCore&, double*);
void smartfile_test(Work::GameCore&);
void pathmngr_debug(Work::GameCore&, const Interface::PathManager&);

int main() {

	Work::GameCore core(logpath, configpath);
	std::string _what_test = "Loading";

	Tools::SuperResource<Work::ProgressBar> progressbar_source;
	Tools::SuperResource<Bitmap> bmps;
	Tools::SuperResource<Font> source_font;
	Tools::SuperResource<Work::Text> texts;

	auto progress = progressbar_source.create("_load_progress");
	progress->set(Work::progressbar::e_double::PROGRESS, 0.0);
	progress->set(Work::progressbar::e_color::FOREGROUND_TOP_LEFT, Interface::Color{ 67, 170, 185 });
	progress->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_LEFT, Interface::Color{ 51, 135, 142 });
	progress->set(Work::progressbar::e_color::FOREGROUND_TOP_RIGHT, Interface::Color{ 95, 190, 236 });
	progress->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_RIGHT, Interface::Color{ 67, 155, 189 });
	progress->set(Work::progressbar::e_color::BACKGROUND_TOP_LEFT, Interface::Color{ 9, 25, 50 });
	progress->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_LEFT, Interface::Color{ 4, 8, 29 });
	progress->set(Work::progressbar::e_color::BACKGROUND_TOP_RIGHT, Interface::Color{ 16, 40, 55 });
	progress->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_RIGHT, Interface::Color{ 8, 17, 30 });
	progress->set(Work::progressbar::e_boolean::FIXED_BACKGROUND, true);
	progress->set(Work::progressbar::e_double::BORDER_THICKNESS, 0.0); // disable
	progress->set(Work::progressbar::e_double::SMOOTHNESS, 50.0);
	//progress->set(Work::progressbar::e_double::BORDER_THICKNESS, 0.01);
	//progress->set(Work::progressbar::e_double::BORDER_ROUNDNESS, 0.004);
	//progress->set(Work::progressbar::e_double::BORDER_PROPORTION_X, 1.000);
	//progress->set(Work::progressbar::e_double::BORDER_PROPORTION_Y, 1.025);
	progress->set(Work::sprite::e_double::SCALE_G, 0.75);
	progress->set(Work::sprite::e_double::SCALE_Y, 0.1);
	progress->set(Work::sprite::e_double::TARG_POSX, 0.0);
	progress->set(Work::sprite::e_double::TARG_POSY, 0.7);

	auto _text_early = texts.create("__loading");
	_text_early->set(Work::sprite::e_boolean::DRAW, false);
	_text_early->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	_text_early->set(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	_text_early->set<Tools::Cstring>(Work::text::e_cstring::STRING, [&] { return "&9" + _what_test + [] {std::string s; for (unsigned u = 0; u < ((int)(al_get_time() * 1.4)) % 3; u++) s += '.'; return '.' + s; }(); });
	_text_early->set(Work::sprite::e_double::TARG_POSX, 0.0);
	_text_early->set(Work::sprite::e_double::TARG_POSY, 0.58);
	_text_early->set(Work::sprite::e_double::SCALE_G, 0.085);
	_text_early->set(Work::sprite::e_double::SCALE_X, 1.27);
	_text_early->set(Work::text::e_double::UPDATES_PER_SECOND, 60.0);
	_text_early->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(55, 27, 100));
	_text_early->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.002);
	_text_early->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.002);
	_text_early->set(Work::text::e_boolean::USE_COLOR_INSTEAD_OF_AUTO, true);
	_text_early->set(Work::sprite::e_color::COLOR, Interface::Color(205, 230, 255));

	auto _text_early2 = texts.create("__loading2");
	_text_early2->set(Work::sprite::e_boolean::DRAW, false);
	_text_early2->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	_text_early2->set(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	_text_early2->set<Tools::Cstring>(Work::text::e_cstring::STRING, [&] { return "&f" + Tools::sprintf_a("%04.1lf%c", 100.0 * progress->get_direct<double>(Work::progressbar::e_double_readonly::PROGRESS_SMOOTH), '%'); });
	_text_early2->set(Work::sprite::e_double::TARG_POSX, 0.0);
	_text_early2->set(Work::sprite::e_double::TARG_POSY, 0.7);
	_text_early2->set(Work::sprite::e_double::SCALE_G, 0.055);
	_text_early2->set(Work::sprite::e_double::SCALE_X, 1.2);
	_text_early2->set(Work::text::e_double::UPDATES_PER_SECOND, 5.0);
	_text_early2->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(55, 27, 100));
	_text_early2->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.0015);
	_text_early2->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.0015);
	_text_early2->set(Work::text::e_boolean::USE_COLOR_INSTEAD_OF_AUTO, true);
	_text_early2->set(Work::sprite::e_color::COLOR, Interface::Color(205, 230, 255));
	_text_early2->set(Work::sprite::e_color::COLOR, Interface::Color(205, 230, 255));


	auto _delete_drawtask = core.get_display().add_draw_task([&](const auto& cam) {
		progress->draw(cam);
		_text_early->draw(cam);
		_text_early2->draw(cam);
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		});

	//progress->set<double>(Work::progressbar::e_double::TOP_BOTTOM_OFFSET, 0.1);

	// test progressbar
	//while (1) std::this_thread::sleep_for(std::chrono::seconds(5));

	Logger logg;

	logg << L::SLF << fsr() << "&7#======&7====================================&7======#" << L::ELF;
	logg << L::SLF << fsr() << "&7#      &eThis is not a game yet, just a test.&7      #" << L::ELF;
	logg << L::SLF << fsr() << "&7#======&7====================================&7======#" << L::ELF;


	progress->set(Work::progressbar::e_double::PROGRESS, 2e-2);

	check_file_download(core);

	// technically the test is later on, but whatever
	Interface::PathManager pather;
	pather.add_path(datapath);

	auto early_work = core.get_display().add_once_task([pather] {
		Logger logg;
		Tools::SuperResource<Font> source_font;

		auto foo = source_font.create("DEFAULT");

		// use .zip
		pather.apply();

		foo->load("font.ttf", font_size_set);

		if (!foo) logg << L::SLF << fsr(E::WARN) << "Failed to load font." << L::ELF;
		else {
			logg << L::SLF << fsr() << "Loaded font successfully." << L::ELF;
		}

		pather.unapply();

		return true;
		});
	early_work.wait();

	Tools::Resource<Interface::Font> sfont;
	if (!source_font.get("DEFAULT", sfont)) {
		logg << L::SLF << fsr(E::ERRR) << "Font wasn't found somehow" << L::ELF;
		core.shutdown();
		return -1;
	}

	_text_early->set(sfont);
	_text_early->set(Work::sprite::e_boolean::DRAW, true);
	_text_early2->set(sfont);
	_text_early2->set(Work::sprite::e_boolean::DRAW, true);



	progress->set(Work::progressbar::e_double::PROGRESS, 10e-2);

	//core.get_display().set_double_buffering_scale(0.3);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "PathManager Test";

	pathmngr_debug(core, pather);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Loading textures";

	Tools::SuperResource<Work::Block> blocks;

	auto working_on = core.get_display().add_once_task([pather] {
		Logger logg;
		Tools::SuperResource<Bitmap> source;

		auto atlas = source.create("ATLAS_GLOBAL");
		auto lmao = source.create("_test");
		auto dirt = source.create("_dirt");
		auto mouse = source.create("_mouse");

		// use .zip
		pather.apply();

		atlas->load("atlas0.png");
		bool gud = *atlas;

		if (!gud) logg << L::SLF << fsr(E::WARN) << "Failed to load atlas." << L::ELF;

		if (gud) {
			gud = lmao->create_sub(atlas, 0, 1536, 512, 512);
			gud = gud && dirt->create_sub(atlas, 0, 1024, 512, 512);
			gud = gud && mouse->create_sub(atlas, 1536, 1024, 256, 256);
			if (!gud) {
				logg << L::SLF << fsr(E::WARN) << "Failed to load sub bitmap from atlas." << L::ELF;
			}
			else {
				logg << L::SLF << fsr() << "Loaded a bitmap from atlas SUCESSFULLY" << L::ELF;
			}
		}
		if (!gud) {
			lmao->create(300, 300);
			lmao->clear_to_color(Color(255, 255, 255));
			if (*lmao) {
				logg << L::SLF << fsr() << "Successfully created a bitmap to show." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(E::ERRR) << "Failed to create a bitmap." << L::ELF;
			}
		}

		pather.unapply();

		return true;
		});

	//auto got = working_on.get().get<Tools::Resource<Bitmap>>();

	progress->set(Work::progressbar::e_double::PROGRESS, 90e-2);

	working_on.wait();

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Done loading textures";

	bool __simple_texts = false;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - BLOCKS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Generating Blocks";

	Tools::Resource<Bitmap> lmao, dirt, mouse;

	if (!bmps.get("_test", lmao)) {
		logg << L::SLF << fsr(E::ERRR) << "Texture wasn't found somehow..." << L::ELF;
		core.shutdown();
		return -1;
	}
	if (!bmps.get("_dirt", dirt)) {
		logg << L::SLF << fsr(E::ERRR) << "Texture wasn't found somehow..." << L::ELF;
		core.shutdown();
		return -1;
	}
	if (!bmps.get("_mouse", mouse)) {
		logg << L::SLF << fsr(E::ERRR) << "Texture wasn't found somehow..." << L::ELF;
		core.shutdown();
		return -1;
	}

	auto blk0 = blocks.create("_test_block");
	auto blk1 = blocks.create("_test_block2");
	auto blk_mouse = blocks.create("_test_block_mouse");
	auto blk3 = blocks.create("_test_col");
	auto blk_height = blocks.create("_test_slidex");
	auto blk_zoom = blocks.create("_test_slidezoom");
	auto blk_width = blocks.create("_test_slidey");
	auto blk_switch = blocks.create("_test_switch");

	blk0->insert(lmao);
	blk_mouse->insert(mouse);
	blk3->insert(lmao);
	blk1->insert(lmao);
	blk_height->insert(lmao);
	blk_zoom->insert(lmao);
	blk_width->insert(lmao);
	blk_switch->insert(dirt);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Setting up Blocks";


	blk_switch->set(Work::sprite::e_double::TARG_POSX, 0.9);
	blk_switch->set(Work::sprite::e_double::TARG_POSY, -0.9);
	blk_switch->set(Work::sprite::e_double::SCALE_G, 0.1);
	blk_switch->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	blk_switch->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk_switch->set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_CLICK, [&](const Tools::Any& ref) {
		__simple_texts = !__simple_texts;

		Logger logg;
		logg << L::SL << fsr() << "SWITCH TEXT " << (__simple_texts ? "Y" : "N") << L::EL;
		});


	blk_height->set(Work::sprite::e_double::TARG_POSX, 0.99);
	blk_height->set(Work::sprite::e_double::TARG_POSY, 0.0);
	blk_height->set(Work::sprite::e_double::SCALE_G, 1.0);
	blk_height->set(Work::sprite::e_double::SCALE_Y, 1.9);
	blk_height->set(Work::sprite::e_double::SCALE_X, 0.02);
	blk_height->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	blk_height->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk_height->set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_UNCLICK, [&](const Tools::Any& ref) {
		auto u = ref.get<std::array<double, 4>>();
		core.get_display().get_current_camera().get_classic().y = u[3];
		core.get_display().set_refresh_camera();

		Logger logg;
		logg << L::SL << fsr() << "Y set to " << u[3] << L::EL;
		});

	blk_zoom->set(Work::sprite::e_double::TARG_POSX, -0.99);
	blk_zoom->set(Work::sprite::e_double::TARG_POSY, 0.0);
	blk_zoom->set(Work::sprite::e_double::SCALE_G, 1.0);
	blk_zoom->set(Work::sprite::e_double::SCALE_Y, 1.6);
	blk_zoom->set(Work::sprite::e_double::SCALE_X, 0.02);
	blk_zoom->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	blk_zoom->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk_zoom->set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_UNCLICK, [&](const Tools::Any& ref) {
		auto u = ref.get<std::array<double, 4>>();
		core.get_display().get_current_camera().get_classic().sx = pow(1.01 - u[3], 2.0);
		core.get_display().get_current_camera().get_classic().sy = pow(1.01 - u[3], 2.0);
		core.get_display().set_refresh_camera();

		Logger logg;
		logg << L::SL << fsr() << "Zoom set to " << pow(1.01 - u[3], 2.0) << "x" << L::EL;
		});

	blk_width->set(Work::sprite::e_double::TARG_POSX, 0.0);
	blk_width->set(Work::sprite::e_double::TARG_POSY, 0.99);
	blk_width->set(Work::sprite::e_double::SCALE_G, 1.0);
	blk_width->set(Work::sprite::e_double::SCALE_X, 1.9);
	blk_width->set(Work::sprite::e_double::SCALE_Y, 0.02);
	blk_width->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	blk_width->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk_width->set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_UNCLICK, [&](const Tools::Any& ref) {
		auto u = ref.get<std::array<double, 4>>();
		core.get_display().get_current_camera().get_classic().x = u[2];
		core.get_display().set_refresh_camera();

		Logger logg;
		logg << L::SL << fsr() << "X set to " << u[2] << L::EL;
		});






	blk0->set<double>(Work::sprite::e_double::TARG_POSX, 0.0);
	blk0->set<double>(Work::sprite::e_double::TARG_POSY, [] { return 0.25 + 0.55 * sin(MILLI_NOW.count() / 689.0); });
	blk0->set<double>(Work::sprite::e_double::SCALE_G, 0.8);
	blk0->set<double>(Work::sprite::e_double::SCALE_Y, 0.3);
	blk0->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk1->set<double>(Work::sprite::e_double::TARG_POSX, 0.0);
	blk1->set<double>(Work::sprite::e_double::TARG_POSY, [] { return -0.25 + 0.55 * sin(MILLI_NOW.count() / 689.0); });
	blk1->set<double>(Work::sprite::e_double::SCALE_G, 0.8);
	blk1->set<double>(Work::sprite::e_double::SCALE_Y, 0.3);
	blk1->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	blk0->set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_CLICK, [&](auto) {
		Logger logg;
		logg << L::SL << fsr() << "Uau vc sabe clicar, boa!" << L::EL;
		});


	blk_mouse->set<double>(Work::sprite::e_double::TARG_POSX, [&] { return core.get_config().get_as<double>("mouse", "x"); });
	blk_mouse->set<double>(Work::sprite::e_double::TARG_POSY, [&] { return core.get_config().get_as<double>("mouse", "y"); });
	blk_mouse->set<double>(Work::sprite::e_double::SCALE_G, 0.15);
	blk_mouse->set<double>(Work::sprite::e_double::TARG_ROTATION, [] {return 30.0 * al_get_time(); });
	blk_mouse->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));
	//blk_mouse->set(Work::sprite::e_boolean::SHOWBOX, true);
	//blk_mouse->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);



	blk3->set<double>(Work::sprite::e_double::TARG_POSX, -0.65);
	blk3->set<double>(Work::sprite::e_double::TARG_POSY, -0.5);
	blk3->set<double>(Work::sprite::e_double::SCALE_G, 0.20);
	blk3->set<double>(Work::sprite::e_double::SCALE_X, 0.85);
	//blk3->set(Work::sprite::e_boolean::SHOWBOX, true);


	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Starting Blocks' Collisioner";


	Work::Collisioner overlay_control(core.get_config());
	overlay_control.insert(blk_height);
	overlay_control.insert(blk_width);
	overlay_control.insert(blk_switch);
	overlay_control.insert(blk_zoom);

	Work::Collisioner collision_control(core.get_config());
	collision_control.insert(blk0);
	collision_control.insert(blk3);
	collision_control.insert(blk1);
	collision_control.insert(blk_mouse);

	collision_control.start(1.0 / 15);
	overlay_control.start(1.0 / 10);


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - BLOCKS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //

	progress->set(Work::progressbar::e_double::PROGRESS, 95e-2);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - TEXTS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Generating Texts";


	auto txt0 = texts.create("_line0");
	auto txtdebug = texts.create("_line4");
	auto txtu = texts.create("_lineu");

	auto txtmouse = texts.create("_follow_mouse_test");

	auto txtf0 = texts.create("_follow_0");
	auto txtf1 = texts.create("_follow_1");

	txt0->set(sfont);
	txtdebug->set(sfont);
	txtu->set(sfont);
	txtmouse->set(sfont);
	txtf0->set(sfont);
	txtf1->set(sfont);

	txt0->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtdebug->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtu->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtmouse->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtf0->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtf1->set(Work::text::e_integer::FONT_SIZE, font_size_set);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Setting up Texts";


	Tools::Stopwatch sw; // noo need to mutex, []{} is called in sync with draw
	sw.prepare(15);
	size_t __c = 0;


	txt0->set<Tools::Cstring>(Work::text::e_cstring::STRING,
		[&core, &collision_control, &overlay_control, &__simple_texts] {
			return (__simple_texts ? ("&a" + std::to_string(core.get_display().get_frames_per_second())) : (
				"&5FPS: &a" + std::to_string(core.get_display().get_frames_per_second()) + (core.get_display().get_fps_cap() ? (" / " + std::to_string(core.get_display().get_fps_cap()) + " (limit)") : "") + (core.get_display().get_vsync() ? (" &6VSYNC SET") : " &bVSYNC UNSET") + "\n" +
				"&3TPS: &d" + std::to_string(collision_control.effective_tps()) + " [" + Tools::sprintf_a("%04.1lf", collision_control.actual_ms()) + " ms, " + Tools::sprintf_a("%04.1lf", 100.0 * collision_control.actual_load()) + "%] & " +
				std::to_string(overlay_control.effective_tps()) + " [" + Tools::sprintf_a("%04.1lf", overlay_control.actual_ms()) + " ms, " + Tools::sprintf_a("%04.1lf", 100.0 * overlay_control.actual_load()) + "%]\n" +
				"&2Times open: &b" + std::to_string(core.get_config().get_as<unsigned long long>("registry", "times_open")) + "\n" +
				"&7Mouse pos: &e" + std::to_string(core.get_config().get_as<float>(Work::gamecore::conf_mouse_memory, "x")) + "x" + std::to_string(core.get_config().get_as<float>(Work::gamecore::conf_mouse_memory, "y"))
				));
		});
	txt0->set(Work::sprite::e_double::TARG_POSX, -1.0);
	txt0->set(Work::sprite::e_double::TARG_POSY, -0.97);
	txt0->set(Work::sprite::e_double::SCALE_G, 0.07);
	//txt0->set(Work::sprite::e_double::SCALE_X, 0.35);
	txt0->set(Work::text::e_double::LINE_ADJUST, 0.85);
	txt0->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	txt0->set(Work::text::e_double::UPDATES_PER_SECOND, 1.0);
	txt0->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(0, 95, 147));
	txt0->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.001);
	txt0->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.001);
	txt0->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	txt0->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 1.0);


	txtdebug->set<Tools::Cstring>(Work::text::e_cstring::STRING, [&] {
		if (!__c) return std::string("&9Debug sync lost, please wait.");
		return
			"&9DEBUG BMP/TEXT DRAW:\n" + sw.generate_table() + "\n&eTotal time draw: " + sw.get_string_between(0, sw.last_point_valid());
		});
	txtdebug->set(Work::sprite::e_double::TARG_POSX, -1.0);
	txtdebug->set(Work::sprite::e_double::TARG_POSY, -0.69);
	txtdebug->set(Work::sprite::e_double::SCALE_G, 0.052);
	//txtdebug->set(Work::sprite::e_double::SCALE_X, 0.35);
	txtdebug->set(Work::text::e_double::LINE_ADJUST, 0.75);
	txtdebug->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	txtdebug->set(Work::text::e_double::UPDATES_PER_SECOND, 2.0);
	txtdebug->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(127, 0, 0));
	txtdebug->set<bool>(Work::sprite::e_boolean::DRAW, [&] {return !__simple_texts; });
	txtdebug->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.0007);
	txtdebug->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.0007);
	//txtdebug->set(Work::sprite::e_string::ID, std::string("HEYO MAYO!"));
	txtdebug->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	txtdebug->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 1.0);


	txtu->set<Tools::Cstring>(Work::text::e_cstring::STRING, [&logg] { return logg.get_last_line().filter_ascii_range(); });
	txtu->set(Work::sprite::e_double::TARG_POSX, -1.0);
	txtu->set(Work::sprite::e_double::TARG_POSY, 0.96);
	txtu->set(Work::sprite::e_double::SCALE_G, 0.06);
	//txtu->set(Work::sprite::e_double::SCALE_X, 0.35);
	//txtu->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(0, 0, 0));
	txtu->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.001);
	txtu->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.001);
	txtu->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	txtu->set<bool>(Work::sprite::e_boolean::DRAW, [&] {return !__simple_texts; });
	txtu->set(Work::text::e_double::UPDATES_PER_SECOND, 3.0);
	txtu->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	txtu->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 1.0);

	txtf0->set<Tools::Cstring>(Work::text::e_cstring::STRING, [] { std::string t = "FOLLOW 0 HYPE"; return t.substr(0, (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 500) % (10 + t.length())); });
	txtf0->set(Work::sprite::e_double::TARG_POSX, 0.0);
	//txtf0->set(Work::sprite::e_double::TARG_POSY, -0.1);
	txtf0->set(Work::sprite::e_double::SCALE_G, 0.15);
	//txtf0->set(Work::sprite::e_double::SCALE_X, 0.6);
	txtf0->set<int>(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	txtf0->set<Work::Sprite_Base>(Work::text::e_sprite_ref::FOLLOWING, *blk0);
	txtf0->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 0.66);
	txtf0->set(Work::text::e_double::UPDATES_PER_SECOND, 20.0);
	txtf0->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(150, 100, 100));
	txtf0->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.005);
	txtf0->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.005);
	txtf0->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);

	txtf1->set<Tools::Cstring>(Work::text::e_cstring::STRING, "FOLLOW 1 HYPERSMOOTH 60 FPS");
	txtf1->set(Work::sprite::e_double::TARG_POSX, 0.0);
	//txtf1->set(Work::sprite::e_double::TARG_POSY, -0.1);
	txtf1->set(Work::sprite::e_double::SCALE_G, 0.15);
	txtf1->set(Work::sprite::e_double::SCALE_X, 0.85);
	txtf1->set<int>(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	txtf1->set<Work::Sprite_Base>(Work::text::e_sprite_ref::FOLLOWING, *blk1);
	txtf1->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 0.40);
	txtf1->set(Work::text::e_double::UPDATES_PER_SECOND, 60.0);
	txtf1->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(100, 100, 150));
	txtf1->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.005);
	txtf1->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.005);
	txtf1->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);

	txtmouse->set<Tools::Cstring>(Work::text::e_cstring::STRING, "&d*mouse");
	txtmouse->set(Work::sprite::e_double::TARG_POSX, 0.1);
	txtmouse->set(Work::sprite::e_double::TARG_POSY, 0.1);
	txtmouse->set(Work::sprite::e_double::SCALE_G, 0.12);
	//txtmouse->set(Work::sprite::e_double::SCALE_X, 0.28);
	txtmouse->set<int>(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	txtmouse->set<Work::Sprite_Base>(Work::text::e_sprite_ref::FOLLOWING, *blk_mouse);
	txtmouse->set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 0.80);
	txtmouse->set(Work::text::e_double::UPDATES_PER_SECOND, 24.0);
	txtmouse->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(0, 255, 255));
	txtmouse->set(Work::text::e_double::SHADOW_DISTANCE_X, 0.002);
	txtmouse->set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.002);
	txtmouse->set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	txtmouse->set<bool>(Work::sprite::e_boolean::DRAW, [&] {return !__simple_texts; });
	//txtmouse->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	txtmouse->set<double>(Work::sprite::e_double::TARG_ROTATION, [] {return 90.0 * al_get_time(); });


	overlay_control.insert(txtdebug);
	overlay_control.insert(txt0);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - TEXTS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //

	progress->set(Work::progressbar::e_double::PROGRESS, 100e-2);

	_what_test = "Setting up main camera";
	progress->set(Work::progressbar::e_double::SMOOTHNESS, 20.0);

	while (progress->get_direct<double>(Work::progressbar::e_double_readonly::PROGRESS_SMOOTH) < 99e-2) std::this_thread::sleep_for(std::chrono::milliseconds(250));

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	{
		core.get_display().remove_draw_task(_delete_drawtask);

		core.get_display().add_draw_task([=, &__c, &sw](const Camera& c) {
			///got->draw((Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, (Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, 0.3, 0.3);

			if (!__c) sw.start(); // 0
			blk0->draw(c);
			if (!__c) sw.click_one(); // 1
			blk1->draw(c);
			if (!__c) sw.click_one(); // 2
			blk_mouse->draw(c);
			if (!__c) sw.click_one(); // 3
			blk3->draw(c);
			if (!__c) sw.click_one(); // 4
			blk_height->draw(c);
			if (!__c) sw.click_one(); // 5
			blk_width->draw(c);
			if (!__c) sw.click_one(); // 6
			blk_switch->draw(c);
			if (!__c) sw.click_one(); // 7
			blk_zoom->draw(c);
			if (!__c) sw.click_one(); // 8
			txt0->draw(c);
			if (!__c) sw.click_one(); // 9
			txtu->draw(c);
			if (!__c) sw.click_one(); // 10
			txtmouse->draw(c);
			if (!__c) sw.click_one(); // 11
			txtf0->draw(c);
			if (!__c) sw.click_one(); // 12
			txtf1->draw(c);
			if (!__c) sw.click_one(); // 13
			txtdebug->draw(c);
			if (!__c) sw.click_one(); // 14

			if (++__c > 200) __c = 0;

			});
	}

	Tools::SuperResource<Track> tracks;
	Tools::SuperResource<Sample> samples;

	auto a_music = samples.create("ONE_SAMPLE");
	auto track = tracks.create("MY_TRACK");

	pather.apply();

	if (a_music->load(music_test_inner_path)) {
		track->load(a_music);
		track->attach_to(core.get_mixer());
		track->set_play_mode(track::play_mode::MODE_LOOP);

		track->play();
		logg << L::SLF << fsr() << "Started background music and paused with playmode " << static_cast<int>(track->get_play_mode()) << L::ELF;
		track->pause();
	}
	else {
		logg << L::SLF << fsr(E::ERRR) << "Couldn't start track." << L::ELF;
	}

	// reset
	pather.unapply();



	EventHandler fullscreen;
	fullscreen.add(get_keyboard_event());
	fullscreen.set_run_autostart([&](const RawEvent& ev) {
		switch (ev.type())
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (ev.keyboard_event().keycode) {
			case ALLEGRO_KEY_F11:
			{
				logg << L::SL << fsr() << "Toggle Fullscreen called" << L::EL;
				core.get_display().toggle_fullscreen();
			}
			break;
			case ALLEGRO_KEY_F:
			{
				logg << L::SL << fsr() << "Current FPS: &a" << core.get_display().get_frames_per_second() << L::EL;
				logg << L::SL << fsr() << "&eOut of range errors: &7" << core.get_display().debug_errors_out_of_range_skip() << L::EL;
				logg << L::SL << fsr() << "&eUnexpected errors: &7" << core.get_display().debug_errors_unexpected() << L::EL;
			}
			break;
			case ALLEGRO_KEY_P:
			{
				logg << L::SL << fsr() << "Play/Pause button called" << L::EL;
				if (track->exists()) {
					if (track->is_playing()) track->pause();
					else track->play();
				}
			}
			break;
			case ALLEGRO_KEY_R:
			{
				logg << L::SL << fsr() << "Reverse/Continuous button called" << L::EL;
				if (track->exists()) {
					track->set_speed(-track->get_speed());
				}
			}
			break;
			case ALLEGRO_KEY_MINUS:
			{
				logg << L::SL << fsr() << "SlowDown button called" << L::EL;
				if (track->exists()) {
					if (fabs(track->get_speed()) > 0.1) track->set_speed(track->get_speed() > 0.0 ? track->get_speed() - 0.05 : track->get_speed() + 0.05);
					logg << L::SL << fsr() << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			break;
			case ALLEGRO_KEY_EQUALS:
			{
				logg << L::SL << fsr() << "Accel button called" << L::EL;
				if (track->exists()) {
					track->set_speed(track->get_speed() > 0.0 ? track->get_speed() + 0.05 : track->get_speed() - 0.05);
					logg << L::SL << fsr() << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			break;
			case ALLEGRO_KEY_PAD_MINUS:
			{
				logg << L::SL << fsr() << "PAD- button called" << L::EL;
				float vol = core.get_mixer().get_gain();
				if (vol > 0.02) vol -= 0.02;
				core.get_mixer().set_gain(vol);

				logg << L::SL << fsr() << "Now volume = " << vol << "." << L::EL;
			}
			break;
			case ALLEGRO_KEY_PAD_PLUS:
			{
				logg << L::SL << fsr() << "PAD+ button called" << L::EL;
				float vol = core.get_mixer().get_gain();
				if (vol < 1.0) vol += 0.02;
				core.get_mixer().set_gain(vol);

				logg << L::SL << fsr() << "Now volume = " << vol << "." << L::EL;
			}
			break;
			case ALLEGRO_KEY_COMMA: // ,
			{
				logg << L::SL << fsr() << "< called, reducing fps cap by 5..." << L::EL;
				auto& d = core.get_display();
				if (d.get_fps_cap() >= 5) d.set_fps_cap(d.get_fps_cap() - 5);
				else d.set_fps_cap(0);
			}
			break;
			case ALLEGRO_KEY_FULLSTOP: // .
			{
				logg << L::SL << fsr() << "> called, raising fps cap by 5..." << L::EL;
				auto& d = core.get_display();
				d.set_fps_cap(d.get_fps_cap() + 5);
			}
			break;
			case ALLEGRO_KEY_V:
			{
				logg << L::SL << fsr() << "V called, vsync switch (reopen app to set)." << L::EL;
				auto& d = core.get_display();
				d.set_vsync(!d.get_vsync());
			}
			break;
			case ALLEGRO_KEY_L:
			{
				auto dbuffs = core.get_display().get_current_buffering_scale();
				if (dbuffs >= 0.05) {
					dbuffs -= 0.05;
					if (dbuffs < 0.05) dbuffs = 0.0;
					logg << L::SL << fsr() << "L called, lowering dbuffer to " << Tools::sprintf_a("%.0lf", 100.0 * dbuffs) << "%..." << L::EL;
					core.get_display().set_double_buffering_scale(dbuffs);
				}
				else {
					logg << L::SL << fsr() << "L called, but already disabled double buffering!" << L::EL;
				}
			}
			break;
			case ALLEGRO_KEY_O:
			{
				auto dbuffs = core.get_display().get_current_buffering_scale();
				dbuffs += 0.05;
				logg << L::SL << fsr() << "O called, trying to increase dbuffer to " << Tools::sprintf_a("%.0lf", 100.0 * dbuffs) << "%..." << L::EL;
				core.get_display().set_double_buffering_scale(dbuffs);
			}
			break;
			case ALLEGRO_KEY_F1:
			{
				auto d = 1.0 / collision_control.get_speed();
				if (d < 0.0) {
					logg << L::SL << fsr() << "Cannot change acceleration." << L::EL;
					break;
				}

				if (d > 50.0) {
					d -= 10.0;
				}
				else if (d > 20.0) {
					d -= 5.0;
				}
				else if (d > 10.0) {
					d -= 2.0;
				}
				else if (d > 5.0) {
					d -= 1.0;
				}
				else if (d < 5.0) d = 5.0;
				logg << L::SL << fsr() << "Slowing down collision system, now at " << (int)d << " tps." << L::EL;
				collision_control.set_speed(1.0 / d);
			}
			break;
			case ALLEGRO_KEY_F2:
			{
				auto d = 1.0 / collision_control.get_speed();
				if (d < 0.0) {
					logg << L::SL << fsr() << "Cannot change acceleration." << L::EL;
					break;
				}

				if (d < 10.0) {
					d += 1.0;
				}
				else if (d < 20.0) {
					d += 2.0;
				}
				else if (d < 50.0) {
					d += 5.0;
				}
				else {
					d += 10.0;
				}
				logg << L::SL << fsr() << "Accelerating collision system, now at " << (int)d << " tps." << L::EL;
				collision_control.set_speed(1.0 / d);
			}
			break;
			default:
			{
				logg << L::SL << fsr() << "KEY pressed: &5" << ev.keyboard_event().keycode << L::EL;
			}
			break;
			}
			break;
		}
		});

	core.yield();

	collision_control.set_stop();
	overlay_control.set_stop();

	collision_control.stop();
	overlay_control.stop();

	core.shutdown();

	std::this_thread::sleep_for(std::chrono::seconds(1));


	logg << L::SLF << fsr() << "&eThe end." << L::ELF;

	//debug("DEBUG TEST LSW");

	return 0;
}

void check_file_download(Work::GameCore & core)
{
	Logger logg;

	// download resource
	{
		core.get_config().ensure("registry", "times_open", 0, Interface::config::config_section_mode::SAVE);
		auto val = core.get_config().get_as<unsigned long long>("registry", "times_open");
		core.get_config().set("registry", "times_open", val + 1);

		bool download_this = false;

		if (need_texture_update && core.get_config().has(Work::gamecore::conf_versioning, "automatic version")) {
			download_this |= core.get_config().get(Work::gamecore::conf_versioning, "automatic version") != Work::version_app;
		}

		std::string cpy = datapath;
		Handling::handle_path(cpy);
		download_this |= Interface::quick_get_file_size(cpy) <= 0;

		if (download_this) {// || (val % 15 == 0 && !val)
			//if (size <= 0) logg << L::SLF << fsr() << "&cData files not found." << L::ELF;
			//else logg << L::SLF << fsr() << "&5After 15 times opening/closing, I'll update the zip just to be sure." << L::ELF;

			logg << L::SLF << fsr() << "&cNew version or file not found. Downloading latest resource pack..." << L::ELF;

			logg << L::SLF << fsr() << "Downloading..." << L::ELF;

			Downloader down;
			auto fut = down.get_async(resource_url);

			while (!fut.get_ready(1000)) {
				logg << L::SLF << fsr() << "This may take a while, please wait... (downloaded " << Tools::byte_auto_string(down.bytes_read()) << "b)" << L::ELF;
			}

			logg << L::SLF << fsr() << "Opening file to save downloaded zip (size " << Tools::byte_auto_string(down.bytes_read()) << "b)..." << L::ELF;

			SmartFile fp;
			if (!fp.open(cpy, smartfile::file_modes::WRITE)) {
				logg << L::SLF << fsr() << "&4Can't open path!&ePlease make sure this app can write into '" << cpy << "'." << L::ELF;
				std::terminate();
			}

			logg << L::SLF << fsr() << "Flushing file..." << L::ELF;

			fp.write(*fut.get());

			logg << L::SLF << fsr() << "Saving file..." << L::ELF;

			fp.close();

			logg << L::SLF << fsr() << "&aDone." << L::ELF;
		}
		else {
			logg << L::SLF << fsr() << "&aData file found, so no download being done (for now)." << L::ELF;
		}
	}
}


void multitask_test(Work::GameCore & core)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > MULTITASK TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr() << "Setting up thread..." << L::ELF;

		Tools::SuperThread<int> thr([](Tools::boolThreadF b) {
			int a = 0;
			while (b()) a++;
			return a;
			});

		logg << L::SLF << fsr() << "Starting thread..." << L::ELF;
		auto fut = thr.start();

		logg << L::SLF << fsr() << "Waiting 200 ms..." << L::ELF;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		logg << L::SLF << fsr() << "Stopping thread..." << L::ELF;

		thr.stop();
		thr.join();

		logg << L::SLF << fsr() << "Value got: " << fut.get() << L::ELF;

	}
	logg << L::SLF << fsr() << "&e> > > > > END OF MULTITASK TEST < < < < <" << L::ELF;

}

void socketsys_test(Work::GameCore & core, double* progression)
{
	*progression = 0e-2;
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > SOCKET TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr() << "Initializing host..." << L::ELF;

		*progression = 05e-2;
		Hosting host;
		*progression = 10e-2;

		logg << L::SLF << fsr() << "Initialized. Trying to create a client to connect to this host..." << L::ELF;

		Connection client;
		if (!client.connect()) {
			logg << L::SLF << fsr() << "Deu ruim" << L::ELF;
			*progression = 100e-2;
		}
		else {
			*progression = 30e-2;
			logg << L::SLF << fsr() << "Yay success." << L::ELF;


			std::shared_ptr<Connection> host_c = host.get_connection(0);

			*progression = 40e-2;

			const char* first_msg = "hello there my friend my name is Jonas and I'm here to test this thing out. If it has problems, it will show them now!";
			const char* second_msg = "bananas de pijamas are nice tho. Just testing a little bit of code with some huge messages, as you can see. This is bigger than min buf for sure.";


			logg << L::SLF << fsr() << "Trying to send from client to server..." << L::ELF;

			client.send_package(first_msg);
			*progression = 45e-2;

			logg << L::SLF << fsr() << "Recovering data sent on server..." << L::ELF;

			while (!host_c->has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
			*progression = 50e-2;

			auto res = host_c->get_next();

			logg << L::SLF << fsr() << "Did they match? " << ((res == first_msg) ? "YES" : "NO") << L::ELF;

			logg << L::SLF << fsr() << "Sending from server to client..." << L::ELF;

			host_c->send_package(second_msg);
			*progression = 55e-2;

			while (!client.has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
			*progression = 60e-2;

			res = client.get_next();

			logg << L::SLF << fsr() << "Did they match? " << ((res == second_msg) ? "YES" : "NO") << L::ELF;

			for (size_t pingin = 0; pingin < 6; pingin++) {
				logg << L::SLF << fsr() << "Local ping try #" << pingin << ": " << L::ELF;
				logg << L::SLF << fsr() << "- Client: " << client.get_ping() << " ms" << L::ELF;
				logg << L::SLF << fsr() << "- Server: " << host_c->get_ping() << " ms" << L::ELF;
				*progression = 60e-2 + 40e-2 * ((pingin + 1) * 1.0 / 6);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}
	logg << L::SLF << fsr() << "&e> > > > > END OF SOCKET TEST < < < < <" << L::ELF;

}

void smartfile_test(Work::GameCore & core)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > FILE TEST < < < < <" << L::ELF;
	{
		Interface::SmartFile file;

		logg << L::SLF << fsr() << "Opening &9test_example.txt&f..." << L::ELF;

		if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_KEEP))
		{
			logg << L::SLF << fsr() << "Opened in READ_WRITE_KEEP." << L::ELF;

		}
		else {
			if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_OVERWRITE)) {
				logg << L::SLF << fsr() << "Opened in READ_WRITE_OVERWRITE." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(E::ERRR) << "Cannot open file." << L::ELF;
			}
		}

		if (file.is_open()) {
			std::string buf;

			if (file.read(buf, 256)) {
				logg << L::SLF << fsr() << "Got this from file: " << buf << L::ELF;
			}
			else {
				logg << L::SLF << fsr() << "Cannot read from file." << L::ELF;
			}

			file.seek(0, smartfile::file_seek::BEGIN);

			file.write("this is a huge test.");
			logg << L::SLF << fsr() << "Set file value." << L::ELF;

			file.close();
		}

	}
	logg << L::SLF << fsr() << "&e> > > > > END OF FILE TEST < < < < <" << L::ELF;

}

void pathmngr_debug(Work::GameCore & core, const Interface::PathManager & pather)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > PATHMANAGER TEST < < < < <" << L::ELF;

	pather.apply();

	logg << L::SLF << fsr() << "Paths found there:" << L::ELF;

	for (auto& i : pather.paths_set()) {
		logg << L::SLF << fsr() << "> &9" << i << L::ELF;
	}
	logg << L::SLF << fsr() << "Files found there:" << L::ELF;

	for (auto& i : pather.files_in_paths()) {
		logg << L::SLF << fsr() << "> &5" << i.path << " [" << i.size_str << "]" << L::ELF;
	}

	pather.unapply();

	logg << L::SLF << fsr() << "&e> > > > > END OF PATHMANAGER TEST < < < < <" << L::ELF;

}