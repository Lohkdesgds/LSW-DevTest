#include "LSW/LSWv5.h"

#include <iostream>

using namespace LSW::v5;
using namespace LSW::v5::Interface;

const std::string logpath = "%appdata%/Lohk's Studios/TheBlast/log/log.log";
const std::string configpath = "%appdata%/Lohk's Studios/TheBlast/config/config.lohk";
const std::string datapath = "%appdata%/Lohk's Studios/TheBlast/data/data.zip";

const std::string resource_url = "https://www.dropbox.com/s/tuc0mu4eotoh7ay/data.zip?dl=1";

const std::string music_test_inner_path = "musics/music_alt.ogg";

int main() {

	Logger logg;
	{
		auto c = logpath;
		Handling::handle_path(c);
		logg.init(c);
	}

	logg << L::SLF << fsr(__FUNCSIG__) << "&7#======&7====================================&7======#" << L::ELF;
	logg << L::SLF << fsr(__FUNCSIG__) << "&7#      &eThis is not a game yet, just a test.&7      #" << L::ELF;
	logg << L::SLF << fsr(__FUNCSIG__) << "&7#======&7====================================&7======#" << L::ELF;
	

	Tools::SuperResource<Config> configs;

	auto conf = configs.create("DEFAULT");

	if (!conf->load(configpath)) {
		logg << L::SLF << fsr(__FUNCSIG__) << "&8No config file found... &eCreating one..." << L::ELF;

		conf->save_path(configpath);

		conf->set("config", "was_osd_on", false);
		conf->set("config", "second_screen_debug", false);
		conf->set("config", "ultradebug", false);
		conf->set("config", "double_buffering_screen", false);
		conf->set("config", "last_volume", 0.2);
		conf->set("config", "resolution_proportion", 1.000000);
		conf->set("config", "fx_amount", 0.000000);
		conf->set("config", "screen_width", 1280);
		conf->set("config", "screen_height", 720);
		conf->set("config", "last_display_flags", 0);
		conf->set("config", "pref_refresh_rate", 0);
		conf->set("config", "times_open", 0);
		conf->set("config", "playername", "NickName");
		conf->set("config", "last_version", "unknown");
		conf->set("config", "playercolor", { 0.0f, 0.0f, 0.0f });
		conf->set("config", "hidemouse", true);
		conf->set("config", "limit_framerate_to", 0);
		conf->set("config", "prints_path", "%win_photos_path%/Lohk's Studios Screenshots/");

		conf->flush();
		logg << L::SLF << fsr(__FUNCSIG__) << "&aConfig created and saved successfully." << L::ELF;
	}
	else {
		logg << L::SLF << fsr(__FUNCSIG__) << "&aConfig loaded successfully." << L::ELF;
	}

	/*try {
		conf->load(configpath);
	}
	catch (const Handling::Abort& a) {
		logg << L::SLF << fsr(__FUNCSIG__) << "&e4Fatal error: " << a << L::ELF;
	}*/


	// download resource
	{
		auto val = conf->get_as<unsigned long long>("config", "times_open");
		conf->set("config", "times_open", val + 1);

		std::string cpy = datapath;
		Handling::handle_path(cpy);
		auto size = Tools::get_file_size(cpy);

		if (size <= 0/* || (val % 15 == 0 && !val)*/) {
			/*if (size <= 0) */logg << L::SLF << fsr(__FUNCSIG__) << "&cData files not found." << L::ELF;
			//else logg << L::SLF << fsr(__FUNCSIG__) << "&5After 15 times opening/closing, I'll update the zip just to be sure." << L::ELF;

			logg << L::SLF << fsr(__FUNCSIG__) << "Downloading..." << L::ELF;

			Downloader down;
			auto fut = down.get_async(resource_url);

			while (fut.get_ready(500)) {
				logg << L::SL << "Downloading..." << L::EL;
			}

			logg << L::SLF << fsr(__FUNCSIG__) << "Opening file..." << L::ELF;

			SmartFile fp;
			if (!fp.open(cpy, smartfile::file_modes::WRITE)) {
				logg << L::SLF << fsr(__FUNCSIG__) << "&4Can't open path!&ePlease make sure this app can write into '" << cpy << "'." << L::ELF;
				return 1;
			}

			logg << L::SLF << fsr(__FUNCSIG__) << "Flushing file..." << L::ELF;

			fp.write(*fut.get());

			logg << L::SLF << fsr(__FUNCSIG__) << "Saving file..." << L::ELF;

			fp.close();

			logg << L::SLF << fsr(__FUNCSIG__) << "&aDone." << L::ELF;
		}
		else {
			logg << L::SLF << fsr(__FUNCSIG__) << "&aData file found, so no download being done (for now)." << L::ELF;
		}
	}



	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > TIMEDMEMORY TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr(__FUNCSIG__) << "Creating a int with value 10 that should last for about 300 ms..." << L::ELF;
		Tools::TimedMemory<int> testmemory(300, 10);
		logg << L::SLF << fsr(__FUNCSIG__) << "AutoMemory: &a" << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("&cnot alloc"); }() << L::ELF;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		logg << L::SLF << fsr(__FUNCSIG__) << "AutoMemory: &a" << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("&cnot alloc"); }() << L::ELF;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		logg << L::SLF << fsr(__FUNCSIG__) << "AutoMemory: &a" << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("&cnot alloc"); }() << L::ELF;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		logg << L::SLF << fsr(__FUNCSIG__) << "AutoMemory: &a" << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("&cnot alloc"); }() << L::ELF;
	}
	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > END OF TIMEDMEMORY TEST < < < < <" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > MULTITASK TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr(__FUNCSIG__) << "Setting up thread..." << L::ELF;

		Tools::SuperThreadT<int> thr([](Tools::boolThreadF b) {
			int a = 0;
			while (b()) a++;
			return a;
		});

		logg << L::SLF << fsr(__FUNCSIG__) << "Starting thread..." << L::ELF;
		auto fut = thr.start();

		logg << L::SLF << fsr(__FUNCSIG__) << "Waiting 200 ms..." << L::ELF;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		logg << L::SLF << fsr(__FUNCSIG__) << "Stopping thread..." << L::ELF;

		thr.stop();
		thr.join();

		logg << L::SLF << fsr(__FUNCSIG__) << "Value got: " << fut.get() << L::ELF;

	}
	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > END OF MULTITASK TEST < < < < <" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > SOCKET TEST < < < < <" << L::ELF;
	{
		logg << L::SLF << fsr(__FUNCSIG__) << "Initializing host..." << L::ELF;

		Hosting host;

		logg << L::SLF << fsr(__FUNCSIG__) << "Initialized. Trying to create a client to connect to this host..." << L::ELF;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		Connection client;
		if (!client.connect()) {
			logg << L::SLF << fsr(__FUNCSIG__) << "Deu ruim" << L::ELF;
		}
		else {
			logg << L::SLF << fsr(__FUNCSIG__) << "Yay success. Waiting a sec to send stuff to the host." << L::ELF;

			std::this_thread::sleep_for(std::chrono::seconds(1));

			std::shared_ptr<Connection> host_c = host.get_connection(0);

			std::this_thread::sleep_for(std::chrono::seconds(1));

			const char* first_msg = "hello there my friend my name is Jonas and I'm here to test this thing out. If it has problems, it will show them now!";
			const char* second_msg = "bananas de pijamas are nice tho. Just testing a little bit of code with some huge messages, as you can see. This is bigger than min buf for sure.";


			logg << L::SLF << fsr(__FUNCSIG__) << "Trying to send from client to server..." << L::ELF;

			client.send_package(first_msg);

			logg << L::SLF << fsr(__FUNCSIG__) << "Recovering data sent on server..." << L::ELF;

			while (!host_c->has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			auto res = host_c->get_next();

			logg << L::SLF << fsr(__FUNCSIG__) << "Did they match? " << ((res == first_msg) ? "YES" : "NO") << L::ELF;

			logg << L::SLF << fsr(__FUNCSIG__) << "Sending from server to client..." << L::ELF;

			host_c->send_package(second_msg);

			while (!client.has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			res = client.get_next();

			logg << L::SLF << fsr(__FUNCSIG__) << "Did they match? " << ((res == second_msg) ? "YES" : "NO") << L::ELF;
		}
	}
	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > END OF SOCKET TEST < < < < <" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > FILE TEST < < < < <" << L::ELF;
	{
		Interface::SmartFile file;

		logg << L::SLF << fsr(__FUNCSIG__) << "Opening &9test_example.txt&f..." << L::ELF;

		if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_KEEP))
		{
			logg << L::SLF << fsr(__FUNCSIG__) << "Opened in READ_WRITE_KEEP." << L::ELF;

		}
		else {
			if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_OVERWRITE)) {
				logg << L::SLF << fsr(__FUNCSIG__) << "Opened in READ_WRITE_OVERWRITE." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "Cannot open file." << L::ELF;
			}
		}

		if (file.is_open()) {
			std::string buf;
			
			if (file.read(buf, 256)) {
				logg << L::SLF << fsr(__FUNCSIG__) << "Got this from file: " << buf << L::ELF;
			}
			else {
				logg << L::SLF << fsr(__FUNCSIG__) << "Cannot read from file." << L::ELF;
			}

			file.seek(0, smartfile::file_seek::BEGIN);
			
			file.write("this is a huge test.");
			logg << L::SLF << fsr(__FUNCSIG__) << "Set file value." << L::ELF;

			file.close();
		}

	}
	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > END OF FILE TEST < < < < <" << L::ELF;


	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > PATHMANAGER TEST < < < < <" << L::ELF;

	Interface::PathManager pather;
	pather.add_path(datapath);
	pather.apply();

	logg << L::SLF << fsr(__FUNCSIG__) << "Paths found there:" << L::ELF;

	for (auto& i : pather.paths_set()) {
		logg << L::SLF << fsr(__FUNCSIG__) << "> &9" << i << L::ELF;
	}
	logg << L::SLF << fsr(__FUNCSIG__) << "Files found there:" << L::ELF;

	for (auto& i : pather.files_in_paths()) {
		logg << L::SLF << fsr(__FUNCSIG__) << "> &5" << i.path << " [" << i.size_str << "]" << L::ELF;
	}

	pather.unapply();

	logg << L::SLF << fsr(__FUNCSIG__) << "&e> > > > > ENDED SETUP OF PATHMANAGER < < < < <" << L::ELF;









	logg << L::SLF << fsr(__FUNCSIG__) << "Creating display..." << L::ELF;

	Display disp;

	if (auto k = conf->get_as<int>("config", "last_display_flags"); k != 0) disp.set_new_flags(k);
	if (auto k = conf->get_as<int>("config", "screen_width"); k != 0) disp.set_width(k);
	if (auto k = conf->get_as<int>("config", "screen_height"); k != 0) disp.set_height(k);

	Tools::Future<bool> disp_fut = disp.init();

	logg << L::SLF << fsr(__FUNCSIG__) << "Created display." << L::ELF;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto working_on = disp.add_once_task([pather] {		
		Logger logg;
		Tools::SuperResource<Bitmap> source;
		Tools::SuperResource<Font> source_font;

		auto atlas = source.create("ATLAS_GLOBAL");
		auto lmao = source.create("_test");

		auto foo = source_font.create("DEFAULT");

		// use .zip
		pather.apply();

		foo->load("font.ttf");

		if (!foo) logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "Failed to load font." << L::ELF;
		else {
			logg << L::SLF << fsr(__FUNCSIG__) << "Loaded font successfully." << L::ELF;
		}

		atlas->load("atlas0.png");
		bool gud = *atlas;

		if (!gud) logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "Failed to load atlas." << L::ELF;

		if (gud) {
			gud = lmao->create_sub(*atlas, 0, 1536, 512, 512);
			if (!gud) {
				logg << L::SLF << fsr(__FUNCSIG__, E::WARN) << "Failed to load sub bitmap from atlas." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(__FUNCSIG__) << "Loaded a bitmap from atlas SUCESSFULLY" << L::ELF;
			}
		}
		if (!gud) {
			lmao->create(300, 300);
			lmao->clear_to_color(Color(255, 255, 255));
			if (*lmao) {
				logg << L::SLF << fsr(__FUNCSIG__) << "Successfully created a bitmap to show." << L::ELF;
			}
			else {
				logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "Failed to create a bitmap." << L::ELF;
			}
		}

		pather.unapply();

		return lmao;
	});
	auto got = working_on.get().get<Tools::Resource<Bitmap>>();

	// moved config to top

	///auto vector_lol1 = conf->get_array<float>("config", "playercolor");
	///for (auto& i : vector_lol1) {
	//////logg << L::SLF << fsr(__FUNCSIG__) << "ARR B4: " << i << L::ELF;
	///}

	///conf->comment("config", "This is a new version of myself");
	///conf->set("config", "was_osd_on", false);
	///conf->set("config", "second_screen_debug", false);
	///conf->set("config", "ultradebug", false);
	///conf->set("config", "double_buffering_screen", true);
	///conf->set("config", "last_volume", 0.200218);
	///conf->set("config", "resolution_proportion", 2.000000);
	///conf->set("config", "fx_amount", 0.280000);
	///conf->set("config", "screen_width", 1920);
	///conf->set("config", "screen_height", 1080);
	///conf->set("config", "last_display_flags", 21);
	///conf->set("config", "pref_refresh_rate", 144);
	///conf->set("config", "times_open", 6);
	///conf->set("config", "playername", "Lohk");
	///conf->set("config", "last_version", "B202002113719");
	///conf->set("config", "playercolor", { 0.4023f, 0.5307f, 0.2331f });
	///conf->set("config", "hidemouse", true);
	///conf->set("config", "limit_framerate_to", 0);
	///conf->set("config", "prints_path", "%win_photos_path%/Lohk's Studios Screenshots/");
	///conf->set("private", config::config_section_mode::MEMORY_ONLY);
	///conf->set("private", "yo config", 10);
	///conf->flush();

	///auto vector_lol = conf->get_array<float>("config", "playercolor");
	///for (auto& i : vector_lol) {
	//////logg << L::SLF << fsr(__FUNCSIG__) << "ARR AFT: " << i << L::ELF;
	///}
	///logg << L::SLF << fsr(__FUNCSIG__) << "Only in memory: " << conf->get_as<int>("private", "yo config") << L::ELF;
	

	{
		Tools::SuperResource<Font> source_font;

		Tools::Resource<Interface::Font> lool;
		source_font.get("DEFAULT", lool);

		disp.add_draw_task([=](Camera&) {
			got->draw((Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, (Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, 0.3, 0.3);
			/*if (track->exists()) {
				float xx = 1.9f * ((1.0f * track->get_position_ms() / track->get_length_ms()) - 0.5f);

				got->draw(got->get_width() / 2, got->get_height() / 2, xx, 0.9f, 0.0003, 0.0003);
			}*/
			});

		disp.add_draw_task([lool, &disp, conf, logg](Camera& curr) {
			Camera cam2 = curr;
			const float prop = 2500.0f;
			cam2.classic_transform(0, 0, 1.0 / prop, 1.0 / prop, 0.0); // change this later somehow
			cam2.apply();
			lool->draw(-prop, -1.0f * prop, 0, "&5FPS: &a" + std::to_string(disp.get_frames_per_second()));
			lool->draw(-prop, -0.96f * prop, 0, "&2Times open: &b" + std::to_string(conf->get_as<unsigned long long>("config", "times_open")));
			lool->draw(-prop, -0.92f * prop, 0, "&7Mouse pos: &e" + std::to_string(conf->get_as<float>("local", "mouse_x")) + "x" + std::to_string(conf->get_as<float>("local", "mouse_y")));
			lool->draw(-prop, 0.96f * prop, 0, logg.get_last_line().filter_ascii_range());
			curr.apply();
			});
	}

	Tools::SuperResource<Camera> cameras;

	auto cam = cameras.create("DEFAULT");
	cam->classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);

	disp.set_camera(cam);

	Tools::SuperResource<Voice> voices;
	Tools::SuperResource<Mixer> mixers;
	Tools::SuperResource<Track> tracks;
	Tools::SuperResource<Sample> samples;

	auto main_voice = voices.create("MAIN_VOICE");
	auto main_mixer = mixers.create("MAIN_MIXER");

	main_voice->load();
	main_mixer->load();
	main_mixer->attach_to(main_voice);
	main_mixer->set_gain(conf->get_as<float>("config", "last_volume"));

	auto a_music = samples.create("ONE_SAMPLE");
	auto track = tracks.create("MY_TRACK");


	// music file inside zip
	pather.apply();

	if (a_music->load(music_test_inner_path)) {
		track->load(a_music);
		track->attach_to(main_mixer);
		track->set_play_mode(track::play_mode::MODE_LOOP);

		track->play();
		logg << L::SLF << fsr(__FUNCSIG__) << "Started background music and paused with playmode " << static_cast<int>(track->get_play_mode()) << L::ELF;
		track->pause();
	}
	else {
		logg << L::SLF << fsr(__FUNCSIG__, E::ERRR) << "Couldn't start track." << L::ELF;
	}

	// reset
	pather.unapply();


	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	int nowt = -1;
	disp.add_draw_task([&](Camera&) {
		if (auto lala = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()); lala - now > std::chrono::seconds(1)) {
			now = lala;
			if (track->exists()) {
				int ltr = 100.0 * track->get_position_ms() / track->get_length_ms();
				if (ltr != nowt) {
					nowt = ltr;
					logg << L::SLF << fsr(__FUNCSIG__) << nowt << "% | " << (track->get_position_ms() / 1e3) << " of " << (track->get_length_ms() / 1e3) << " s" << L::ELF;
				}
			}
		}
	});

	EventHandler fullscreen;
	fullscreen.add(get_keyboard_event());
	fullscreen.add(get_mouse_event());
	fullscreen.add(disp.get_event_source());
	fullscreen.set_run_autostart([&, conf](const RawEvent& ev) {
		switch (ev.type())
		{
		case ALLEGRO_EVENT_MOUSE_AXES:
			{
				float x, y;
				x = ev.mouse_event().x;
				y = ev.mouse_event().y;
				if (auto camnow = disp.get_current_camera(); camnow) {
					Camera inv = *camnow;
					inv.invert();
					inv.transform(x, y);
					//logg << L::SL << fsr(__FUNCSIG__) << "Mouse axis: [" << ev.mouse_event().x << ";" << ev.mouse_event().y << "] " << FormatAs("%.4f") << x << ";" << FormatAs("%.4f") << y << L::EL;

					conf->set("local", config::config_section_mode::MEMORY_ONLY);
					conf->set("local", "mouse_x", x);
					conf->set("local", "mouse_y", y);
				}
			}
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			if (ev.keyboard_event().keycode == ALLEGRO_KEY_F11) {
				logg << L::SL << fsr(__FUNCSIG__) << "Toggle Fullscreen called" << L::EL;
				disp.toggle_fullscreen();
								
				conf->set("config", "last_display_flags", disp.get_flags());
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_F) {
				logg << L::SL << fsr(__FUNCSIG__) << "Current FPS: &a" << disp.get_frames_per_second() << L::EL;
				logg << L::SL << fsr(__FUNCSIG__) << "&eOut of range errors: &7" << disp.debug_errors_out_of_range_skip() << L::EL;
				logg << L::SL << fsr(__FUNCSIG__) << "&eUnexpected errors: &7" << disp.debug_errors_unexpected() << L::EL;
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_P) {
				logg << L::SL << fsr(__FUNCSIG__) << "Play/Pause button called" << L::EL;
				if (track->exists()) {
					if (track->is_playing()) track->pause();
					else track->play();
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_R) {
				logg << L::SL << fsr(__FUNCSIG__) << "Reverse/Continuous button called" << L::EL;
				if (track->exists()) {
					track->set_speed(-track->get_speed());
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_MINUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "SlowDown button called" << L::EL;
				if (track->exists()) {
					if (fabs(track->get_speed()) > 0.1) track->set_speed(track->get_speed() > 0.0 ? track->get_speed() - 0.05 : track->get_speed() + 0.05);
					logg << L::SL << fsr(__FUNCSIG__) << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_EQUALS) {
				logg << L::SL << fsr(__FUNCSIG__) << "Accel button called" << L::EL;
				if (track->exists()) {
					track->set_speed(track->get_speed() > 0.0 ? track->get_speed() + 0.05 : track->get_speed() - 0.05);
					logg << L::SL << fsr(__FUNCSIG__) << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_PAD_MINUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "PAD- button called" << L::EL;
				float vol = main_mixer->get_gain();
				if (vol > 0.02) vol -= 0.02;
				main_mixer->set_gain(vol);
				conf->set("config", "last_volume", vol);
				logg << L::SL << fsr(__FUNCSIG__) << "(saved) Now volume = " << vol << "." << L::EL;
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_PAD_PLUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "PAD+ button called" << L::EL;
				float vol = main_mixer->get_gain();
				if (vol < 1.0) vol += 0.02;
				main_mixer->set_gain(vol);
				conf->set("config", "last_volume", vol);
				logg << L::SL << fsr(__FUNCSIG__) << "(saved) Now volume = " << vol << "." << L::EL;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			logg << L::SL << fsr(__FUNCSIG__) << "Close app called" << L::EL;
			if (track->exists()) track->stop();

			if (auto k = disp.get_width(); k) conf->set("config", "screen_width", k);
			if (auto k = disp.get_height(); k) conf->set("config", "screen_height", k);

			disp.set_stop();
			break;
		}
	});

	disp_fut.then<void>([&](bool good) {
		logg << L::SLF << fsr(__FUNCSIG__) << "Display closed with SUCCESS=" << good << L::ELF;
		logg.debug("disp_fut.then() called.");
	});

	disp_fut.wait();


	std::this_thread::sleep_for(std::chrono::seconds(1));

	logg.debug("DEBUG TEST LSW");

	return 0;
}