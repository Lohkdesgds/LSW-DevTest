#pragma once

#include <any>
#include <string>

namespace LSW {
	namespace v5 {
		namespace Tools {

			/// <summary>
			/// <para>Custom std::any that works with char arrays (transforming them to std::string)</para>
			/// <para>operator= and get template changed if T is char* or const char*</para>
			/// </summary>
			class Any : public std::any {
				std::string if_string_buf;
			public:
				using std::any::operator=;

				/// <summary>
				/// <para>Somehow just setting to use any's directly fail.</para>
				/// </summary>
				/// <param name="{args}">Arguments to init a std::any.</param>
				template<typename... Args> Any(Args...);

				/// <summary>
				/// <para>Set a string as variable (internally  it will be saved as std::string).</para>
				/// </summary>
				/// <param name="{char*}">String.</param>
				/// <returns>{Any&} itself</returns>
				Any& operator=(char*);

				/// <summary>
				/// <para>Set a string as variable (internally  it will be saved as std::string).</para>
				/// </summary>
				/// <param name="{char*}">String.</param>
				/// <returns>{Any&} itself</returns>
				Any& operator=(const char*);

				/// <summary>
				/// <para>Get value directly.</para>
				/// </summary>
				/// <returns>{T} The value, if valid.</returns>
				template<typename T> T get();

				/// <summary>
				/// <para>Get value directly.</para>
				/// </summary>
				/// <returns>{char*} The value, if valid.</returns>
				template<> char* get();

				/// <summary>
				/// <para>Get value directly.</para>
				/// </summary>
				/// <returns>{char*} The value, if valid.</returns>
				template<> const char* get();

				/// <summary>
				/// <para>Get value directly.</para>
				/// </summary>
				/// <returns>{char*} The value, if valid.</returns>
				template<typename T> T get() const;

				template<> char* get() const = delete;
				template<> const char* get() const = delete;
			};

		}
	}
}

#include "any.ipp"