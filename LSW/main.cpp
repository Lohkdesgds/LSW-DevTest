#include "LSW/LSWv5.h"

#include <iostream>

#define TESTING_ALL

using namespace LSW::v5;
using namespace LSW::v5::Interface;

const std::string logpath = "%appdata%/Lohk's Studios/TheBlast/log/log.log";
const std::string configpath = "%appdata%/Lohk's Studios/TheBlast/config/config.lohk";
const std::string datapath = "%appdata%/Lohk's Studios/TheBlast/data/data.zip";

const std::string resource_url = "https://www.dropbox.com/s/tuc0mu4eotoh7ay/data.zip?dl=1";

const std::string music_test_inner_path = "musics/music_alt.ogg";


void check_file_download(Work::GameCore&);
void autoabort_test(Work::GameCore&);
void multitask_test(Work::GameCore&);
void socketsys_test(Work::GameCore&);
void smartfile_test(Work::GameCore&);
void pathmngr_debug(Work::GameCore&, const Interface::PathManager&);


int main() {

	Work::GameCore core(logpath, configpath);

	Logger logg;

	logg << L::SLF << fsr() << "&7#======&7====================================&7======#" << L::ELF;
	logg << L::SLF << fsr() << "&7#      &eThis is not a game yet, just a test.&7      #" << L::ELF;
	logg << L::SLF << fsr() << "&7#======&7====================================&7======#" << L::ELF;

	check_file_download(core);

#ifdef TESTING_ALL
	autoabort_test(core);
	multitask_test(core);
	socketsys_test(core);
	smartfile_test(core);
#endif

	Interface::PathManager pather;
	pather.add_path(datapath);

	pathmngr_debug(core, pather);



	auto working_on = core.get_display().add_once_task([pather] {
		Logger logg;
		Tools::SuperResource<Bitmap> source;
		Tools::SuperResource<Font> source_font;

		auto atlas = source.create("ATLAS_GLOBAL");
		auto lmao = source.create("_test");

		auto foo = source_font.create("DEFAULT");

		// use .zip
		pather.apply();

		foo->load("font.ttf");

		if (!foo) logg << L::SLF << fsr(E::WARN) << "Failed to load font." << L::ELF;
		else {
			logg << L::SLF << fsr() << "Loaded font successfully." << L::ELF;
		}

		atlas->load("atlas0.png");
		bool gud = *atlas;

		if (!gud) logg << L::SLF << fsr(E::WARN) << "Failed to load atlas." << L::ELF;

		if (gud) {
			gud = lmao->create_sub(atlas, 0, 1536, 512, 512);
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

		return lmao;
		});
	auto got = working_on.get().get<Tools::Resource<Bitmap>>();

	{
		Tools::SuperResource<Font> source_font;

		Tools::Resource<Interface::Font> lool;
		source_font.get("DEFAULT", lool);

		core.get_display().add_draw_task([=](Camera&) {
			got->draw((Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, (Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, 0.3, 0.3);
			//if (track->exists()) {
			//	float xx = 1.9f * ((1.0f * track->get_position_ms() / track->get_length_ms()) - 0.5f);
			//
			//	got->draw(got->get_width() / 2, got->get_height() / 2, xx, 0.9f, 0.0003, 0.0003);
			//}
		});

		core.get_display().add_draw_task([lool, logg, core](Camera& curr) {
			Camera cam2 = curr;
			const float prop = 2500.0f;
			cam2.classic_transform(0, 0, 1.0 / prop, 1.0 / prop, 0.0); // change this later somehow
			cam2.apply();
			lool->draw(-prop, -1.0f * prop, 0, "&5FPS: &a" + std::to_string(core.get_display().get_frames_per_second()) + (core.get_display().get_fps_cap() ? (" / " + std::to_string(core.get_display().get_fps_cap()) + " (limit)") : "") + (core.get_display().get_vsync() ? (" &6VSYNC SET") : " &bVSYNC UNSET"));
			lool->draw(-prop, -0.96f * prop, 0, "&2Times open: &b" + std::to_string(core.get_config().get_as<unsigned long long>("registry", "times_open")));
			lool->draw(-prop, -0.92f * prop, 0, "&7Mouse pos: &e" + std::to_string(core.get_config().get_as<float>(Work::gamecore::conf_mouse_memory, "x")) + "x" + std::to_string(core.get_config().get_as<float>(Work::gamecore::conf_mouse_memory, "y")));
			lool->draw(-prop, 0.96f * prop, 0, logg.get_last_line().filter_ascii_range());
			curr.apply();
		});
	}

	Tools::SuperResource<Camera> cameras;

	auto cam = cameras.create("DEFAULT");
	cam->classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);

	core.get_display().set_camera(cam);

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
	core.shutdown();

	std::this_thread::sleep_for(std::chrono::seconds(1));


	logg << L::SLF << fsr() << "&eThe end." << L::ELF;

	//debug("DEBUG TEST LSW");

	return 0;
}

void check_file_download(Work::GameCore& core)
{
	Logger logg;

	// download resource
	{
		core.get_config().ensure("registry", "times_open", 0, Interface::config::config_section_mode::SAVE);
		auto val = core.get_config().get_as<unsigned long long>("registry", "times_open");
		core.get_config().set("registry", "times_open", val + 1);

		std::string cpy = datapath;
		Handling::handle_path(cpy);
		auto size = Interface::quick_get_file_size(cpy);

		if (size <= 0) {// || (val % 15 == 0 && !val)
			//if (size <= 0) logg << L::SLF << fsr() << "&cData files not found." << L::ELF;
			//else logg << L::SLF << fsr() << "&5After 15 times opening/closing, I'll update the zip just to be sure." << L::ELF;

			logg << L::SLF << fsr() << "&cData files not found." << L::ELF;

			logg << L::SLF << fsr() << "Downloading..." << L::ELF;

			Downloader down;
			auto fut = down.get_async(resource_url);

			while (fut.get_ready(500)) {
				logg << L::SL << "Downloading..." << L::EL;
			}

			logg << L::SLF << fsr() << "Opening file..." << L::ELF;

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

void autoabort_test(Work::GameCore& core)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > AUTOABORT TEST < < < < <" << L::ELF;
	{
		{
			logg << L::SLF << fsr() << "Setting up thread mode=ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN 500 ms..." << L::ELF;

			unsigned count = 0, copy = 0;
			Tools::SuperThread on;
			on.set([&](Tools::boolThreadF) {
				Tools::AutoAbort aa(Tools::autoabort::abort_mode::ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN, 500, [&] {
					copy = count;
					});
				while (1) {
					count++;
				}
				});

			logg << L::SLF << fsr() << "Waiting..." << L::ELF;

			on.start();

			while (copy == 0)
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

			logg << L::SLF << fsr() << "Got results: Thread had " << count << " and abort copied " << copy << "." << L::ELF;

			on.kill();
		}
		//{
		//	logg << L::SLF << fsr() << "Setting up thread mode=ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF 500 ms..." << L::ELF;
		//
		//	unsigned count = 0, copy = 0;
		//	Tools::SuperThread on;
		//	on.set([&](Tools::boolThreadF) {
		//		Tools::AutoAbort aa(Tools::autoabort::abort_mode::ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF, 500, [&] {
		//			copy = count;
		//		});
		//		while (1) {
		//			count++;
		//		}
		//	});
		//
		//	logg << L::SLF << fsr() << "Waiting..." << L::ELF;
		//
		//	on.start();
		//
		//	while (copy == 0)
		//		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//
		//	logg << L::SLF << fsr() << "Got results: Thread had " << count << " and abort copied " << copy << "." << L::ELF;
		//
		//	on.kill();
		//} ///ref there [&] is invalid later at copy = count apparently
	}
	logg << L::SLF << fsr() << "&e> > > > > END OF AUTOABORT TEST < < < < <" << L::ELF;

}

void multitask_test(Work::GameCore& core)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > MULTITASK TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr() << "Setting up thread..." << L::ELF;

		Tools::SuperThreadT<int> thr([](Tools::boolThreadF b) {
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

void socketsys_test(Work::GameCore& core)
{
	Logger logg;

	logg << L::SLF << fsr() << "&e> > > > > SOCKET TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr() << "Initializing host..." << L::ELF;

		Hosting host;

		logg << L::SLF << fsr() << "Initialized. Trying to create a client to connect to this host..." << L::ELF;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		Connection client;
		if (!client.connect()) {
			logg << L::SLF << fsr() << "Deu ruim" << L::ELF;
		}
		else {
			logg << L::SLF << fsr() << "Yay success. Waiting a sec to send stuff to the host." << L::ELF;

			std::this_thread::sleep_for(std::chrono::seconds(1));

			std::shared_ptr<Connection> host_c = host.get_connection(0);

			std::this_thread::sleep_for(std::chrono::seconds(1));

			const char* first_msg = "hello there my friend my name is Jonas and I'm here to test this thing out. If it has problems, it will show them now!";
			const char* second_msg = "bananas de pijamas are nice tho. Just testing a little bit of code with some huge messages, as you can see. This is bigger than min buf for sure.";


			logg << L::SLF << fsr() << "Trying to send from client to server..." << L::ELF;

			client.send_package(first_msg);

			logg << L::SLF << fsr() << "Recovering data sent on server..." << L::ELF;

			while (!host_c->has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			auto res = host_c->get_next();

			logg << L::SLF << fsr() << "Did they match? " << ((res == first_msg) ? "YES" : "NO") << L::ELF;

			logg << L::SLF << fsr() << "Sending from server to client..." << L::ELF;

			host_c->send_package(second_msg);

			while (!client.has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			res = client.get_next();

			logg << L::SLF << fsr() << "Did they match? " << ((res == second_msg) ? "YES" : "NO") << L::ELF;

			for (size_t pingin = 0; pingin < 10; pingin++) {
				logg << L::SLF << fsr() << "Local ping try #" << pingin << ": " << L::ELF;
				logg << L::SLF << fsr() << "- Client: " << client.get_ping() << " ms" << L::ELF;
				logg << L::SLF << fsr() << "- Server: " << host_c->get_ping() << " ms" << L::ELF;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}
	logg << L::SLF << fsr() << "&e> > > > > END OF SOCKET TEST < < < < <" << L::ELF;

}

void smartfile_test(Work::GameCore& core)
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

void pathmngr_debug(Work::GameCore& core, const Interface::PathManager& pather)
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