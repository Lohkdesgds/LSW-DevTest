#define LSW_THREAD_ALLEGRO

#include "Library\LSW5\logger\logger.h"
#include "Library\LSW5\camera\camera.h"
#include "Library\LSW5\database\database.h"
#include "Library\LSW5\core\core.h"
#include "Library\LSW5\display\display.h"
#include "Library\LSW5\superresource\superresource.h"
#include "Library\LSW5\sprite\sprite.h"

using namespace LSW::v5;


// game specific here stuff
const char project_name[] = PROJECTNAME;			// properties, c++, preprocessor definitions. Default autogen: 'PROJECTNAME="$(ProjectName)";%(PreprocessorDefinitions)'
const char project_company[] = "Lohk's Studios";	// manually put

const std::string discord_api_extract = std::string("%appdata%/") + project_company + "/" + project_name + "/data/discordapi.exe";
const std::string discord_api_extract_dll = std::string("%appdata%/") + project_company + "/" + project_name + "/data/discord_game_sdk.dll";
const std::string start_zip_default_extract_path = std::string("%appdata%/") + project_company + "/" + project_name + "/data/data.zip";
const std::string config_default_file = std::string("%appdata%/") + project_company + "/" + project_name + "/config/config.lohk";
const std::string default_file_log_path = std::string("%appdata%/") + project_company + "/" + project_name + "/logs/latest.log";
const std::string default_print_path = std::string("%win_photos_path%/Lohk's Studios Screenshots/");



int main() {
	Logger logg;
	logg.init(default_file_log_path);
	Database db(config_default_file);

	

	db.set(database::e_string::PRINT_PATH, default_print_path);
	db.set(database::e_string::DATA_PATH, start_zip_default_extract_path);

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


	

	Core core;

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Initializing CORE once - - -" << L::ELF;

	core.pause(); // set to pause before doing anything
	core.init();

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Waiting CORE to start and then wait - - -" << L::ELF;
	
	while(!core.allPaused());

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Initializing resources - - -" << L::ELF;

	SuperResource<Sprite_Base> sprites;
	auto* ref = sprites.create("test");
	ref->set("show_box", true);
	ref->set("show_dot", true);
	ref->set("scale_g", 0.3);
	ref->set("target_pos_y", 0.5);
	auto* ref2 = sprites.create("test2");
	ref2->set("show_box", true);
	ref2->set("show_dot", true);
	ref2->set("scale_g", 0.2);
	ref2->set("target_pos_y", -0.5);

	Camera cam;
	cam.set(camera::e_integer::ID, 0);
	cam.apply();
	{
		int gut;
		if (!cam.get(camera::e_integer::ID, gut)) {
			logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&6Couldn't get CAMERA::ID? hmm..." << L::ELF;
		}
	}

	core.unpause();

	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Unpaused CORE - - -" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__, E::DEBUG) << "&5 - - - Resting - - -" << L::ELF;

	for (ULONGLONG t = GetTickCount64(); GetTickCount64() - t < 5000;) {
		ref->set(sprite::e_double::TARG_POSX, 0.5 * cos(al_get_time()));
		ref2->set(sprite::e_double::TARG_POSX, 0.5 * cos(al_get_time() + 0.7));
	}


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

	db.close();
	logg.flush();
	return 0;
}