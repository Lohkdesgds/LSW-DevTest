//
// abort.h
// *******
//
// Abort is used as an option to std::exception.
// - Includes AutoAbort (automatic timed abort).
// 
// Abort can be static_cast'ed or +variable to generate a std::string
//
// Revision by LSW
// 

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <iostream>
#include <Windows.h>

namespace LSW {
	namespace v5 {
		namespace Handling {

			namespace abort {
				enum class abort_level { GIVEUP, FATAL_ERROR, OTHER };
			}

			/// <summary>
			/// <para>Abort is a exception handling class.</para>
			/// <para>All errors thrown in ths library should be Abort. You can get it as std::exception, but it only has what()...</para>
			/// <para>Abort give more information about where, what exactly and how bad is the error.</para>
			/// </summary>
			class Abort : public std::exception {
				std::string					s_from_where,
					s_details;
				abort::abort_level			abort_level = abort::abort_level::FATAL_ERROR;
			public:
				Abort() = delete;

				/// <summary>
				/// <para>Default constructor for a throw scenario.</para>
				/// </summary>
				/// <param name="{std::string}">Normally filled with __FUNCSIG__, the WHERE.</param>
				/// <param name="{std::string}">What happened? This is the description part.</param>
				/// <param name="{abort::abort_level}">Is it impossible to continue from here (FATAL_ERROR)? Or is it just a "get me out of here" (GIVEUP)?</param>
				Abort(const std::string&, const std::string&, const abort::abort_level = abort::abort_level::FATAL_ERROR);

				/// <summary>
				/// <para>Copy constructor.</para>
				/// <param name="{Abort}">Abort to copy</param>
				/// </summary>
				Abort(const Abort&);

				/// <summary>
				/// <para>Gets from where the exception has been thrown.</para>
				/// </summary>
				/// <returns>{std::string} Probably __FUNCSIG__ format.</returns>
				const std::string& get_from() const;

				/// <summary>
				/// <para>Gets the details about the throw.</para>
				/// </summary>
				/// <returns>{std::string} Details about the throw.</returns>
				const std::string& get_details() const;

				/// <summary>
				/// <para>Gets the level of the issue.</para>
				/// </summary>
				/// <returns>{abort::abort_level} The level of the issue.</returns>
				const abort::abort_level& get_level() const;

				/// <summary>
				/// <para>If you just want a simple string with all information, use this.</para>
				/// <para>Format: "From: FROM;\nDetails: DETAILS;\nLevel: LEVEL_AS_STRING;"</para>
				/// </summary>
				/// <returns>{std::string} Itself as one string with all information.</returns>
				operator std::string() const;
			};



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
				std::thread* running = nullptr;
				bool* abort_cancel = nullptr;
			public:
				/// <summary>
				/// <para>Creates and starts timeout timer.</para>
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


			std::string operator+(const Abort&);
		}
	}
}