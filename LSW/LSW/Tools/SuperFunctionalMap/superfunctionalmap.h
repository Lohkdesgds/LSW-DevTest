#pragma once

#include <string>
#include <functional>
#include <type_traits>

#include "../SuperMap/supermap.h"
#include "../Common/common.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace base {
				enum class e_base { BASE, BASE_FUNC }; // virtual use to know who it is
			}

			/// <summary>
			/// <para>This is a internal feature. You can use it, but it's not recommended.</para>
			/// </summary>
			template<typename T>
			class sfm_safe_feature {
				std::function<T(void)> gen;
				T ref{};
				mutable std::mutex gen_use;
				bool using_gen = false;
			public:
				sfm_safe_feature() = default;
				sfm_safe_feature(const sfm_safe_feature&);
				sfm_safe_feature(sfm_safe_feature&&);
				sfm_safe_feature(const T&);
				sfm_safe_feature(const std::function<T(void)>&);

				void operator=(const sfm_safe_feature&);
				void operator=(sfm_safe_feature&&);

				void set(const T&);
				void set(const std::function<T(void)>&);

				T operator()();

				T get();
				std::function<T(void)> get_f() const;
			};

			/// <summary>
			/// <para>SuperFunctionalMap is a SuperMap, but aimed to handle with functions instead of values themselves. It can be transformed to a SuperMap and back to SuperFunctionalMap (if you're using a shared_ptr of SuperMap).</para>
			/// <para>This exists so you can directly get the value directly or function in a easier way. You can set a value as a function so the value changes as time goes by.</para>
			/// <para>———————————————————————————————————————————————————————</para>
			/// <para># NOTES:</para>
			/// <para>1. Internally this is a SuperMap of a sfm_safe_feature (function/var class). It has a shared_ptr of that. You can get back and set a new SuperMap of function as you please.</para>
			/// <para>2. get_direct&lt;T&gt; returns the value (not function, the value itself), while get_direct&lt;T*&gt; returns a pointer to the function (not value, the REAL function!)</para>
			/// </summary>
			template<typename BaseType/*, typename TransformedToFuncType = std::function<BaseType(void)>*/>
			class SuperFunctionalMap {
			protected:
				std::shared_ptr<SuperMap<sfm_safe_feature<BaseType>>> map = std::make_shared<SuperMap<sfm_safe_feature<BaseType>>>();
			public:
				SuperFunctionalMap() = default;

				/// <summary>
				/// <para>Copy constructor, but from a similar SuperMap one.</para>
				/// </summary>
				/// <param name="{SuperMap}">A SuperMap of function.</param>
				/// <returns></returns>
				SuperFunctionalMap(const SuperMap<sfm_safe_feature<BaseType>>& olmap) {
					(*map) = olmap;
				}

				/// <summary>
				/// <para>Gets the shared_ptr of the SuperMap inside this object.</para>
				/// </summary>
				/// <returns>{SuperMap} Shared_ptr of SuperMap.</returns>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					std::shared_ptr<SuperMap<sfm_safe_feature<BaseType>>> get() {
					return map;
				}

				/// <summary>
				/// <para>Gets the shared_ptr of the SuperMap inside this object.</para>
				/// </summary>
				/// <returns>{SuperMap} Shared_ptr of SuperMap.</returns>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					std::shared_ptr<SuperMap<sfm_safe_feature<BaseType>>> get() const {
					return map;
				}

				/// <summary>
				/// <para>Adds to the SuperMap internally.</para>
				/// </summary>
				/// <param name="{SuperMap}">Another SuperMap with other SuperPairs.</param>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(const SuperMap<sfm_safe_feature<BaseType>>& mp) {
					map->add(mp);
				}

				/// <summary>
				/// <para>Moves to the SuperMap internally.</para>
				/// </summary>
				/// <param name="{SuperMap}">Another SuperMap with other SuperPairs.</param>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(SuperMap<sfm_safe_feature<BaseType>>&& mp) {
					map->add(std::move(mp));
				}

				/// <summary>
				/// <para>Sets the internal shared_ptr to another SuperMap.</para>
				/// </summary>
				/// <param name="{shared_ptr}">The other SuperMap this should be referencing to.</param>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(const std::shared_ptr<SuperMap<sfm_safe_feature<BaseType>>>& mp) {
					map = mp;
				}

				/// <summary>
				/// <para>Move the internal shared_ptr from another SuperMap.</para>
				/// </summary>
				/// <param name="{shared_ptr}">The other SuperMap this should be cutting from.</param>
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(std::shared_ptr<SuperMap<sfm_safe_feature<BaseType>>>&& mp) {
					map = std::move(mp);
				}

				/// <summary>
				/// <para>Adds a Key to return the Value (automatically creates a function that returns this value).</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <param name="{Arg2}">The value this key would return.</param>
				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(const Arg1& arg1, Key arg2) {
					auto* ptr = map->get_pair(arg1);
					if (!ptr) map->add({ sfm_safe_feature<BaseType>(arg2), arg1 });
					else ptr->get_value().set(arg2);
				}

				/// <summary>
				/// <para>Adds a Key to return the Function.</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <param name="{Arg2}">The function this key would return.</param>
				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(const Arg1& arg1, std::function<Key(void)> arg2) {
					auto* ptr = map->get_pair(arg1);
					if (!ptr) map->add({ sfm_safe_feature<BaseType>(arg2), arg1 });
					else ptr->get_value().set(arg2);
				}

				/// <summary>
				/// <para>Gets a Value from Key. (if the function is complex, the value may not be always the same, of course)</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <param name="{Arg2}">The value.</param>
				/// <returns>{bool} True if success.</returns>
				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					bool get(const Arg1& arg1, Key& arg2) const {

					if (auto* ptr = (*map)[arg1]; ptr) {
						arg2 = (*ptr)();
						return true;
					}
					return false;
				}

				/// <summary>
				/// <para>Gets a Function from Key.</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <param name="{Arg2}">The function.</param>
				/// <returns>{bool} True if success.</returns>
				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					bool get(const Arg1& arg1, std::function<Key(void)>& arg2) const {
					if (auto* ptr = (*map)[arg1]; ptr) {
						arg2 = *ptr;
						return true;
					}
					return false;
				}

				/// <summary>
				/// <para>Gets directly value as return from Key. (if the function is complex, the value may not be always the same, of course)</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <returns>{Ret} A valid value if worked, else default value (like 0).</returns>
				template<typename Ret, typename Arg1, typename Compare = r_cast_t<Ret>,
					std::enable_if_t<(!std::is_pointer<Ret>::value&& std::is_same<Compare, BaseType>::value), int> = 0
				>
					Ret get_direct(const Arg1& arg1) const {
					if (auto* ptr = (*map)[arg1]; ptr) {
						return (*ptr)();
					}
					return Ret();
				}

				/// <summary>
				/// <para>Gets directly the function from Key.</para>
				/// </summary>
				/// <param name="{Arg1}">A key.</param>
				/// <returns>{std::function*} Not nullptr if the function was found.</returns>
				template<typename Ret, typename Arg1, typename Compare = r_cast_t<std::remove_pointer_t<Ret>>,
					std::enable_if_t<(std::is_pointer<Ret>::value && std::is_same<Compare, BaseType>::value), int> = 0
				>
					std::function<Compare(void)>* get_direct(const Arg1& arg1) {
					if (auto* ptr = (*map)(arg1); ptr) {
						return ptr->get_f();
					}
					return nullptr;
				}
			};

			template<typename T>
			inline sfm_safe_feature<T>::sfm_safe_feature(const sfm_safe_feature& o)
			{
				*this = o;
			}

			template<typename T>
			inline sfm_safe_feature<T>::sfm_safe_feature(sfm_safe_feature&& o)
			{
				*this = std::move(o);
			}

			template<typename T>
			inline sfm_safe_feature<T>::sfm_safe_feature(const T& val)
			{
				set(val);
			}

			template<typename T>
			inline sfm_safe_feature<T>::sfm_safe_feature(const std::function<T(void)>& ff)
			{
				set(ff);
			}

			template<typename T>
			inline void sfm_safe_feature<T>::operator=(const sfm_safe_feature& o)
			{
				std::lock_guard<std::mutex> l1(gen_use);
				std::lock_guard<std::mutex> l2(o.gen_use);
				gen = o.gen;
				ref = o.ref;
				using_gen = o.using_gen;
			}

			template<typename T>
			inline void sfm_safe_feature<T>::operator=(sfm_safe_feature&& o)
			{
				std::lock_guard<std::mutex> l1(gen_use);
				std::lock_guard<std::mutex> l2(o.gen_use);
				gen = std::move(o.gen);
				ref = o.ref;
				using_gen = o.using_gen;
				o.using_gen = false;
			}

			template<typename T>
			inline void sfm_safe_feature<T>::set(const T& t)
			{
				using_gen = false;
				ref = t;
			}

			template<typename T>
			inline void sfm_safe_feature<T>::set(const std::function<T(void)>& f)
			{
				using_gen = false;
				{
					std::lock_guard<std::mutex> l(gen_use);
					gen = f;
				}
				using_gen = true;
			}

			template<typename T>
			inline T sfm_safe_feature<T>::operator()()
			{
				return get();
			}

			template<typename T>
			inline T sfm_safe_feature<T>::get()
			{
				if (using_gen) {
					std::lock_guard<std::mutex> l(gen_use);
					ref = gen();
					return ref;
				}
				return ref;
			}

			template<typename T>
			inline std::function<T(void)> sfm_safe_feature<T>::get_f() const
			{				
				std::lock_guard<std::mutex> l(gen_use);
				return gen;
			}
}
	}
}