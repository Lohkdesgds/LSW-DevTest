#include "display.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			template<size_t u>
			inline void DisplayT<u>::targ_set(const Bitmap& b)
			{
				_targ.set(b);
			}

			template<size_t u>
			inline void DisplayT<u>::targ_set(const std::function<Bitmap(void)>& f)
			{
				_targ.set(f);
			}

			template<size_t u>
			inline void DisplayT<u>::targ_apply()
			{
				_targ.apply();
			}

			template<size_t u>
			inline Bitmap DisplayT<u>::targ_get()
			{
				return _targ.get();
			}

			template<size_t u>
			inline bool operator==(ALLEGRO_DISPLAY* a, const DisplayT<u>& b)
			{
				return b.operator==(a);
			}

			template<size_t u>
			inline bool operator!=(ALLEGRO_DISPLAY* a, const DisplayT<u>& b)
			{
				return !b.operator==(a);
			}

			template<size_t u>
			inline Tools::Resource<Display> generate_display()
			{
				return std::shared_ptr<Display>((Display*)new DisplayT<u>(), [](Display* d) {delete (DisplayT<u>*)d; });
			}

		}
	}
}