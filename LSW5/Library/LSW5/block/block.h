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

			const SuperMap<std::chrono::milliseconds>		e_chronomillis_readonly_defaults = {
				{MILLI_NOW,																										(e_chronomillis_readonly::LAST_FRAME),					CHAR_INIT("last_frame")},
				{MILLI_NOW,																										(e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION),	CHAR_INIT("last_tie_verification")}
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

		class Block : public Sprite_Base {
			struct _bitmap {
				std::shared_ptr<Bitmap> ref;
				std::string source, id;
				bool is_sub_bitmap = false; // if true, source is source's id, else source is path
			};

			struct block_data {
				SuperMap<std::function<int(void)>>			integer_data = block::e_integer_defaults;
				SuperMap<std::function<double(void)>>		double_data = block::e_double_defaults;
				SuperMap<std::function<bool(void)>>			boolean_data = block::e_boolean_defaults;
				SuperMap<std::chrono::milliseconds>			chronomillis_readonly_data = block::e_chronomillis_readonly_defaults;
			};

			std::shared_ptr<block_data> data_block = std::make_shared<block_data>();

			std::vector<_bitmap> bitmaps;


			//Bitmap* bmp = nullptr;

			std::shared_ptr<Bitmap> reference;
			void draw_self();
		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getRef;

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

			void set(const block::e_integer, int);
			void set(const block::e_double, double);
			void set(const block::e_boolean, bool);
			void set(const block::e_integer, std::function<int(void)>);
			void set(const block::e_double, std::function<double(void)>);
			void set(const block::e_boolean, std::function<bool(void)>);
			void set(const std::string, int);
			void set(const std::string, double);
			void set(const std::string, bool);
			void set(const std::string, std::function<int(void)>);
			void set(const std::string, std::function<double(void)>);
			void set(const std::string, std::function<bool(void)>);

			bool get(const block::e_integer, int&);
			bool get(const block::e_double, double&);
			bool get(const block::e_boolean, bool&);
			bool get(const block::e_integer, std::function<int(void)>&);
			bool get(const block::e_double, std::function<double(void)>&);
			bool get(const block::e_boolean, std::function<bool(void)>&);
			bool get(const std::string, int&);
			bool get(const std::string, double&);
			bool get(const std::string, bool&);
			bool get(const std::string, std::function<int(void)>&);
			bool get(const std::string, std::function<double(void)>&);
			bool get(const std::string, std::function<bool(void)>&);

			bool get(const block::e_chronomillis_readonly, std::chrono::milliseconds&);

			std::function<int(void)>* getRef(const block::e_integer);
			std::function<double(void)>* getRef(const block::e_double);
			std::function<bool(void)>* getRef(const block::e_boolean);
			const std::chrono::milliseconds* getRef(const block::e_chronomillis_readonly) const;
		};


	}
}