#pragma once

namespace LSW {
	namespace v5 {
		namespace Work {
			
			template<size_t u>
			inline void BlockT<u>::targ_set(const Interface::Bitmap& b)
			{
				_targ.set(b);
			}

			template<size_t u>
			inline void BlockT<u>::targ_set(const std::function< Interface::Bitmap(void)>& f)
			{
				_targ.set(f);
			}

			template<size_t u>
			inline void BlockT<u>::targ_apply()
			{
				_targ.apply();
			}

			template<size_t u>
			inline  Interface::Bitmap BlockT<u>::targ_get()
			{
				return _targ.get();
			}

			template<size_t u>
			inline Tools::Resource<Block> generate_block()
			{
				return std::shared_ptr<Block>((Block*)new BlockT<u>(), [](Block* d) {delete (BlockT<u>*)d; });
			}

		}
	}
}