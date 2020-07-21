#include "track.h"

namespace LSW {
	namespace v5 {

		Track::Track()
		{
			lsw_init();
			lsw_init_acodec_addon();
			lsw_install_audio();
		}

		Track::~Track()
		{
			SuperResource<ALLEGRO_SAMPLE_INSTANCE> instances;
			if (playing) instances.remove(playing);
		}

		bool Track::load(const std::shared_ptr<Sample> sample)
		{
			if (!playing) {
				SuperResource<ALLEGRO_SAMPLE_INSTANCE> instances;

				playing = instances.load("track_t&" + Tools::generateRandomUniqueStringN());

				al_set_sample_instance_playing(&(*playing), false);
			}

			if (sample) {
				return al_set_sample(&(*playing), &(*sample->getInstance()));
			}
			return false;
		}

		bool Track::load(const std::string str)
		{
			if (!playing) {
				SuperResource<ALLEGRO_SAMPLE_INSTANCE> instances;

				playing = instances.load("track_t&" + Tools::generateRandomUniqueStringN());

				al_set_sample_instance_playing(&(*playing), false);
			}
			SuperResource<Sample> samples;
			if (std::shared_ptr<Sample> sample; samples.get(str, sample)) {
				return al_set_sample(&(*playing), &(*sample->getInstance()));
			}
			return false;
		}

		bool Track::attach(const std::shared_ptr<Mixer> s)
		{
			return al_attach_sample_instance_to_mixer(&(*playing), &(*s->getInstance()));
		}

		bool Track::autoAttach()
		{
			SuperResource<Mixer> mixers;
			auto mmixer = mixers.getMain();
			if (mmixer) return attach(mmixer);
			return false;
		}

		void Track::twinUpAttributes(const std::shared_ptr<track_data> oth)
		{
			data_track = oth;
		}

		std::shared_ptr<Track::track_data> Track::getAttributes()
		{
			return data_track;
		}

		void Track::set(const track::e_boolean e, bool v)
		{
			if (auto* ptr = data_track->boolean_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Track::set(const track::e_double e, double v)
		{
			if (auto* ptr = data_track->double_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Track::set(const track::e_integer e, int v)
		{
			if (auto* ptr = data_track->integer_data(e); ptr)
				*ptr = [=] {return v; };
		}

		void Track::set(const std::string e, bool v)
		{
			auto* ptr = data_track->boolean_data(e.c_str(), e.length());
			if (!ptr) data_track->boolean_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Track::set(const std::string e, double v)
		{
			auto* ptr = data_track->double_data(e.c_str(), e.length());
			if (!ptr) data_track->double_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Track::set(const std::string e, int v)
		{
			auto* ptr = data_track->integer_data(e.c_str(), e.length());
			if (!ptr) data_track->integer_data.add({ [=] {return v; }, e.c_str(), e.length() });
			else *ptr = [=] {return v; };
		}

		void Track::set(const track::e_boolean e, std::function<bool(void)> v)
		{
			if (auto* ptr = data_track->boolean_data(e); ptr)
				*ptr = v;
		}

		void Track::set(const track::e_double e, std::function<double(void)> v)
		{
			if (auto* ptr = data_track->double_data(e); ptr)
				*ptr = v;
		}

		void Track::set(const track::e_integer e, std::function<int(void)> v)
		{
			if (auto* ptr = data_track->integer_data(e); ptr)
				*ptr = v;
		}

		void Track::set(const std::string e, std::function<bool(void)> v)
		{
			auto* ptr = data_track->boolean_data(e.c_str(), e.length());
			if (!ptr) data_track->boolean_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Track::set(const std::string e, std::function<double(void)> v)
		{
			auto* ptr = data_track->double_data(e.c_str(), e.length());
			if (!ptr) data_track->double_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		void Track::set(const std::string e, std::function<int(void)> v)
		{
			auto* ptr = data_track->integer_data(e.c_str(), e.length());
			if (!ptr) data_track->integer_data.add({ v, e.c_str(), e.length() });
			else *ptr = v;
		}

		bool Track::get(const track::e_boolean e, bool& v)
		{
			if (auto* ptr = data_track->boolean_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const track::e_double e, double& v)
		{
			if (auto* ptr = data_track->double_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const track::e_integer e, int& v)
		{
			if (auto* ptr = data_track->integer_data[e]; ptr)
			{
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, bool& v)
		{
			if (auto* ptr = data_track->boolean_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, double& v)
		{
			if (auto* ptr = data_track->double_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, int& v)
		{
			if (auto* ptr = data_track->integer_data(e.c_str(), e.length()); ptr) {
				v = (*ptr)();
				return true;
			}
			return false;
		}

		bool Track::get(const track::e_boolean e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_track->boolean_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Track::get(const track::e_double e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_track->double_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Track::get(const track::e_integer e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_track->integer_data[e]; ptr)
			{
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, std::function<bool(void)>& v)
		{
			if (auto* ptr = data_track->boolean_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, std::function<double(void)>& v)
		{
			if (auto* ptr = data_track->double_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		bool Track::get(const std::string e, std::function<int(void)>& v)
		{
			if (auto* ptr = data_track->integer_data(e.c_str(), e.length()); ptr) {
				v = *ptr;
				return true;
			}
			return false;
		}

		std::function<bool(void)>* Track::getDirect(const track::e_boolean e)
		{
			if (auto* ptr = data_track->boolean_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<double(void)>* Track::getDirect(const track::e_double e)
		{
			if (auto* ptr = data_track->double_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::function<int(void)>* Track::getDirect(const track::e_integer e)
		{
			if (auto* ptr = data_track->integer_data(e); ptr)
				return ptr;
			return nullptr;
		}

		std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> Track::getInstance()
		{
			return playing;
		}

		void Track::update()
		{
			/*			
			enum class e_boolean {PLAYING};
			enum class e_double {VOLUME, SPEED};
			enum class e_integer {PLAY_MODE};
			*/

			if (playing) {

				const auto play = (*data_track->boolean_data[track::e_boolean::PLAYING])();
				const auto vol = (*data_track->double_data[track::e_double::VOLUME])();
				const auto speed = (*data_track->double_data[track::e_double::SPEED])();
				const auto pmode = (*data_track->integer_data[track::e_integer::PLAY_MODE])();

				al_set_sample_instance_playing(&(*playing), play);
				al_set_sample_instance_gain(&(*playing), static_cast<float>(vol));
				al_set_sample_instance_speed(&(*playing), static_cast<float>(speed));
				al_set_sample_instance_playmode(&(*playing), static_cast<ALLEGRO_PLAYMODE>(pmode));
			}
		}

	}
}