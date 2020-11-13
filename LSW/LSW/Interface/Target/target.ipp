#pragma once

namespace LSW {
	namespace v5 {
		namespace Interface {

			template<size_t index> Tools::FastFunction<Bitmap> Target<index>::func;

			template<size_t index>
			inline void Target<index>::set(const Bitmap& c)
			{
				func = c;
			}

			template<size_t index>
			inline void Target<index>::set(const std::function<Bitmap(void)>& f)
			{
				func = f;
			}

			template<size_t index>
			inline void Target<index>::apply()
			{
				func().set_as_target();
			}

			template<size_t index>
			inline Bitmap Target<index>::get()
			{
				return func();
			}

		}
	}
}