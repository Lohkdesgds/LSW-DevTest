#pragma once

#include <string>
#include <functional>
#include <type_traits>

#include "..\SuperMap\supermap.h"
#include "..\Common\common.h"

namespace LSW {
	namespace v5 {
		namespace Tools {

			namespace base {
				enum class e_base { BASE, BASE_FUNC }; // virtual use to know who it is
			}

			template<typename BaseType, typename TransformedToFuncType = std::function<BaseType(void)>>
			class SuperFunctionalMap {
			protected:
				std::shared_ptr<SuperMap<TransformedToFuncType>> map = std::make_shared<SuperMap<TransformedToFuncType>>();
			public:
				SuperFunctionalMap() = default;
				SuperFunctionalMap(const SuperMap<TransformedToFuncType>& olmap) {
					(*map) = olmap;
				}

				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					std::shared_ptr<SuperMap<TransformedToFuncType>> get() {
					return map;
				}

				// COPY REFERENCE (because not std::shared_ptr)
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(SuperMap<TransformedToFuncType> mp) {
					map->add(mp);
				}

				// DUPLICATE REFERENCE (because you are doing std::shared_ptr) [no copy]
				template<typename Key, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(std::shared_ptr<SuperMap<TransformedToFuncType>> mp) {
					map = mp;
				}

				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(Arg1 arg1, Key arg2) {
					auto* ptr = (*map)[arg1];
					if (!ptr) map->add({ [=] {return arg2; }, arg1 });
					else *ptr = [=] {return arg2; };
				}

				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					void set(Arg1 arg1, std::function<Key(void)> arg2) {
					auto* ptr = (*map)[arg1];
					if (!ptr) map->add({ arg2, arg1 });
					else *ptr = arg2;
				}

				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					bool get(Arg1 arg1, Key& arg2) {

					if (auto* ptr = (*map)[arg1]; ptr) {
						arg2 = (*ptr)();
						return true;
					}
					return false;
				}

				template<typename Key, typename Arg1, typename Compare = r_cast_t<Key>,
					std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
				>
					bool get(Arg1 arg1, std::function<Key(void)>& arg2) {
					if (auto* ptr = (*map)[arg1]; ptr) {
						arg2 = *ptr;
						return true;
					}
					return false;
				}

				template<typename Ret, typename Arg1, typename Compare = r_cast_t<Ret>,
					std::enable_if_t<(!std::is_pointer<Ret>::value&& std::is_same<Compare, BaseType>::value), int> = 0
				>
					Ret get_direct(Arg1 arg1) {
					if (auto* ptr = (*map)[arg1]; ptr) {
						return (*ptr)();
					}
					return Ret();
				}

				template<typename Ret, typename Arg1, typename Compare = r_cast_t<std::remove_pointer_t<Ret>>,
					std::enable_if_t<(std::is_pointer<Ret>::value&& std::is_same<Compare, BaseType>::value), int> = 0
				>
					std::function<Compare(void)>* get_direct(Arg1 arg1) {
					if (auto* ptr = (*map)(arg1); ptr) {
						return ptr;
					}
					return nullptr;
				}
			};
		}
	}
}