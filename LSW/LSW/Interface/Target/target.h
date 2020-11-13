#pragma once

#include "../../Handling/Abort/abort.h"
#include "../../Handling/Initialize/initialize.h"
#include "../../Tools/FastFunction/fastfunction.h"
#include "../Bitmap/bitmap.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			class Target {
				static std::unordered_map<size_t, Tools::FastFunction<Bitmap>> func;
				const size_t i;
			public:
				Target(const size_t);

				void set(const Bitmap&);
				void set(const std::function<Bitmap(void)>&);
				void apply();
				Bitmap get();
			};			

		}
	}
}