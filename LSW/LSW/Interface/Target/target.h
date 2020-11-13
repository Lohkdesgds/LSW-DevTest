#pragma once

#include "../../Handling/Abort/abort.h"
#include "../../Handling/Initialize/initialize.h"
#include "../../Tools/FastFunction/fastfunction.h"
#include "../Bitmap/bitmap.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			/// <summary>
			/// <para>This you can use on classes that use Target, then you just need to override the functions in the template version with the differences.</para>
			/// <para>This must be a clone of Target (but with targ_ prefix), but not a template! This way we can link stuff in a more efficient way!</para>
			/// </summary>
			class TargetInterface {
			public:
				virtual void targ_set(const Bitmap&) = 0;
				virtual void targ_set(const std::function<Bitmap(void)>&) = 0;
				virtual void targ_apply() = 0;
				virtual Bitmap targ_get() = 0;
			};			
			

			template <size_t index>
			class Target {
				static Tools::FastFunction<Bitmap> func;
			public:				
				void set(const Bitmap&);
				void set(const std::function<Bitmap(void)>&);
				void apply();
				Bitmap get();
			};			

		}
	}
}

#include "target.ipp"