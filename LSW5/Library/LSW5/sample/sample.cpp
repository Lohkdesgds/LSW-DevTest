#include "sample.h"

namespace LSW {
	namespace v5 {

		Sample::Sample()
		{
			lsw_init();
			lsw_init_acodec_addon();
			lsw_install_audio();
		}

		Sample::~Sample()
		{
			SuperResource<ALLEGRO_SAMPLE> file_samples;
			if (file_sample) file_samples.remove(file_sample);
		}

		void Sample::load(const std::string path)
		{
			SuperResource<ALLEGRO_SAMPLE> file_samples;
			file_sample = file_samples.load("sample_s&" + Tools::generateRandomUniqueStringN(), path);
		}

		bool Sample::isLoaded()
		{
			if (file_sample) return true; // i know right
			return false;
		}

		std::shared_ptr<ALLEGRO_SAMPLE> Sample::getInstance()
		{
			return file_sample;
		}

	}
}