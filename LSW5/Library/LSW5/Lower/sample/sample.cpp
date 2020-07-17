#include "sample.h"

namespace LSW {
	namespace v5 {
		bool Sample::set(ALLEGRO_SAMPLE* s)
		{
			if (!s) return false;
			file_sample = std::shared_ptr<ALLEGRO_SAMPLE>(s, [](ALLEGRO_SAMPLE*& b) { if (al_is_system_installed() && b) { al_destroy_sample(b); b = nullptr; } });
			return !(!file_sample);
		}
		Sample::Sample()
		{
			lsw_init();
			lsw_init_acodec_addon();
			lsw_install_audio();
		}

		bool Sample::load(const std::string p)
		{
			return set(al_load_sample(p.c_str()));
		}

		Sample::operator bool() const
		{
			return !(!file_sample);
		}

		const bool Sample::operator!() const
		{
			return !file_sample;
		}

		const bool Sample::operator==(Sample& u) const
		{
			return file_sample == u.getInstance();
		}

		const bool Sample::operator==(ALLEGRO_SAMPLE*& u) const
		{
			return &(*file_sample) == u;
		}

		std::shared_ptr<ALLEGRO_SAMPLE> Sample::getInstance()
		{
			return file_sample;
		}

	}
}