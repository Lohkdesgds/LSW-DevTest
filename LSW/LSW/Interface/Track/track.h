#pragma once

// C++
#include <memory>
#include <string>
#include <functional>

// Others
#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"
#include "..\Mixer\mixer.h"
#include "..\Sample\sample.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace track {
				
				enum class play_mode {
					MODE_ONCE = ALLEGRO_PLAYMODE_ONCE,
					MODE_LOOP = ALLEGRO_PLAYMODE_LOOP,
					MODE_BIDIR = ALLEGRO_PLAYMODE_BIDIR
				};
			}

			class Track {
				std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> playing;
				unsigned paused_at = 0;
				bool set(ALLEGRO_SAMPLE_INSTANCE*);
			public:
				Track();

				// id, sample id to load
				bool load(const std::shared_ptr<Sample>);

				// attaches to a Mixer
				bool attach_to(const std::shared_ptr<Mixer>);

				bool exists() const;

				/* ALLEGRO STUFF */

				void play();
				void pause();
				void stop();
				bool is_playing() const;

				ALLEGRO_CHANNEL_CONF get_channel_configuration() const;
				size_t get_channels_num() const;
				ALLEGRO_AUDIO_DEPTH get_depth() const;
				unsigned get_frequency() const;

				unsigned get_length_samples() const;
				void set_length_samples(const unsigned);
				size_t get_length_ms() const;
				void set_length_ms(const size_t);

				unsigned get_position_samples() const;
				void set_position_samples(const unsigned);
				size_t get_position_ms() const;
				void set_position_ms(const size_t);

				float get_speed() const;
				void set_speed(const float);

				float get_gain() const;
				void set_gain(const float);

				float get_pan() const;
				void set_pan(const float);

				float get_time_s() const;

				track::play_mode get_play_mode() const;
				void set_play_mode(const track::play_mode);


			};
		}
	}
}