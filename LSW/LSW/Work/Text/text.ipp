#pragma once


namespace LSW {
	namespace v5 {
		namespace Work {
			
			template<size_t u>
			inline void TextT<u>::targ_set(const Interface::Bitmap& b)
			{
				_targ.set(b);
			}

			template<size_t u>
			inline void TextT<u>::targ_set(const std::function< Interface::Bitmap(void)>& f)
			{
				_targ.set(f);
			}

			template<size_t u>
			inline void TextT<u>::targ_apply()
			{
				_targ.apply();
			}

			template<size_t u>
			inline  Interface::Bitmap TextT<u>::targ_get()
			{
				return _targ.get();
			}

			template<size_t u>
			inline Tools::Resource<Text> generate_block()
			{
				return std::shared_ptr<Text>((Text*)new TextT<u>(), [](Text* d) {delete (TextT<u>*)d; });
			}

		}
	}
}