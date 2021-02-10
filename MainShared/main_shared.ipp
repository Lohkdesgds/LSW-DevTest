#include "../LSW_Shared/LSW/LSWv5.h"
#include <iostream>

#ifndef _DEBUG
#define TESTING_ALL
#endif

#define TESTING_ALL

using namespace LSW::v5;
using namespace LSW::v5::Interface;

const std::string logpath = "%appdata%/Lohk's Studios/TheBlast/log/log.log";
const std::string configpath = "%appdata%/Lohk's Studios/TheBlast/config/config.lohk";
const std::string datapath = "%appdata%/Lohk's Studios/TheBlast/data/data.zip";

const std::string resource_url = "https://www.dropbox.com/s/tuc0mu4eotoh7ay/data.zip?dl=1";

const std::string music_test_inner_path = "musics/music_alt.ogg";
const int font_size_set = 200;
const bool need_texture_update = false;// if I change something, set to true


bool check_file_download(Work::GameCore&);
bool multitask_test(Work::GameCore&, double*, Tools::Resource<Work::ProgressBar>&);
bool socketsys_test(Work::GameCore&, double*, Tools::Resource<Work::ProgressBar>&);
bool smartfile_test(Work::GameCore&);
bool pathmngr_debug(Work::GameCore&, const Interface::PathManager&);

int main() {

	//Logger::show_debug_anywhere(true);

	Work::GameCore core(logpath, configpath);

	core.get_display().set_window_name("The Blast 2");

	std::string _what_test = "Loading";

	Tools::SuperResource<Work::ProgressBar> progressbar_source;
	Tools::SuperResource<Bitmap> bmps;
	Tools::SuperResource<Font> source_font;
	Tools::SuperResource<Work::Text> texts;
	Tools::SuperResource<Work::Button> buttons;

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
		
	auto progress2 = progressbar_source.create("_load_progress2");
	progress2->set(Work::sprite::e_boolean::DRAW, false);
	progress2->set(Work::progressbar::e_double::PROGRESS, 0.0);
	progress2->set(Work::progressbar::e_color::FOREGROUND_TOP_LEFT, Interface::Color{ 30, 125, 25 });
	progress2->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_LEFT, Interface::Color{ 20, 90, 32 });
	progress2->set(Work::progressbar::e_color::FOREGROUND_TOP_RIGHT, Interface::Color{ 50, 139, 17 });
	progress2->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_RIGHT, Interface::Color{ 30, 150, 24 });
	progress2->set(Work::progressbar::e_color::BACKGROUND_TOP_LEFT, Interface::Color{ 3, 40, 2 });
	progress2->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_LEFT, Interface::Color{ 2, 22, 1 });
	progress2->set(Work::progressbar::e_color::BACKGROUND_TOP_RIGHT, Interface::Color{ 6, 42, 5 });
	progress2->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_RIGHT, Interface::Color{ 5, 13, 4 });
	progress2->set(Work::progressbar::e_boolean::FIXED_BACKGROUND, true);
	progress2->set(Work::progressbar::e_double::BORDER_THICKNESS, 0.0); // disable
	progress2->set(Work::progressbar::e_double::SMOOTHNESS, 2.0);
	progress2->set(Work::sprite::e_double::SCALE_G, 0.75);
	progress2->set(Work::sprite::e_double::SCALE_Y, 0.023);
	progress2->set(Work::sprite::e_double::TARG_POSX, 0.0);
	progress2->set(Work::sprite::e_double::TARG_POSY, 0.765);
	
	auto progress3 = progressbar_source.create("_load_progress3");
	progress3->set(Work::sprite::e_boolean::DRAW, false);
	progress3->set(Work::progressbar::e_double::PROGRESS, 0.0);
	progress3->set(Work::progressbar::e_color::FOREGROUND_TOP_LEFT, Interface::Color{ 125, 30, 21 });
	progress3->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_LEFT, Interface::Color{ 80, 15, 19 });
	progress3->set(Work::progressbar::e_color::FOREGROUND_TOP_RIGHT, Interface::Color{ 145, 15, 22 });
	progress3->set(Work::progressbar::e_color::FOREGROUND_BOTTOM_RIGHT, Interface::Color{ 142, 27, 24 });
	progress3->set(Work::progressbar::e_color::BACKGROUND_TOP_LEFT, Interface::Color{ 45, 10, 5 });
	progress3->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_LEFT, Interface::Color{ 28, 8, 1 });
	progress3->set(Work::progressbar::e_color::BACKGROUND_TOP_RIGHT, Interface::Color{ 47, 11, 4 });
	progress3->set(Work::progressbar::e_color::BACKGROUND_BOTTOM_RIGHT, Interface::Color{ 20, 9, 2 });
	progress3->set(Work::progressbar::e_boolean::FIXED_BACKGROUND, true);
	progress3->set(Work::progressbar::e_double::BORDER_THICKNESS, 0.0); // disable
	progress3->set(Work::progressbar::e_double::SMOOTHNESS, 1.0);
	progress3->set(Work::sprite::e_double::SCALE_G, 0.75);
	progress3->set(Work::sprite::e_double::SCALE_Y, 0.023);
	progress3->set(Work::sprite::e_double::TARG_POSX, 0.0);
	progress3->set(Work::sprite::e_double::TARG_POSY, 0.795);

	auto _text_early = texts.create("__loading");
	_text_early->set(Work::sprite::e_boolean::DRAW, false);
	//_text_early->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	_text_early->set(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	_text_early->set<Tools::Cstring>(Work::text::e_cstring::STRING, [&] { return "&9" + _what_test + [] {std::string s; for (int u = 0; u < ((int)(al_get_time() * 1.4)) % 3; u++) s += '.'; return '.' + s; }(); });
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
	//_text_early2->set(Work::text::e_integer::FONT_SIZE, font_size_set);
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
		progress2->draw(cam);
		progress3->draw(cam);
		_text_early->draw(cam);
		_text_early2->draw(cam);
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	});

	//progress->set<double>(Work::progressbar::e_double::TOP_BOTTOM_OFFSET, 0.1);

	// test progressbar
	//while (1) std::this_thread::sleep_for(std::chrono::seconds(5));

	Logger logg;

	logg << L::SLF << fsr() << "&7#======&7================================&7======#" << L::ELF;
	logg << L::SLF << fsr() << "&7#      &eThis is not a game, just a test.&7      #" << L::ELF;
	logg << L::SLF << fsr() << "&7#======&7================================&7======#" << L::ELF;


	progress->set(Work::progressbar::e_double::PROGRESS, 2e-2);

	if (!check_file_download(core)) {
		logg << L::SLF << fsr(E::ERRR) << "&cFailed to run \"check_file_download\"." << L::ELF;
		core.shutdown();
		return -1;
	}
	else {
		logg << L::SLF << fsr() << "&a\"check_file_download\" passed the test successfully." << L::ELF;
	}

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

