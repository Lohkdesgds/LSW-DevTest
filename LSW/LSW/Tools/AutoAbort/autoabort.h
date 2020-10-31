#pragma once

#include <Windows.h>

#include "../../Handling/Abort/abort.h"
#include "../SuperThread/superthread.h"


namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace autoabort {
				enum class abort_mode { ASYNC_TIMEOUT_RUN_AND_EXIT, ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN, ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF };
			}

			/// <summary>
			/// <para>AutoAbort is an automatic abort system you can use if you have something that can take "too long" and you don't want it to hold the app forever.</para>
			/// <para>There are 3 modes:</para>
			/// <para>———————————————————————————————————————————————————————</para>
			/// <para>1. ASYNC_TIMEOUT_RUN_AND_EXIT: if timeout, it runs the function and then it kills THE APP.</para>
			/// <para>2. ASYNC_TIMEOUT_KILLTHISTHREADSELF_THEN_RUN: if timeout, it kills the thread that created the object and then it runs the function.</para>
			/// <para>3. ASYNC_TIMEOUT_RUN_THEN_KILLTHISTHREADSELF: if timeout, it runs the function and then it kills the thread that created the object.</para>
			/// <para>PS: (Case 2 and 3) If it's a std::thread not detached, it will still be possible to join() aka joinable().</para>
			/// </summary>
			class AutoAbort {
				std::function<void(void)> multif;
				SuperThread running;
				bool* abort_cancel = nullptr;
			public:
				/// <summary>
				/// <para>Creates automatic abort and starts timeout timer.</para>
				/// </summary>
				/// <param name="{autoabort::abort_mode} The mode that the AutoAbort will follow"></param>
				/// <param name="{size_t} Timeout, in milliseconds"></param>
				/// <param name="{std::function} The multifunctional function (it depends on the mode)"></param>
				AutoAbort(const autoabort::abort_mode, const size_t, std::function<void(void)>);
				~AutoAbort();

				/// <summary>
				/// <para>Aborts the autoabort (if it didn't autoabort itself yet).</para>
				/// </summary>
				void abort();
			};

		}
	}
}