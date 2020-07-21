#pragma once

// C
#define _CRT_RAND_S
// C++
#include <numeric>
#include <functional>
// Others
#include "..\lower\bitmap\bitmap.h"
#include "..\sprite\sprite.h"
#include "..\superresource\superresource.h"
#include "..\database\database.h"
#include "..\tools\tools.h"
#include "..\shared\shared.h"

#undef max


namespace LSW {
	namespace v5 {


		namespace block {

			enum class e_chronomillis_readonly { LAST_FRAME, LAST_TIE_FRAME_VERIFICATION };
			enum class e_integer { FRAME };
			enum class e_boolean { SET_FRAME_VALUE_READONLY };
			enum class e_double { FRAMES_PER_SECOND, TIE_SIZE_TO_DISPLAY_PROPORTION };

			const SuperMap<std::function<std::chrono::milliseconds(void)>>		e_chronomillis_readonly_defaults = {
				{[] {return MILLI_NOW;},																						(e_chronomillis_readonly::LAST_FRAME),					CHAR_INIT("last_frame")},
				{[]{return MILLI_NOW;},																							(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	CHAR_INIT("last_tie_verification")}
			};
			const SuperMap<std::function<int(void)>>		e_integer_defaults = {
				{[] {return 0; },																								(e_integer::FRAME),										CHAR_INIT("frame")}
			};
			const SuperMap<std::function<bool(void)>>		e_boolean_defaults = {
				{[] {return false; },																							(e_boolean::SET_FRAME_VALUE_READONLY),					CHAR_INIT("set_frame_readonly")}
			};
			const SuperMap<std::function<double(void)>>		e_double_defaults = {
				{[] {return 10.0; },																							(e_double::FRAMES_PER_SECOND),							CHAR_INIT("frames_per_second")},
				{[] {return 0.0; },																								(e_double::TIE_SIZE_TO_DISPLAY_PROPORTION),				CHAR_INIT("tie_size_to_display_proportion")}
			};

			constexpr size_t maximum_bitmap_amount = static_cast<size_t>(std::numeric_limits<int>::max());
			constexpr size_t static_frame = static_cast<size_t>(-1); // static frame X?
			constexpr auto default_delta_t_frame_delay = std::chrono::seconds(5); // don't need to be fast (see also bubblefx::...)
		}

		class Block : public Sprite_Base, public Workaround<std::chrono::milliseconds> { // Sprite_Base already has others included!!!!!!!
			struct _bitmap {
				std::shared_ptr<Bitmap> ref;
				std::string source, id;
				bool is_sub_bitmap = false; // if true, source is source's id, else source is path
			};

			struct block_data : Sprite_Base::sprite_base_data {
				std::shared_ptr<SuperMap<std::function<std::chrono::milliseconds(void)>>>	chronomillis_readonly_data;
			};

			//std::shared_ptr<block_data> data_block = std::make_shared<block_data>();

			std::vector<_bitmap> bitmaps;


			//Bitmap* bmp = nullptr;

			std::shared_ptr<Bitmap> reference;
			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getDirect;
			using Workaround<std::chrono::milliseconds>::set;
			using Workaround<std::chrono::milliseconds>::get;
			using Workaround<std::chrono::milliseconds>::getDirect;

			Block();
			Block(Block&);
			//~Block(); // bitmaps are handled by SuperResource, there might be some other people around using the same resource


			// set to blindly use this comrade's attributes as theirs (this must be Block)
			void twinUpAttributes(const std::shared_ptr<block_data>);
			// actually copy a strong "reference" to its data somewhere else (share)
			std::shared_ptr<block_data> getAttributes();


			// from what id, path (can be "null" if you know the file is indeed loaded as id)
			void load(const std::string, const std::string = "");
			// assumes that you want main texture (see SuperResource.setMain())
			void load();
			// from what id copy, x, y, w, h
			void loadCut(const std::string, const int, const int, const int, const int);
			// assumes that you want main texture (see SuperResource.setMain()), x, y, w, h
			void loadCut(const int, const int, const int, const int);
			// removes from its list all matches
			void remove(const std::string);
		};


	}
}