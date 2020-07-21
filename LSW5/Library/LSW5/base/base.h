#pragma once

// C++
#include <string>
#include <functional>
#include <type_traits>
// Others
#include "..\supermap\supermap.h"
#include "..\autocaster\autocaster.h"

namespace LSW {
	namespace v5 {

		namespace base {
			enum class e_base { BASE, BASE_FUNC }; // virtual use to know who it is
		}

		template<typename BaseType, typename TransformedToFuncType = std::function<BaseType(void)>>
		class Workaround {
		protected:
			std::shared_ptr<SuperMap<TransformedToFuncType>> map = std::make_shared<SuperMap<TransformedToFuncType>>();
		public:
			Workaround() = default;

			template<typename Key, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				std::shared_ptr<SuperMap<TransformedToFuncType>> get() {
				return map;
			}

			// COPY REFERENCE (because not std::shared_ptr)
			template<typename Key, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				void set(SuperMap<TransformedToFuncType> mp) {
				map->add(mp);
			}

			// DUPLICATE REFERENCE (because you are doing std::shared_ptr) [no copy]
			template<typename Key, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				void set(std::shared_ptr<SuperMap<TransformedToFuncType>> mp) {
				map = mp;
			}

			// you can get<type>()->set to set a new key / handle supermap
			/*template<typename Key, typename Arg1, typename Arg2, typename Compare1 = Cast::r_cast_t<Arg1>, typename Compare2 = Cast::r_cast_t<Arg2>, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<(!std::is_same<Compare1, Compare2>::value&& std::is_same<Compare, BaseType>::value), int> = 0
			>
				void add(Arg1 arg1, Arg2 arg2) {
				SuperPair<TransformedToFuncType>* ptr = nullptr;
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast

					ptr = map->getPair(arg1_transformed->c_str(), arg1_transformed->length());
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast

					ptr = map->getPair(*arg1_transformed, strlen(*arg1_transformed));
				}
				else {
					ptr = map->getPair(arg1);
				}

				if (ptr) {
					if (std::is_same<Cast::r_cast_t<Arg2>, std::string>::value) {
						std::string* arg2_transformed = ((std::string*)(void*) & arg2); // ultracast
						ptr->set(arg2_transformed->c_str(), arg2_transformed->length());
					}
					else if (std::is_same<Cast::r_cast_t<Arg2>, char*>::value) {
						char** arg2_transformed = ((char**)(void*)&arg2); // ultracast
						ptr->set(*arg2_transformed, strlen(*arg2_transformed));
					}
					else {
						ptr->set(arg2);
					}
				}
			}*/

			template<typename Key, typename Arg1, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				void set(Arg1 arg1, Key arg2) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast

					auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length());
					if (!ptr) map->add({ [=] {return arg2; } , arg1_transformed->c_str(), arg1_transformed->length() });
					else *ptr = [=] {return arg2; };
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed));
					if (!ptr) map->add({ [=] {return arg2; }, *arg1_transformed, strlen(*arg1_transformed) });
					else *ptr = [=] {return arg2; };
				}
				else {
					auto* ptr = (*map)(arg1);
					if (!ptr) map->add({ [=] {return arg2; }, arg1 });
					else *ptr = [=] {return arg2; };
				}
			}

			template<typename Key, typename Arg1, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				void set(Arg1 arg1, std::function<Key(void)> arg2) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast

					auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length());
					if (!ptr) map->add({ arg2, arg1_transformed->c_str(), arg1_transformed->length() });
					else *ptr = arg2;
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed));
					if (!ptr) map->add({ arg2, *arg1_transformed, strlen(*arg1_transformed) });
					else *ptr = arg2;
				}
				else {
					auto* ptr = (*map)(arg1);
					if (!ptr) map->add({ arg2, arg1 });
					else *ptr = arg2;
				}
			}

			template<typename Key, typename Arg1, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				bool get(Arg1 arg1, Key& arg2) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast
					if (auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length()); ptr) {
						arg2 = (*ptr)();
						return true;
					}
					return false;
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					if (auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed)); ptr) {
						arg2 = (*ptr)();
						return true;
					}
					return false;
				}
				else {
					if (auto* ptr = (*map)(arg1); ptr) {
						arg2 = (*ptr)();
						return true;
					}
					return false;
				}
			}

			template<typename Key, typename Arg1, typename Compare = Cast::r_cast_t<Key>,
				std::enable_if_t<std::is_same<Compare, BaseType>::value, int> = 0
			>
				bool get(Arg1 arg1, std::function<Key(void)>& arg2) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast
					if (auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length()); ptr) {
						arg2 = *ptr;
						return true;
					}
					return false;
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					if (auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed)); ptr) {
						arg2 = *ptr;
						return true;
					}
					return false;
				}
				else {
					if (auto* ptr = (*map)(arg1); ptr) {
						arg2 = *ptr;
						return true;
					}
					return false;
				}
			}

			template<typename Ret, typename Arg1, typename Compare = Cast::r_cast_t<Ret>,
				std::enable_if_t<(!std::is_pointer<Ret>::value&& std::is_same<Compare, BaseType>::value), int> = 0
			>
				Ret getDirect(Arg1 arg1) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast
					if (auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length()); ptr) {
						return (*ptr)();
					}
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					if (auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed)); ptr) {
						return (*ptr)();
					}
				}
				else {
					if (auto* ptr = (*map)(arg1); ptr) {
						return (*ptr)();
					}
				}
				return Ret();
			}

			template<typename Ret, typename Arg1, typename Compare = Cast::r_cast_t<std::remove_pointer_t<Ret>>,
				std::enable_if_t<(std::is_pointer<Ret>::value&& std::is_same<Compare, BaseType>::value), int> = 0
			>
				std::function<Compare(void)>* getDirect(Arg1 arg1) {
				if (std::is_same<Cast::r_cast_t<Arg1>, std::string>::value) {
					std::string* arg1_transformed = ((std::string*)(void*) & arg1); // ultracast
					if (auto* ptr = (*map)(arg1_transformed->c_str(), arg1_transformed->length()); ptr) {
						return ptr;
					}
				}
				else if (std::is_same<Cast::r_cast_t<Arg1>, char*>::value) {
					char** arg1_transformed = ((char**)(void*)&arg1); // ultracast
					if (auto* ptr = (*map)(*arg1_transformed, strlen(*arg1_transformed)); ptr) {
						return ptr;
					}
				}
				else {
					if (auto* ptr = (*map)(arg1); ptr) {
						return ptr;
					}
				}
				return nullptr;
			}
		};

		///template<typename T> using CommonMap = SmartSuperMap<T>;
		///template<typename T> using FunctionalMap = SmartSuperMap<std::function<T(void)>>;
		// use this if you want to keep things easy (probably)
		///template<typename T> using DF = std::function<T(void)>;
	}
}