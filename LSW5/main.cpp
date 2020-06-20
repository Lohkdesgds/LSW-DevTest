#define LSW_THREAD_ALLEGRO

#include "Library\LSW5\logger\logger.h"
#include "Library\LSW5\camera\camera.h"
#include "Library\LSW5\database\database.h"
#include "Library\LSW5\core\core.h"
#include "Library\LSW5\display\display.h"
#include "Library\LSW5\superresource\superresource.h"
#include "Library\LSW5\sprite\sprite.h"
#include "Library\LSW5\entities\entities.h"
#include "Library\LSW5\autocaster\autocaster.h"
#include "Library\LSW5\filesystem\filesystem.h"

using namespace LSW::v5;

// nvidia stuff
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}



// game specific here stuff
const char project_name[] = PROJECTNAME;			// properties, c++, preprocessor definitions. Default autogen: 'PROJECTNAME="$(ProjectName)";%(PreprocessorDefinitions)'
const char project_company[] = "Lohk's Studios";	// manually put

const std::string discord_api_extract = std::string("%appdata%/") + project_company + "/" + project_name + "/data/discordapi.exe";
const std::string discord_api_extract_dll = std::string("%appdata%/") + project_company + "/" + project_name + "/data/discord_game_sdk.dll";
const std::string start_zip_default_extract_path = std::string("%appdata%/") + project_company + "/" + project_name + "/data/data.zip";
const std::string start_zip_default_path = std::string("%appdata%/") + project_company + "/" + project_name + "/data";
const std::string config_default_file = std::string("%appdata%/") + project_company + "/" + project_name + "/config/config.lohk";
const std::string default_file_log_path = std::string("%appdata%/") + project_company + "/" + project_name + "/logs/latest.log";
const std::string default_print_path = std::string("%win_photos_path%/Lohk's Studios Screenshots/");

const std::string datapack_insertion_automatic = "NOT_READY_DATAPACK";