#ifdef TESTING_ALL

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "MultiTask test";


	{
		double dd = 0.0;
		progress->set<double>(Work::progressbar::e_double::PROGRESS, [&] {return 10e-2 + 20e-2 * dd; });
		progress3->set<double>(Work::progressbar::e_double::PROGRESS, [&] {return 100e-2 * dd; });
		progress3->set(Work::sprite::e_boolean::DRAW, true);

		if (!multitask_test(core, &dd, progress2)) {
			logg << L::SLF << fsr(E::ERRR) << "&cFailed to run \"multitask_test\"." << L::ELF;
			core.shutdown();
			return -1;
		}
		else {
			logg << L::SLF << fsr() << "&a\"multitask_test\" passed the test successfully." << L::ELF;
		}
		progress2->set(Work::sprite::e_boolean::DRAW, false);
		progress3->set(Work::sprite::e_boolean::DRAW, false);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "Sockets test";

	{
		double dd = 0.0;
		progress->set<double>(Work::progressbar::e_double::PROGRESS, [&] {return 30e-2 + 50e-2 * dd; });
		progress3->set<double>(Work::progressbar::e_double::PROGRESS, [&] {return 100e-2 * dd; });
		progress3->set(Work::sprite::e_boolean::DRAW, true);
		
		if (!socketsys_test(core, &dd, progress2)) {
			logg << L::SLF << fsr(E::ERRR) << "&cFailed to run \"socketsys_test\"." << L::ELF;
			core.shutdown();
			return -1;
		}
		else {
			logg << L::SLF << fsr() << "&a\"socketsys_test\" passed the test successfully." << L::ELF;
		}
		progress2->set(Work::sprite::e_boolean::DRAW, false);
		progress3->set(Work::sprite::e_boolean::DRAW, false);
	}

	progress->set(Work::progressbar::e_double::PROGRESS, 80e-2);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "SmartFile test";

	if (!smartfile_test(core)) {
		logg << L::SLF << fsr(E::ERRR) << "&cFailed to run \"smartfile_test\"." << L::ELF;
		core.shutdown();
		return -1;
	}
	else {
		logg << L::SLF << fsr() << "&a\"smartfile_test\" passed the test successfully." << L::ELF;
	}

	progress->set(Work::progressbar::e_double::PROGRESS, 85e-2);
#endif

	//core.get_display().set_double_buffering_scale(0.3);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_what_test = "PathManager Test";

	if (!pathmngr_debug(core, pather)) {
		logg << L::SLF << fsr(E::ERRR) << "&cFailed to run \"pathmngr_debug\"." << L::ELF;
		core.shutdown();
		return -1;
	}
	else {
		logg << L::SLF << fsr() << "&a\"pathmngr_debug\" passed the test successfully." << L::ELF;
	}


	// * * * * * * * * * * * * * * * * * * EVERY TEST PASSED! YAY!  * * * * * * * * * * * * * * * * * * //

	logg << L::SLF << fsr() << "&aEvery test was a success!" << L::ELF;

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	_what_test = "Loading textures";

	Tools::SuperResource<Work::Block> blocks;
	Tools::SuperResource<Work::BubbleFX> bubbles;
	Tools::SuperResource<Work::ShineFX> shiners;


	auto working_on = core.get_display().add_once_task([pather] {
		Logger logg;
		Tools::SuperResource<Bitmap> source;

		auto atlas = source.create("ATLAS_GLOBAL");
		auto lmao = source.create("_test");
		auto dirt = source.create("_dirt");
		auto mouse = source.create("_mouse");
		auto icon = source.create("_icon");

		// use .zip
		pather.apply();

		atlas->load("atlas0.png");
		bool gud = *atlas;

		if (!gud) logg << L::SLF << fsr(E::WARN) << "Failed to load atlas." << L::ELF;

		if (gud) {
			gud = lmao->create_sub(atlas, 0, 1536, 512, 512);
			gud = gud && dirt->create_sub(atlas, 0, 1024, 512, 512);
			gud = gud && mouse->create_sub(atlas, 1536, 1024, 256, 256);
			gud = gud && icon->create_sub(atlas, 1536, 1280, 256, 256);
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

	Tools::Resource<Bitmap> _icon;
	if (!bmps.get("_icon", _icon)) {
		logg << L::SLF << fsr(E::ERRR) << "Texture wasn't found somehow..." << L::ELF;
		core.shutdown();
		return -1;
	}
	else {
		core.get_display().set_window_icon(_icon);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	_what_test = "Done loading textures";

	bool __simple_texts = false;

	Work::TextInput testtin;
	testtin.get_text().set(sfont);
	//testtin.get_text().set(Work::text::e_integer::FONT_SIZE, font_size_set);
	testtin.get_text().set<Tools::Cstring>(Work::text::e_cstring::STRING, "insert here");
	//testtin.get_text().set(Work::sprite::e_double::TARG_POSY, 0.1);
	testtin.get_text().set(Work::sprite::e_double::SCALE_G, 0.12);
	testtin.get_text().set(Work::sprite::e_double::SCALE_X, 0.85);
	testtin.get_text().set<int>(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	testtin.get_text().set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 1.00);
	testtin.get_text().set(Work::text::e_double::UPDATES_PER_SECOND, 15.0);
	testtin.get_text().set(Work::text::e_color::SHADOW_COLOR, Interface::Color(100, 100, 150));
	testtin.get_text().set(Work::text::e_double::SHADOW_DISTANCE_X, 0.005);
	testtin.get_text().set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.005);
	testtin.get_text().set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	testtin.get_text().set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	testtin.get_text().set(Work::text::e_integer::STRING_Y_MODE, static_cast<int>(Work::text::e_text_y_modes::CENTER));
	testtin.get_text().set(Work::text::e_double::LINE_ADJUST, 0.68);
	testtin.get_text().set(Work::text::e_double::MAX_TEXT_LENGTH_SIZE, 0.5);
	testtin.get_text().set(Work::text::e_integer::LINE_MAX_LENGTH, 50);
	testtin.get_text().set(Work::text::e_integer::MAX_LINES_AMOUNT, 2);
	testtin.get_text().set(Work::text::e_boolean::SCROLL_INSTEAD_OF_MAX_LEN_SIZE_BLOCK, false);
	testtin.main().set(Work::textinput::e_boolean::ENTER_BREAK_LINE, true);
	testtin.main().set<Work::sprite::functional>(Work::textinput::e_tie_functional::SAVED_STRING, [](const Tools::Any& str) {
		Logger logg;
		logg << L::SL << fsr() << "Saved string: " << str.get<Tools::Cstring>() << L::EL;
		});

	testtin.get_block().set(Work::sprite::e_double::TARG_POSX, 0.2);
	testtin.get_block().set(Work::sprite::e_double::TARG_POSY, -0.6);
	testtin.get_block().set(Work::sprite::e_double::SCALE_G, 0.25);
	testtin.get_block().set(Work::sprite::e_double::SCALE_X, 2.5);
	testtin.get_block().set<Interface::Color>(Work::sprite::e_color::COLOR, Interface::Color(180, 180, 180, 90));
	testtin.get_block().set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	testtin.get_block().set(Work::sprite::e_boolean::DRAW_COLOR_BOX, true);
	testtin.get_block().set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~~~~~~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - BLOCKS&BUTTONS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
	auto btn_switch = buttons.create("_test_switch");
	auto bubblr = bubbles.create("_test_bubblr");
	auto shinfx = shiners.create("_test_shinee");

	blk0->insert(lmao);
	blk_mouse->insert(mouse);
	blk3->insert(lmao);
	blk1->insert(lmao);
	blk_height->insert(lmao);
	blk_zoom->insert(lmao);
	blk_width->insert(lmao);
	//btn_switch->insert(dirt);

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	_what_test = "Setting up Blocks";


	btn_switch->get_text().set(sfont);
	//btn_switch->get_text().set(Work::text::e_integer::FONT_SIZE, font_size_set);
	btn_switch->get_text().set<Tools::Cstring>(Work::text::e_cstring::STRING, [&] { return __simple_texts ? "&6SWITCH" : "&dSWITCH"; });
	//btn_switch->get_text().set(Work::sprite::e_double::TARG_POSY, 0.1);
	btn_switch->get_text().set(Work::sprite::e_double::SCALE_G, 0.10);
	btn_switch->get_text().set(Work::sprite::e_double::SCALE_X, 0.85);
	btn_switch->get_text().set<int>(Work::text::e_integer::STRING_MODE, static_cast<int>(Work::text::e_text_modes::CENTER));
	btn_switch->get_text().set(Work::text::e_double::BUFFER_SCALE_RESOLUTION, 0.40);
	btn_switch->get_text().set(Work::text::e_double::UPDATES_PER_SECOND, 1.0);
	btn_switch->get_text().set(Work::text::e_color::SHADOW_COLOR, Interface::Color(100, 100, 150));
	btn_switch->get_text().set(Work::text::e_double::SHADOW_DISTANCE_X, 0.005);
	btn_switch->get_text().set(Work::text::e_double::SHADOW_DISTANCE_Y, 0.005);
	btn_switch->get_text().set(Work::text::e_boolean::USE_BITMAP_BUFFER, true);
	btn_switch->get_text().set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);

	btn_switch->get_block().set(Work::sprite::e_double::TARG_POSX, 0.9);
	btn_switch->get_block().set(Work::sprite::e_double::TARG_POSY, -0.9);
	btn_switch->get_block().set(Work::sprite::e_double::SCALE_G, 0.1);
	btn_switch->get_block().set(Work::sprite::e_double::SCALE_X, 1.35);
	btn_switch->get_block().set<Interface::Color>(Work::sprite::e_color::COLOR, [&] { return Interface::Color(__simple_texts ? 127 : 75, __simple_texts ? 140 : 45, __simple_texts ? 95 : 75); });
	btn_switch->get_block().set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	btn_switch->get_block().set(Work::sprite::e_boolean::DRAW_COLOR_BOX, true);
	btn_switch->get_block().set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));
	btn_switch->get_block().set<Work::sprite::functional>(Work::sprite::e_tie_functional::COLLISION_MOUSE_CLICK, [&](const Tools::Any& ref) {
		__simple_texts = !__simple_texts;

		Logger logg;
		logg << L::SL << fsr() << "SWITCH TEXT " << (!__simple_texts ? "Y" : "N") << L::EL;
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
#ifdef _DEBUG
		logg << L::SL << fsr() << "Clicked." << L::EL;

		logg << L::SL << fsr() << "0 UPDATE_DELTA: " << blk0->get_direct<double>(Work::sprite::e_double_readonly::UPDATE_DELTA) << L::EL;
		logg << L::SL << fsr() << "2 UPDATE_DELTA: " << blk_mouse->get_direct<double>(Work::sprite::e_double_readonly::UPDATE_DELTA) << L::EL;
		logg << L::SL << fsr() << "3 UPDATE_DELTA: " << blk3->get_direct<double>(Work::sprite::e_double_readonly::UPDATE_DELTA) << L::EL;
		logg << L::SL << fsr() << "4 UPDATE_DELTA: " << blk1->get_direct<double>(Work::sprite::e_double_readonly::UPDATE_DELTA) << L::EL;
#else
		logg << L::SL << fsr() << "Uau vc sabe clicar, boa!" << L::EL;
#endif
		});


	blk_mouse->set<double>(Work::sprite::e_double::TARG_POSX, [&] { return core.get_config().get_as<double>("mouse", "rx"); });
	blk_mouse->set<double>(Work::sprite::e_double::TARG_POSY, [&] { return core.get_config().get_as<double>("mouse", "ry"); });
	blk_mouse->set<double>(Work::sprite::e_double::SCALE_G, 0.15);
	blk_mouse->set<double>(Work::sprite::e_double::TARG_ROTATION, [] {return 30.0 * al_get_time(); });
	blk_mouse->set(Work::sprite::e_integer::COLLISION_MODE, static_cast<int>(Work::sprite::e_collision_mode_cast::COLLISION_STATIC));
	//blk_mouse->set(Work::sprite::e_boolean::DRAW_COLOR_BOX, true);
	blk_mouse->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);



	blk3->set<double>(Work::sprite::e_double::TARG_POSX, -0.65);
	blk3->set<double>(Work::sprite::e_double::TARG_POSY, -0.5);
	blk3->set<double>(Work::sprite::e_double::SCALE_G, 0.20);
	blk3->set<double>(Work::sprite::e_double::SCALE_X, 0.85);
	blk3->set<double>(Work::sprite::e_double::DISTANCE_DRAWING_SCALE, 0.6);
	//blk3->set(Work::sprite::e_boolean::DRAW_COLOR_BOX, true);

	bubblr->set(Work::sprite::e_double::SCALE_G, 2.0);
	bubblr->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	bubblr->reset_positions(300);

	shinfx->set(Work::sprite::e_double::SCALE_G, 2.0);
	shinfx->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	shinfx->set<double>(Work::shinefx::e_double::CENTER_X, [] {return 0.5 * cos(al_get_time()); });
	shinfx->set<double>(Work::shinefx::e_double::CENTER_Y, [] {return 0.5 * sin(al_get_time()); });


	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	_what_test = "Starting Blocks' Collisioner";


	Work::Collisioner overlay_control(core.get_config());
	overlay_control.insert(blk_height);
	overlay_control.insert(blk_width);
	overlay_control.insert(btn_switch->get_block());
	overlay_control.insert(blk_zoom);
	overlay_control.insert(testtin.get_block());

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
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

	/*txt0->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtdebug->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtu->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtmouse->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtf0->set(Work::text::e_integer::FONT_SIZE, font_size_set);
	txtf1->set(Work::text::e_integer::FONT_SIZE, font_size_set);*/

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	_what_test = "Setting up Texts";


	Tools::Stopwatch sw; // noo need to mutex, []{} is called in sync with draw
	sw.prepare(17);
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
			"&9DEBUG BMP/TEXT DRAW:\n" + sw.generate_table_statistics() + "\n&eTotal time draw: " + sw.get_string_between(0, sw.last_point_valid());
		});
	txtdebug->set(Work::sprite::e_double::TARG_POSX, -1.0);
	txtdebug->set(Work::sprite::e_double::TARG_POSY, -0.69);
	txtdebug->set(Work::sprite::e_double::SCALE_G, 0.052);
	//txtdebug->set(Work::sprite::e_double::SCALE_X, 0.35);
	txtdebug->set(Work::text::e_double::LINE_ADJUST, 0.75);
	txtdebug->set(Work::sprite::e_boolean::AFFECTED_BY_CAM, false);
	txtdebug->set(Work::text::e_double::UPDATES_PER_SECOND, 1.0);
	txtdebug->set(Work::text::e_color::SHADOW_COLOR, Interface::Color(127, 0, 0));
	txtdebug->set<bool>(Work::sprite::e_boolean::DRAW, [&] {if (__simple_texts) sw.reset_max(); return !__simple_texts; });
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
	txtf0->set(Work::sprite::e_double::SCALE_G, 0.15);
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

	bool __use_bubble_instead = true;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - TEXTS - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - ~~~~~ - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //

	progress->set(Work::progressbar::e_double::PROGRESS, 100e-2);

	_what_test = "Setting up main camera";
	progress->set(Work::progressbar::e_double::SMOOTHNESS, 20.0);

	while (progress->get_direct<double>(Work::progressbar::e_double_readonly::PROGRESS_SMOOTH) < 99e-2) { std::this_thread::yield(); std::this_thread::sleep_for(std::chrono::milliseconds(250)); }

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	{
		core.get_display().remove_draw_task(_delete_drawtask);

		// cleanup
		progressbar_source.remove(progress);
		progress.reset();
		progressbar_source.remove(progress2);
		progress2.reset();
		progressbar_source.remove(progress3);
		progress3.reset();
		texts.remove(_text_early);
		_text_early.reset();
		texts.remove(_text_early2);
		_text_early2.reset();

		core.get_display().add_draw_task([=, &__use_bubble_instead, &__c, &sw, &testtin](const Camera& c) {
			///got->draw((Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, (Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, 0.3, 0.3);

			if (!__c) sw.start(); // 0
			if (__use_bubble_instead) bubblr->draw(c);
			else shinfx->draw(c);
			if (!__c) sw.click_one(); // 1
			blk0->draw(c);
			if (!__c) sw.click_one(); // 2
			blk1->draw(c);
			if (!__c) sw.click_one(); // 3
			blk_mouse->draw(c);
			if (!__c) sw.click_one(); // 4
			blk3->draw(c);
			if (!__c) sw.click_one(); // 5
			blk_height->draw(c);
			if (!__c) sw.click_one(); // 6
			blk_width->draw(c);
			if (!__c) sw.click_one(); // 7
			btn_switch->draw(c);
			if (!__c) sw.click_one(); // 8
			blk_zoom->draw(c);
			if (!__c) sw.click_one(); // 9
			txt0->draw(c);
			if (!__c) sw.click_one(); // 10
			txtu->draw(c);
			if (!__c) sw.click_one(); // 11
			txtmouse->draw(c);
			if (!__c) sw.click_one(); // 12
			txtf0->draw(c);
			if (!__c) sw.click_one(); // 13
			txtf1->draw(c);
			if (!__c) sw.click_one(); // 14
			txtdebug->draw(c);
			if (!__c) sw.click_one(); // 15
			testtin.draw(c);
			if (!__c) sw.click_one(); // 16

			if (++__c > 500) __c = 0;
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

			if (testtin.main().get_direct<bool>(Work::textinput::e_boolean_readonly::SELECTED)) return;

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
			case ALLEGRO_KEY_H:
			{
				__use_bubble_instead = !__use_bubble_instead;
				logg << L::SL << fsr() << "BubbleFX " << (__use_bubble_instead ? "<<" : ">>") << " ShineFX toggle" << L::EL;
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

bool check_file_download(Work::GameCore& core)
{
	Logger logg;
	bool good = true;

	// download resource
	{
		core.get_config().ensure("registry", "times_open", 0, Interface::config::config_section_mode::SAVE);
		auto val = core.get_config().get_as<unsigned long long>("registry", "times_open");
		core.get_config().set("registry", "times_open", val + 1);

		bool download_this = false;
#ifndef _DEBUG
		if (need_texture_update && core.get_config().has(Work::gamecore::conf_versioning, "automatic version")) {
			download_this |= core.get_config().get(Work::gamecore::conf_versioning, "automatic version") != Work::version_app;
		}
#endif

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
				logg << L::SLF << fsr(E::ERRR) << "&4Can't open path!&ePlease make sure this app can write into '" << cpy << "'." << L::ELF;
				good = false;
			}
			else {
				logg << L::SLF << fsr() << "Flushing file..." << L::ELF;

				fp.write(*fut.get());

				logg << L::SLF << fsr() << "Saving file..." << L::ELF;

				fp.close();

				logg << L::SLF << fsr() << "&aDone." << L::ELF;
			}
		}
		else {
			logg << L::SLF << fsr() << "&aData file found, so no download being done (for now)." << L::ELF;
		}
	}
	return good;
}


bool multitask_test(Work::GameCore& core, double* perc, Tools::Resource<Work::ProgressBar>& aux_bar)
{
	if (!perc) return false;

	aux_bar->set(Work::sprite::e_boolean::DRAW, true);
	aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

	Logger logg;
	bool bad = false;

	logg << L::SLF << fsr() << "&e> > > > > MULTITASK TEST < < < < <" << L::ELF;
	{
		bool k_running = false;
		*perc = 0.0;

		const auto func = [&](Tools::boolThreadF b) {
			k_running = true;
			long long a = 0;
			while (b()) a++;
			return a;
		};

		/*
		HIGH_PERFORMANCE, // undefined max number of events
		PERFORMANCE, // up to 10000 events / sec
		BALANCED, // up to 2000 events / sec
		LOW_POWER, // up to 500 events / sec
		VERY_LOW_POWER, // up to 125 events / sec
		EXTREMELY_LOW_POWER // up to 40 events / sec
		*/

		long long modes[Tools::superthread::performance_mode_count]{};
		Tools::SuperThread<long long> thr;
		for (auto& i : modes) i = 0;

		for (int i = 0; i < Tools::superthread::performance_mode_count; i++) {

			aux_bar->set(Work::progressbar::e_double::PROGRESS, 80e-2 * i / Tools::superthread::performance_mode_count);
			*perc = (80e-2 * i / Tools::superthread::performance_mode_count);

			switch (static_cast<Tools::superthread::performance_mode>(i)) {
			case Tools::superthread::performance_mode::NO_CONTROL:
				logg << L::SLF << fsr() << "&eSetting up thread on NO_CONTROL mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::HIGH_PERFORMANCE:
				logg << L::SLF << fsr() << "&eSetting up thread on HIGH_PERFORMANCE mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::PERFORMANCE:
				logg << L::SLF << fsr() << "&eSetting up thread on PERFORMANCE mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::BALANCED:
				logg << L::SLF << fsr() << "&eSetting up thread on BALANCED mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::LOW_POWER:
				logg << L::SLF << fsr() << "&eSetting up thread on LOW_POWER mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::VERY_LOW_POWER:
				logg << L::SLF << fsr() << "&eSetting up thread on VERY_LOW_POWER mode..." << L::ELF;
				break;
			case Tools::superthread::performance_mode::EXTREMELY_LOW_POWER:
				logg << L::SLF << fsr() << "&eSetting up thread on EXTREMELY_LOW_POWER mode..." << L::ELF;
				break;
			}

			thr.set(func);
			thr.set_performance_mode(static_cast<Tools::superthread::performance_mode>(i));

			logg << L::SLF << fsr() << "Starting thread and waiting about 1/2 sec..." << L::ELF;
			
			k_running = false;
			auto fut = thr.start();
			while (!k_running) std::this_thread::yield();

			std::this_thread::sleep_for(std::chrono::milliseconds(499));

			thr.stop();
			thr.join();

			logg << L::SLF << fsr() << "Stopped thread. Value got: " << FormatAs("%07lld") << fut.get() << L::ELF;

			modes[i] = fut.get();
		}

		*perc = 80e-2;
		aux_bar->set(Work::progressbar::e_double::PROGRESS, 80e-2);

		logg << L::SLF << fsr() << "Processing data..." << L::ELF;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		for (int i = 1; i < Tools::superthread::performance_mode_count; i++) {
			*perc = (80e-2 + 20e-2 * i / Tools::superthread::performance_mode_count);
			aux_bar->set(Work::progressbar::e_double::PROGRESS, (80e-2 + 20e-2 * i / Tools::superthread::performance_mode_count));

			if (modes[i] > modes[i - 1]) {
				logg << L::SLF << fsr(E::WARN) << "&cA faster thread mode performed worse once! &6(&e" << modes[i] << " > " << modes[i - 1] << "&6)" << L::ELF;
				bad = true;
			}
			else {
				logg << L::SLF << fsr() << "&aCheck! &b" << FormatAs("%07lld") << modes[i] << "&e <= &b" << FormatAs("%07lld") << modes[i - 1] << " &6(" << FormatAs("%03.2lf") << (100.0 - (modes[i] * 100.0 / modes[i-1])) << "&6% slower)&a." << L::ELF;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}

		*perc = 100e-2;
	}

	logg << L::SLF << fsr() << "&e> > > > > END OF MULTITASK TEST < < < < <" << L::ELF;

	aux_bar->set(Work::sprite::e_boolean::DRAW, false);

	return !bad;
}

struct __socketsys_test_struct {
	unsigned int good = 0;
	float data = 1.4f;
};

bool socketsys_test(Work::GameCore& core, double* perc, Tools::Resource<Work::ProgressBar>& aux_bar)
{
	if (!perc) return false;
	//Logger::show_debug_anywhere(true);


	bool _end_result = true;

	*perc = 0e-2;
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > SOCKET TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr() << "Initializing host..." << L::ELF;

		*perc = 03e-2;
		Hosting host;

		host.on_new_connection([&](auto ptr) {
			logg << L::SLF << fsr() << "Woo hoo new connection lmao!" << L::ELF;
			});
		host.on_connection_close([&](auto ptr) {
			logg << L::SLF << fsr() << "Disconnected wooo!" << L::ELF;
		});

		*perc = 05e-2;

		logg << L::SLF << fsr() << "Initialized. Trying to create a client to connect to this host..." << L::ELF;

		Connection client;
		if (!client.connect()) {
			logg << L::SLF << fsr() << "No luck" << L::ELF;
			_end_result = false;
			*perc = 100e-2;
		}
		else {
			*perc = 10e-2;

			aux_bar->set(Work::sprite::e_boolean::DRAW, true);
			aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

			logg << L::SLF << fsr() << "Yay success." << L::ELF;

			std::shared_ptr<Connection> host_c = host.get_connection(0);

			/*const auto f_perf = [&](const connection::_who w, const connection::_performance_adapt p) {
				logg << L::SLF << fsr(E::DEBUG) << "&5[PERF_CHANGE] " << ((w == connection::_who::SEND) ? "&fSEND" : "&fRECV") << " &3-> " << ((p == connection::_performance_adapt::HIGH) ? "&6HIGH_" : (p == connection::_performance_adapt::FORCED ? "&6FORCED" : "&6LOW__")) << L::ELF;
			};
			
			host_c->set_performance_watchdog(f_perf);
			client.set_performance_watchdog(f_perf);

			logg << L::SLF << fsr() << "Tied function to see performance changes." << L::ELF;*/


			const char* first_msg = "hello there my friend my name is Jonas and I'm here to test this thing out. If it has problems, it will show them now!";
			const char* second_msg = "bananas de pijamas are nice tho. Just testing a little bit of code with some huge messages, as you can see. This is bigger than min buf for sure.";

			aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);
			logg << L::SLF << fsr() << "Trying to send from client to server..." << L::ELF;

			client.send_package(first_msg);
			*perc = 12e-2;

			logg << L::SLF << fsr() << "Recovering data sent on server..." << L::ELF;

			host_c->wait_for_package();
			*perc = 14e-2;

			__combined_data res = host_c->get_next();

			bool match = (res.buffer == first_msg);
			if (!match) _end_result = false;
			aux_bar->set(Work::progressbar::e_double::PROGRESS, 100e-2);

			logg << L::SLF << fsr() << "Did they match? " << (match ? "YES" : "NO") << L::ELF;

			aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);
			logg << L::SLF << fsr() << "Sending from server to client..." << L::ELF;

			host_c->send_package(second_msg);
			*perc = 16e-2;

			client.wait_for_package();
			*perc = 18e-2;

			res = client.get_next();

			match = (res.buffer == second_msg);
			if (!match) _end_result = false;

			aux_bar->set(Work::progressbar::e_double::PROGRESS, 100e-2);
			logg << L::SLF << fsr() << "Did they match? " << (match ? "YES" : "NO") << L::ELF;

			*perc = 20e-2;
			// 20%

			{
				aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

				size_t server_overload_test = 0;
				size_t client_overload_test = 0;
				const size_t limit_set = 50;

				logg << L::SLF << fsr() << "Setting up overload test..." << L::ELF;

				host_c->set_max_buffering(limit_set);
				while (client.in_memory_can_send() != limit_set) std::this_thread::yield();
				logg << L::SLF << fsr() << "&aClient detected host's limit successfully" << L::ELF;
				client.set_max_buffering(limit_set);
				while (host_c->in_memory_can_send() != limit_set) std::this_thread::yield();
				logg << L::SLF << fsr() << "&aHost detected client's limit successfully" << L::ELF;

				logg << L::SLF << fsr() << "Overload test initialized, overloading host server / client recv now..." << L::ELF;

				std::this_thread::sleep_for(std::chrono::milliseconds(500));


				while (!host_c->is_overloaded()) {
					host_c->send_package(first_msg, true);
					const double __n = server_overload_test * 1.0 / limit_set;
					aux_bar->set(Work::progressbar::e_double::PROGRESS, __n > 1.0 ? 1.0 : __n);
					if (server_overload_test++ > 3 * limit_set) break;
				}

				*perc = 27e-2;

				logg << L::SLF << fsr() << "Checking for overloaded tag..." << L::ELF;

				if (host_c->is_overloaded()) {
					logg << L::SLF << fsr() << "&aYes, overloaded" << L::ELF;
				}
				else {
					logg << L::SLF << fsr(E::WARN) << "&cNope, not overloaded." << L::ELF;
					_end_result = false;
				}
				aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

				logg << L::SLF << fsr() << "Overloading client send / server recv now..." << L::ELF;

				while (!client.is_overloaded()) {
					client.send_package(second_msg, true);
					const double __n = client_overload_test * 1.0 / limit_set;
					aux_bar->set(Work::progressbar::e_double::PROGRESS, __n > 1.0 ? 1.0 : __n);
					if (client_overload_test++ > 3 * limit_set) break;
				}

				*perc = 35e-2;

				logg << L::SLF << fsr() << "Checking for overloaded tag..." << L::ELF;

				if (client.is_overloaded()) {
					logg << L::SLF << fsr() << "&aYes, overloaded" << L::ELF;
				}
				else {
					logg << L::SLF << fsr(E::WARN) << "&cNope, not overloaded." << L::ELF;
					_end_result = false;
				}

				logg << L::SLF << fsr() << "&aTotal packages sent by server, client, and expected: " << L::ELF;
				logg << L::SLF << fsr() << "&b" << server_overload_test << " package(s)" << L::ELF;
				logg << L::SLF << fsr() << "&b" << client_overload_test << " package(s)" << L::ELF;
				logg << L::SLF << fsr() << "&e" << limit_set << " package(s)" << L::ELF;


				logg << L::SLF << fsr() << "Cleanup..." << L::ELF;
				aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

				*perc = 37e-2;

				while (host_c->has_package() || client.has_package()) {
					host_c->get_next();
					client.get_next();
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}

				logg << L::SLF << fsr() << "Done cleaup." << L::ELF;

				*perc = 40e-2;
			}

			// 40%

			host_c->set_max_buffering(0);
			client.set_max_buffering(0);



			if (_end_result) { // good
				auto now = client.get_packages_recv_bytes();

				const int tests = 10;
				const int baseline = 1; // 6 times
				const int power_up_to = 10; // 1, 2, 4, 8 ...

				unsigned long long total_data_len = 0;

				unsigned long long steps[3] = { 0,0,0 }; // microsec
				unsigned long long total_steps_avg = 0;

				logg << L::SLF << fsr() << "&6NO_CONTROL&f used for best scenario right now." << L::ELF;

				client.set_mode(Tools::superthread::performance_mode::NO_CONTROL);
				host_c->set_mode(Tools::superthread::performance_mode::NO_CONTROL);

				std::this_thread::sleep_for(std::chrono::seconds(1));

				for(int kk = 0; kk < power_up_to; kk++)
				{
					const std::string data(connection::package_size * baseline * (pow(kk + 1, 2)), 'a');
					total_data_len += data.size();

					logg << L::SLF << fsr() << "&6 - > Now testing speed with " << Tools::byte_auto_string(data.size(), 4) << "B single data burst (x" << tests << ")." << L::ELF;

					for (int u = 0; u < tests; u++) {
						*perc = 40e-2 + 45e-2 * ((static_cast<double>(u) + static_cast<double>(kk * tests)) * 1.0 / (static_cast<double>(tests) * static_cast<double>(power_up_to)));
						aux_bar->set(Work::progressbar::e_double::PROGRESS, static_cast<double>(u) / tests);

						bool sliced_somewhat = false;

						Tools::Stopwatch watch;
						watch.prepare(4);

						// SEND PACKAGE
						watch.start();

						host_c->send_package(data);

						// UNLOCKED SEND
						watch.click_one();

						client.wait_for_package();

						// HAS PACKAGE
						watch.click_one();

						__combined_data _temp;
						do {
							_temp += client.get_next();
							if (!_temp.is_full) sliced_somewhat = true;
						} while (!_temp.is_full);
						//while(host_c->still_sending()) std::this_thread::yield();

						// END
						watch.click_one();


						steps[0] += watch.get_microsec_between(0, 1).count();
						steps[1] += watch.get_microsec_between(1, 2).count();
						steps[2] += watch.get_microsec_between(2, 3).count();

						if (sliced_somewhat) {
							logg << L::SLF << fsr(E::WARN) << "&6Data was split. " << ((_temp.buffer.size() == data.size()) ? "&aBut they still match" : "&cAnd they didn't match ;-;") << L::ELF;
						}
						if (_temp.buffer.size() != data.size()) {
							logg << L::SLF << fsr(E::ERRR) << "&cData was broken! Size don't match!" << L::ELF;
							_end_result = false;
							break;
						}
						else {
							logg << L::SLF << fsr() << "&7Test &6#" << FormatAs("%03d") << (u + (kk * tests)) + 1 << "&7 of &6" << FormatAs("%03d") << (tests * power_up_to) << "&7 done, bitrate snapshot (approx): &6"
								<< Tools::byte_auto_string(1e6 * (data.size() * 1.0 / watch.get_microsec_between(0, 3).count()), 1) << "B/s&7." << L::ELF;
						}

						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}

				*perc = 85e-2;
				aux_bar->set(Work::progressbar::e_double::PROGRESS, 100e-2);

				client.reset_mode_default();
				host_c->reset_mode_default();

				for (auto& i : steps) {
					i /= tests * power_up_to;
					total_steps_avg += i;
				}
				total_steps_avg /= (sizeof(steps) / sizeof(steps[0]));

				if (_end_result) {

					logg << L::SLF << fsr()
						<< "Total delay speed while sending " << Tools::byte_auto_string(total_data_len, 1) << "B (average): " << FormatAs("%.3lf") << (1e-3 * total_steps_avg) << " ms " << L::ELF; //"&9(total " << Tools::byte_auto_string(1e6 * (total_data_len * 1.0 / total_steps_avg), 4) << "B/s)" << L::ELF;
					logg << L::SLF << fsr() << "&e- &aSEND_PACKAGE delay average: &b" << FormatAs("%.3lf") << (1e-3 * steps[0]) << " ms" << L::ELF;
					logg << L::SLF << fsr() << "&e- &aTransmission delay average: &b" << FormatAs("%.3lf") << (1e-3 * steps[1]) << " ms &9(possible raw " << Tools::byte_auto_string(1e6 * (total_data_len * 1.0 / steps[1]), 4) << "B/s)" << L::ELF;
					logg << L::SLF << fsr() << "&e- &aGET_NEXT delay average:     &b" << FormatAs("%.3lf") << (1e-3 * steps[2]) << " ms" << L::ELF;
				}

				*perc = 87e-2;
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}

			logg << L::SLF << fsr() << "Cleanup..." << L::ELF;
			aux_bar->set(Work::progressbar::e_double::PROGRESS, 0e-2);

			while (host_c->has_package() || client.has_package()) {
				host_c->get_next();
				client.get_next();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}

			*perc = 90e-2;
			// 90%

			logg << L::SLF << fsr() << "Done cleaup." << L::ELF;


			{
				logg << L::SLF << fsr() << "Sending complex data from server to client..." << L::ELF;

				__socketsys_test_struct some_data;
				some_data.data = 2.5f;
				some_data.good = Tools::random();

				host_c->send_package(Interface::transform_any_to_package(&some_data, sizeof(some_data)));

				client.wait_for_package();

				*perc = 93e-2;

				res = client.get_next();
				aux_bar->set(Work::progressbar::e_double::PROGRESS, 100e-2);

				__socketsys_test_struct result_data;

				if (transform_any_package_back(&result_data, sizeof(result_data), res.buffer)) {
					logg << L::SLF << fsr() << "Data type match (size-wide)..." << L::ELF;

					if (result_data.data == some_data.data && result_data.good == some_data.good) {
						logg << L::SLF << fsr() << "&aALL DATA MATCH!" << L::ELF;
					}
					else {
						logg << L::SLF << fsr(E::WARN) << "Data got messed up while converting." << L::ELF;
						_end_result = false;
					}
				}
				else {
					logg << L::SLF << fsr(E::WARN) << "Complex data failed to be converted back." << L::ELF;
					_end_result = false;
				}

				*perc = 97e-2;
			}

			logg << L::SLF << fsr() << "&5Local ping: " << L::ELF;
			logg << L::SLF << fsr() << "&5- &bClient: &6" << client.get_ping() << " ms" << L::ELF;
			logg << L::SLF << fsr() << "&5- &bServer: &6" << host_c->get_ping() << " ms" << L::ELF;

			logg << L::SLF << fsr() << "&5Full data analysis: " << L::ELF;

			*perc = 98e-2;

			logg << L::SLF << fsr() << "&d * * * CLIENT DETAILS * * * " << L::ELF;
			{
				const auto& net = client.get_network_info();
				logg << L::SLF << fsr() << "&2-&a ping_now:         &f" << net.ping_now()																<< "&7 ms" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a ping_average:     &f" << net.ping_average_now()														<< "&7 ms" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_total:       &f" << Tools::byte_auto_string(net.send_get_total(), 3, "&7")						<< "&7B" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_peak:        &f" << Tools::byte_auto_string(net.send_get_peak(), 3, "&7")						<< "&7B in one second" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_bps_now:     &f" << Tools::byte_auto_string(net.send_get_current_bytes_per_second(), 3, "&7")	<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_bps_alltime: &f" << Tools::byte_auto_string(net.send_get_average_total(), 3, "&7")				<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_total:       &f" << Tools::byte_auto_string(net.recv_get_total(), 3, "&7")						<< "&7B" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_peak:        &f" << Tools::byte_auto_string(net.recv_get_peak(), 3, "&7")						<< "&7B in one second" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_bps_now:     &f" << Tools::byte_auto_string(net.recv_get_current_bytes_per_second(), 3, "&7")	<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_bps_alltime: &f" << Tools::byte_auto_string(net.recv_get_average_total(), 3, "&7")				<< "&7B/s" << L::ELF;
			}

			*perc = 99e-2;

			logg << L::SLF << fsr() << "&d * * * HOST DETAILS * * * " << L::ELF;
			{
				const auto& net = host_c->get_network_info();
				logg << L::SLF << fsr() << "&2-&a ping_now:         &f" << net.ping_now()																<< "&7 ms" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a ping_average:     &f" << net.ping_average_now()														<< "&7 ms" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_total:       &f" << Tools::byte_auto_string(net.send_get_total(), 3, "&7")						<< "&7B" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_peak:        &f" << Tools::byte_auto_string(net.send_get_peak(), 3, "&7")						<< "&7B in one second" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_bps_now:     &f" << Tools::byte_auto_string(net.send_get_current_bytes_per_second(), 3, "&7")	<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a send_bps_alltime: &f" << Tools::byte_auto_string(net.send_get_average_total(), 3, "&7")				<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_total:       &f" << Tools::byte_auto_string(net.recv_get_total(), 3, "&7")						<< "&7B" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_peak:        &f" << Tools::byte_auto_string(net.recv_get_peak(), 3, "&7")						<< "&7B in one second" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_bps_now:     &f" << Tools::byte_auto_string(net.recv_get_current_bytes_per_second(), 3, "&7")	<< "&7B/s" << L::ELF;
				logg << L::SLF << fsr() << "&2-&a recv_bps_alltime: &f" << Tools::byte_auto_string(net.recv_get_average_total(), 3, "&7")				<< "&7B/s" << L::ELF;
			}

			*perc = 99e-2;

			client.close();
		}
	}
	logg << L::SLF << fsr() << "&e> > > > > END OF SOCKET TEST < < < < <" << L::ELF;

	*perc = 100e-2;
	aux_bar->set(Work::sprite::e_boolean::DRAW, false);

	Logger::show_debug_anywhere(false);
	return _end_result;
}

bool smartfile_test(Work::GameCore& core)
{
	Logger logg;
	bool good = true;

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
				good = false;
			}
		}

		if (file.is_open()) {
			std::string buf;

			if (file.read(buf, 256)) {
				logg << L::SLF << fsr() << "Got this from file: " << buf << L::ELF;

				file.seek(0, smartfile::file_seek::BEGIN);

				file.write("this is a huge test.");
				logg << L::SLF << fsr() << "Set file value." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(E::ERRR) << "Cannot read from file." << L::ELF;
				good = false;
			}

			file.close();
		}

	}
	logg << L::SLF << fsr() << "&e> > > > > END OF FILE TEST < < < < <" << L::ELF;
	return good;
}

bool pathmngr_debug(Work::GameCore& core, const Interface::PathManager& pather)
{
	Logger logg;
	bool found_one = false;

	logg << L::SLF << fsr() << "&e> > > > > PATHMANAGER TEST < < < < <" << L::ELF;

	pather.apply();

	logg << L::SLF << fsr() << "Paths found there:" << L::ELF;

	for (auto& i : pather.paths_set()) {
		logg << L::SLF << fsr() << "> &9" << i << L::ELF;
		found_one = true;
	}
	logg << L::SLF << fsr() << "Files found there:" << L::ELF;

	for (auto& i : pather.files_in_paths()) {
		logg << L::SLF << fsr() << "> &5" << i.path << " [" << i.size_str << "]" << L::ELF;
		found_one = true;
	}

	pather.unapply();

	logg << L::SLF << fsr() << "&e> > > > > END OF PATHMANAGER TEST < < < < <" << L::ELF;

	return found_one;
}