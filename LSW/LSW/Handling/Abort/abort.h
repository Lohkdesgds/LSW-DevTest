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

#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <iostream>

namespace LSW {
	namespace v5 {
		namespace Handling {

			namespace abort {
				enum class abort_level { GIVEUP, FATAL_ERROR, OTHER };
			}

			class Abort : public std::exception {
				std::string					s_from_where,
					s_details;
				abort::abort_level			abort_level = abort::abort_level::FATAL_ERROR;
			public:
				Abort() = delete;
				/**
				 * Custom exception that provides more information.
				 * @constructor
				 * @param {std::string} From where (use something like __FUNCSIG__).
				 * @param {std::string} Details about the issue.
				 * @param {abort::abort_level} GIVEUP is a throw that can continue, FATAL_ERROR can't, OTHER can be your custom one.
				 */
				Abort(std::string, std::string, abort::abort_level = abort::abort_level::FATAL_ERROR);
				/**
				 * Copy Abort constructor.
				 * @constructor
				 * @param {Abort} Another Abort to copy.
				 */
				Abort(const Abort&);
				/**
				 * Gets the FROM variable (from call).
				 * @returns from where it was called (probably something like __FUNCSIG__).
				 */
				const std::string& get_from() const;
				/**
				 * Gets the DETAILS variable (from call).
				 * @returns details about the issue.
				 */
				const std::string& get_details() const;
				/**
				 * Gets the LEVEL variable (from call).
				 * @returns abort::abort_level level.
				 */
				const abort::abort_level& get_level() const;

				/**
				 * Automatic transform to a string
				 * @returns std::string combining all info
				 */
				operator std::string() const;
			};


			class AutoAbort {
				std::chrono::milliseconds t;
				std::thread thr;
				std::function<void(void)> autodrop, call_death;
				bool drop = false;
			public:
				/**
				 * An AUTOABORT class that will do exception if abort() is not called after some milliseconds.
				 * @constructor
				 * @param {unsigned} Time in milliseconds to cancel everything.
				 * @param {std::functional} A functional to call if abort() is not called in time.
				 */
				AutoAbort(const unsigned, std::function<void(void)> = std::function<void(void)>());

				/**
				 * Aborts the autoabort.
				 */
				void abort();
			};


			std::string operator+(const Abort&);
		}
	}
}