int main(int argc, char* argv[]) {
	Logger logg;
	logg.init(default_file_log_path);
	Database db(config_default_file);
	PhysFS fs;
	fs.hookPrint([&](std::string s) { logg << L::SLF << fsr(__FUNCSIG__, E::INFO) << "&5[PhysFS]&8 " << s << L::ELF; });



	SuperResource<Camera> cameras;
	SuperResource<Sprite_Base> sprites;
	SuperResource<ALLEGRO_BITMAP> bitmaps;
	SuperResource<ALLEGRO_FONT> fonts;

	db.set(database::e_string::PRINT_PATH, default_print_path);
	db.set(database::e_string::DATA_PATH, start_zip_default_extract_path);

	fs.addPath(start_zip_default_extract_path);
	fs.apply();
	fs.dir(); // just register what's inside lmao

	// test keyboard input
	db.set(database::e_boolean::SAVING_STRING_INPUT, true);

	// custom entries (if no reset, they will be there forever lmao)
	db.set("discord_api_extract", discord_api_extract);	
	db.set("discord_api_extract_dll", discord_api_extract_dll);

	{
		std::string t1, t2;
		db.get("discord_api_extract", t1);
		db.get("discord_api_extract_dll", t2);

		logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5Test: " << t1 << ", " << t2 << "." << L::ELF;
	}


	logg << L::SLF << fsr(__FUNCSIG__) << "&2Initializing game &a" << project_name << " &2by &a" << project_company << "&2..." << L::ELF;
	logg << L::SLF << fsr(__FUNCSIG__) << "&2Version: &a" << Shared::version_app << L::ELF;



	auto cam = cameras.create("MAIN_CAMERA");

	Core core;

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Initializing CORE once - - -" << L::ELF;

	core.pause(); // set to pause before doing anything
	core.init();

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Waiting CORE to start and then wait - - -" << L::ELF;
	
	while(!core.allPaused());

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Initializing resources - - -" << L::ELF;


	fonts.create("_FONT", "font.ttf"); // load main font
	bitmaps.create("_ATLAS0", "atlas0.png"); // load big atlas

	auto ref = sprites.create("test");
	ref->set("show_box", true);
	ref->set("show_dot", true);
	ref->set("scale_g", 0.3);
	ref->set("target_pos_x", -0.5);
	ref->set("target_pos_y", 0.2);
	ref->set(sprite::e_double::ACCELERATION_X, 6e-4);

	auto ref2 = sprites.create("test2");
	ref2->set("show_box", true);
	ref2->set("show_dot", true);
	ref2->set("scale_g", 0.2);
	ref2->set("target_pos_x", 0.5);
	ref2->set("target_pos_y", 0.2);
	ref2->set(sprite::e_double::ACCELERATION_X, -6e-4);

	auto ref3 = sprites.create("test3");
	ref3->set("show_box", true);
	ref3->set("show_dot", true);
	ref3->set("scale_g", 0.4);
	ref3->set("target_pos_x", 0.0);
	ref3->set("target_pos_y", 0.6);
	ref3->set(sprite::e_double::ACCELERATION_Y, -6e-4);

	auto ref4_orig = sprites.customLoad("test4", [](Sprite_Base*& b) {return (b = new Block()); });
	Block* ref4 = (Block*)&(*ref4_orig);
	ref4->set(sprite::e_boolean::SHOWBOX, true);
	ref4->set(sprite::e_boolean::SHOWDOT, true);
	ref4->set(sprite::e_boolean::DRAW, true);
	ref4->set(sprite::e_double::SCALE_G, 0.32);
	ref4->set(sprite::e_double::TARG_POSX, 0.0);
	ref4->set(sprite::e_double::TARG_POSY, -0.6);
	ref4->set(sprite::e_double::ACCELERATION_Y, 6e-4);
	ref4->loadCut("fatia_atlas_0", "_ATLAS0", 0, 1536, 512, 512);
	ref4->loadCut("fatia_atlas_1", "_ATLAS0", 0, 1024, 512, 512);
	ref4->set(entity::e_double::FRAMES_PER_SECOND, 4.0);

	auto ref5_orig = sprites.customLoad("test5", [](Sprite_Base*& b) {return (b = new Text()); });
	Text* ref5 = (Text*)&(*ref5_orig);
	//ref5->set(sprite::e_boolean::SHOWBOX, true);
	ref5->set(sprite::e_boolean::SHOWDOT, true);
	ref5->set(sprite::e_boolean::DRAW, true);
	ref5->set(sprite::e_double::TARG_POSX, 0.0);
	ref5->set(sprite::e_double::TARG_POSY, -0.7);
	ref5->set(sprite::e_color::COLOR, al_map_rgb(255, 255, 255));
	ref5->set(sprite::e_double::SCALE_G, 0.1);
	ref5->set(sprite::e_double::SCALE_X, 0.6);
	ref5->load("_FONT");
	ref5->set(text::e_cstring::STRING, "HELLO WORLD DAMN IT FPS=%int_fps%");
	ref5->set(text::e_integer::STRING_MODE, static_cast<int>(text::e_text_modes::CENTER));

	auto ref6_orig = sprites.customLoad("test6", [](Sprite_Base*& b) {return (b = new Text()); });
	Text* ref6 = (Text*)&(*ref6_orig);
	//ref6->set(sprite::e_boolean::SHOWBOX, true);
	ref6->set(sprite::e_boolean::SHOWDOT, true);
	ref6->set(sprite::e_boolean::DRAW, true);
	ref6->set(sprite::e_double::TARG_POSX, -1.0);
	ref6->set(sprite::e_double::TARG_POSY, -0.95);
	ref6->set(sprite::e_color::COLOR, al_map_rgb(255, 255, 255));
	ref6->set(sprite::e_double::SCALE_G, 0.07);
	ref6->set(sprite::e_double::SCALE_X, 0.4);
	ref6->load("_FONT");
	ref6->set(text::e_cstring::STRING, "%int_fps% qps, %int_tps% tps, %int_ups% ups, %int_aps% aps | Mouse: %mouse_x%,%mouse_y% | STR: curr= %curr_string%, last= %last_string% | Sprites=%num_sprites%");
	ref6->set(text::e_integer::STRING_MODE, static_cast<int>(text::e_text_modes::LEFT));

	
	{
		auto ref_orig = sprites.customLoad("_DEBUG_TEXT", [](Sprite_Base*& b) {return (b = new Text()); });
		Text* mtt = (Text*)&(*ref_orig);
		mtt->set(sprite::e_boolean::SHOWDOT, true);
		mtt->set(sprite::e_boolean::DRAW, true);
		mtt->set(text::e_boolean::USE_PER_CHAR_COLORING, true);
		mtt->set(sprite::e_double::TARG_POSX, -1.0);
		mtt->set(sprite::e_double::TARG_POSY, 0.92);
		mtt->set(sprite::e_color::COLOR, al_map_rgb(255, 255, 255));
		mtt->set(sprite::e_double::SCALE_G, 0.065);
		mtt->set(sprite::e_double::SCALE_X, 0.4);
		mtt->load("_FONT");
		mtt->set(text::e_cstring::STRING, "REPOLHO");
		mtt->set(text::e_integer::STRING_MODE, static_cast<int>(text::e_text_modes::LEFT));
		mtt->set(sprite::e_integer::LAYER, 100);
		logg.hook([=](coloured_string str) {mtt->set(text::e_cstring::STRING, str); });



		///logg.hook([=](coloured_string str) {mtt->set(text::e_cstring::STRING, str); });
		//mtt->set(text::e_cstring::STRING,			"REPOLHO");
		///mtt->set(sprite::e_boolean::DRAW,			[&]	{bool d;			db.get(database::e_boolean::WAS_OSD_ON, d);						return d; }());
		//mtt->set(sprite::e_double::SCALE_G, 0.03);
		//mtt->set(sprite::e_double::SCALE_X, 0.55);
		//mtt->set(sprite::e_double::TARG_POSY, 0.965); // 0.935
		//mtt->set(sprite::e_double::TARG_POSX, -1.0);
		///mtt->set(text::e_integer::STRING_MODE, +text::e_alignment::ALIGN_LEFT);
		//mtt->set(text::e_double::TEXT_UPDATE_TIME, 1.0 / 5);
		//mtt->set(sprite::e_boolean::AFFECTED_BY_CAM, false);
		//mtt->set(sprite::e_color::COLOR, al_map_rgb(255, 255, 255));
		///mtt->set(sprite::e_color::COLOR, [&] {return clr_str; }());
	}







	cam->set(camera::e_integer::ID, 0);
	cam->addLayerConfig(Camera::layer_each(0));
	cam->addLayerConfig(Camera::layer_each(100));
	cam->refresh();
	cam->apply();

	{
		int gut;
		if (!cam->get(camera::e_integer::ID, gut)) {
			logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&6Couldn't get CAMERA::ID? hmm..." << L::ELF;
		}
	}

	core.unpause();

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Unpaused CORE - - -" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Resting - - -" << L::ELF;


	while (core.allAlive());/* { // wait user do smth
		cam->set(camera::e_double::SCALE_G, cos(al_get_time() * 0.1) * 0.2 + 0.9);
		cam->set(camera::e_double::OFFSET_X, cos(al_get_time() * 0.04) * 0.3);
		cam->set(camera::e_double::OFFSET_Y, cos(al_get_time() * 0.11) * 0.3);
	}*/


	//for (ULONGLONG t = GetTickCount64(); GetTickCount64() - t < 25000 && core.allAlive(););// {
		//ref->set(sprite::e_double::TARG_POSX, 0.5 * cos(al_get_time()));
		//ref2->set(sprite::e_double::TARG_POSX, 0.5 * cos(al_get_time() + 0.7));
	//}
	//ref->set(sprite::e_double::ACCELERATION_X, -5e-4);
	//ref2->set(sprite::e_double::ACCELERATION_X, 5e-4);
	//for (ULONGLONG t = GetTickCount64(); GetTickCount64() - t < 15000 && core.allAlive(););


	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Ending CORE once - - -" << L::ELF;

	core.fEnd();


	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Resting - - -" << L::ELF;

	al_rest(3.0);

	//for (short kk = 0; kk < 10; kk++) {
	for (short u = 0; u < 16; u++) {
		char format[64];
		sprintf_s(format, "&%xRAINBOW", u);
		logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << format << " test" << L::ELF;
	}
	//}

	for (int a = 10; a > 0; a--) {
		logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&e" << a << " &6sec remaining" << L::ELF;
		Sleep(1000);
	}

	logg.flush();
	db.close();
	return 0;
}
