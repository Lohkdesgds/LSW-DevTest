#pragma once

#include <functional>
#include <mutex>

namespace LSW {
	namespace v5 {
		namespace Tools {
			


			/// <summary>
			/// <para>There are many places where you want to support dynamic function-based values, but also be fast if you don't want complicated stuff.</para>
			/// <para>This allows easy almost atomic (it depends on T) safe (functional side) store.</para>
			/// <para>If you set a value, it won't use mutex. If function is set, it will. If you try to change the function, it switches to var-based, set, then go back, for performance.</para>
			/// </summary>
			template<typename T>
			class FastFunction {
				std::function<T(void)> gen;
				T ref{};
				mutable std::mutex gen_use;
				bool using_gen = false;
			public:
				FastFunction() = default;

				/// <summary>
				/// <para>Copy constructor.</para>
				/// </summary>
				/// <param name="{FastFunction}">Copy.</param>
				FastFunction(const FastFunction&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// </summary>
				/// <param name="{FastFunction}">Move.</param>
				FastFunction(FastFunction&&);

				/// <summary>
				/// <para>Direct set.</para>
				/// </summary>
				/// <param name="{T}">Fixed variable.</param>
				FastFunction(const T&);

				/// <summary>
				/// <para>Dynamic function set.</para>
				/// </summary>
				/// <param name="{std::function}">Generator.</param>
				FastFunction(const std::function<T(void)>&);

				/// <summary>
				/// <para>Copy operator.</para>
				/// </summary>
				/// <param name="{FastFunction}">Copy.</param>
				void operator=(const FastFunction&);

				/// <summary>
				/// <para>Move operator.</para>
				/// </summary>
				/// <param name="{FastFunction}">Move.</param>
				void operator=(FastFunction&&);

				/// <summary>
				/// <para>Direct set.</para>
				/// </summary>
				/// <param name="{T}">Fixed variable.</param>
				void set(const T&);

				/// <summary>
				/// <para>Dynamic function set.</para>
				/// </summary>
				/// <param name="{std::function}">Generator.</param>
				void set(const std::function<T(void)>&);

				/// <summary>
				/// <para>Function-like get (same as get()).</para>
				/// </summary>
				/// <returns>{T} The actual value.</returns>
				T operator()();

				/// <summary>
				/// <para>Get the value (if function, call function and return, else variable directly).</para>
				/// </summary>
				/// <returns>{T} The actual value.</returns>
				T get();

				/// <summary>
				/// <para>Get the function itself (can be empty or not up to date if direct).</para>
				/// </summary>
				/// <returns>{std::function} Last function seen.</returns>
				std::function<T(void)> get_f() const;
			};




			template<typename T>
			inline FastFunction<T>::FastFunction(const FastFunction& o)
			{
				*this = o;
			}

			template<typename T>
			inline FastFunction<T>::FastFunction(FastFunction&& o)
			{
				*this = std::move(o);
			}

			template<typename T>
			inline FastFunction<T>::FastFunction(const T& val)
			{
				set(val);
			}

			template<typename T>
			inline FastFunction<T>::FastFunction(const std::function<T(void)>& ff)
			{
				set(ff);
			}

			template<typename T>
			inline void FastFunction<T>::operator=(const FastFunction& o)
			{
				std::lock_guard<std::mutex> l1(gen_use);
				std::lock_guard<std::mutex> l2(o.gen_use);
				gen = o.gen;
				ref = o.ref;
				using_gen = o.using_gen;
			}

			template<typename T>
			inline void FastFunction<T>::operator=(FastFunction&& o)
			{
				std::lock_guard<std::mutex> l1(gen_use);
				std::lock_guard<std::mutex> l2(o.gen_use);
				gen = std::move(o.gen);
				ref = o.ref;
				using_gen = o.using_gen;
				o.using_gen = false;
			}

			template<typename T>
			inline void FastFunction<T>::set(const T& t)
			{
				using_gen = false;
				ref = t;
			}

			template<typename T>
			inline void FastFunction<T>::set(const std::function<T(void)>& f)
			{
				using_gen = false;
				{
					std::lock_guard<std::mutex> l(gen_use);
					gen = f;
				}
				using_gen = true;
			}

			template<typename T>
			inline T FastFunction<T>::operator()()
			{
				return get();
			}

			template<typename T>
			inline T FastFunction<T>::get()
			{
				if (using_gen) {
					std::lock_guard<std::mutex> l(gen_use);
					ref = gen();
					return ref;
				}
				return ref;
			}

			template<typename T>
			inline std::function<T(void)> FastFunction<T>::get_f() const
			{
				std::lock_guard<std::mutex> l(gen_use);
				return gen;
			}

		}
	}
}
