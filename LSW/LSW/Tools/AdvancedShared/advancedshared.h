#pragma once

#include "../../Handling/Abort/abort.h"
#include "../Common/common.h" // is_type_complete_v

#include <memory>
#include <functional>

namespace LSW {
	namespace v5 {
		namespace Tools {

			/// <summary>
			/// <para></para>
			/// </summary>
			template<typename T>
			class AdvancedShared {
				std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>> data;


				template<typename Q = T, std::enable_if_t<is_type_complete_v<Q>, int> = 0>
				std::unique_ptr<T, std::function<void(T*)>>* gen_unique(T*, std::function<void(T*)> = std::function<void(T*)>());
				template<typename Q = T, std::enable_if_t<!is_type_complete_v<Q>, int> = 0>
				std::unique_ptr<T, std::function<void(T*)>>* gen_unique(T*, std::function<void(T*)> = std::function<void(T*)>());
			public:
				/// <summary>
				/// <para>Create a AdvancedShared with or without initializing internals to default.</para>
				/// </summary>
				/// <param name="{bool}">Initialize? (same as create()).</param>
				template<typename Q = T, std::enable_if_t<is_type_complete_v<Q>, int> = 0>
				AdvancedShared(const bool = false);

				/// <summary>
				/// <para>Create a AdvancedShared without initializing internals vecause of incomplete type.</para>
				/// </summary>
				template<typename Q = T, std::enable_if_t<!is_type_complete_v<Q>, int> = 0>
				AdvancedShared();

				/// <summary>
				/// <para>Create a AdvancedShared with a value set.</para>
				/// </summary>
				/// <param name="{T}">Value to copy.</param>
				template<typename Q = T, std::enable_if_t<is_type_complete_v<Q>, int> = 0>
				AdvancedShared(const T&);

				/// <summary>
				/// <para>Create a AdvancedShared with a pointer and its destructor set.</para>
				/// </summary>
				/// <param name="{T*}">A valid pointer to be stored and managed.</param>
				/// <param name="{std::function}">The destructor to this pointer.</param>
				AdvancedShared(T*, std::function<void(T*)> = std::function<void(T*)>());

				/// <summary>
				/// <para>Copy constructor.</para>
				/// <para>Adds a new reference to whatever this is being set.</para>
				/// </summary>
				/// <param name="{AdvancedShared}">To copy and reference from.</param>
				AdvancedShared(const AdvancedShared&);

				/// <summary>
				/// <para>Move constructor.</para>
				/// <para>The other one gets invalid after this.</para>
				/// </summary>
				/// <param name="{AdvancedShared}">To move from.</param>
				AdvancedShared(AdvancedShared&&);


				/// <summary>
				/// <para>Copy operator.</para>
				/// <para>Adds a new reference to whatever this is being set.</para>
				/// </summary>
				/// <param name="{AdvancedShared}">To copy and reference from.</param>
				void operator=(const AdvancedShared&);

				/// <summary>
				/// <para>Move operator.</para>
				/// <para>The other one gets invalid after this.</para>
				/// </summary>
				/// <param name="{AdvancedShared}">To move from.</param>
				void operator=(AdvancedShared&&);

				/// <summary>
				/// <para>Create the data inside with default value and destructor.</para>
				/// </summary>
				template<typename Q = T, std::enable_if_t<is_type_complete_v<Q>, int> = 0>
				void create();

				/// <summary>
				/// <para>Create the data inside with this value and default destructor.</para>
				/// </summary>
				/// <param name="{T}">Value to set.</param>
				template<typename Q = T, std::enable_if_t<is_type_complete_v<Q>, int> = 0>
				void create(const T&);

				/// <summary>
				/// <para>Set a pointer with default destructor as this (creates with this).</para>
				/// </summary>
				/// <param name="{T*}">A valid pointer to be stored and managed.</param>
				void create(T*);

				/// <summary>
				/// <para>Set a pointer and its destructor as this (creates with this).</para>
				/// </summary>
				/// <param name="{T*}">A valid pointer to be stored and managed.</param>
				/// <param name="{std::function}">The destructor to this pointer.</param>
				void create(T*, std::function<void(T*)>);

				/// <summary>
				/// <para>Change internal shared pointer (used by all references).</para>
				/// </summary>
				/// <param name="{T*}">Swap with this. If there was something internally, it swaps, else moves.</param>
				void swap(T*&);

				/// <summary>
				/// <para>Change internal shared pointer (used by all references) and also destroy with internal destructor.</para>
				/// </summary>
				/// <param name="{T*}">Swap with this. It will destroy what's inside.</param>
				void swap_destroy(T*);
				
				/// <summary>
				/// <para>Swap this shared_ptr internally (won't change others).</para>
				/// </summary>
				/// <param name="{AdvancedShared}">This will have its data swapped with this' data.</param>
				void swap(AdvancedShared&);

				/// <summary>
				/// <para>Reset this object to default. You'll have to re-create() or reassign.</para>
				/// </summary>
				void reset_this();

				/// <summary>
				/// <para>Reset the shared data. You might want to swap() a T* to share again with everyone else.</para>
				/// </summary>
				void reset_all();

				/// <summary>
				/// <para>Automatic cast.</para>
				/// </summary>
				operator T*();

				/// <summary>
				/// <para>Automatic cast.</para>
				/// </summary>
				operator const T*() const;

				/// <summary>
				/// <para>Get internal data as reference.</para>
				/// </summary>
				/// <returns>{T*} The data itself.</returns>
				T& operator*();

				/// <summary>
				/// <para>Get internal data as reference.</para>
				/// </summary>
				/// <returns>{T*} The data itself.</returns>
				const T& operator*() const;

				/// <summary>
				/// <para>Checks if loaded.</para>
				/// </summary>
				/// <returns>{bool} True if loaded.</returns>
				operator bool() const;

				/// <summary>
				/// <para>Checks if loaded.</para>
				/// </summary>
				/// <returns>{bool} False if loaded.</returns>
				bool operator!() const;

				/// <summary>
				/// <para>Checks if it's empty/null.</para>
				/// </summary>
				/// <returns>{bool} False if loaded.</returns>
				bool null() const;

				/// <summary>
				/// <para>Gets how many are sharing the same data as this one.</para>
				/// </summary>
				/// <returns>{size_t} Amount of references.</returns>
				size_t use_count() const;

				/// <summary>
				/// <para>Get internal data pointer.</para>
				/// </summary>
				/// <returns>{T*} The data itself.</returns>
				T* get();

				/// <summary>
				/// <para>Get internal data pointer.</para>
				/// </summary>
				/// <returns>{T*} The data itself.</returns>
				T* get() const;

				/// <summary>
				/// <para>Get the real shared_ptr of unique_ptr handler.</para>
				/// </summary>
				/// <returns>{std::shared_ptr} The real internal sharing solution data.</returns>
				const std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>>& get_internal_data() const;
			};






			// IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION - IMPLEMENTATION //


			template<typename T>
			template<typename Q, std::enable_if_t<is_type_complete_v<Q>, int>>
			std::unique_ptr<T, std::function<void(T*)>>* AdvancedShared<T>::gen_unique(T* v, std::function<void(T*)> f)
			{
				if (f) {
					auto* gen = new std::unique_ptr<T, std::function<void(T*)>>(v, f);
					return std::move(gen);
				}
				else {
					auto* gen = new std::unique_ptr<T, std::function<void(T*)>>(v, [](T* t) { delete t; });
					return std::move(gen);
				}
			}

			template<typename T>
			template<typename Q, std::enable_if_t<!is_type_complete_v<Q>, int>>
			std::unique_ptr<T, std::function<void(T*)>>* AdvancedShared<T>::gen_unique(T* v, std::function<void(T*)> f)
			{
				if (f) {
					auto* gen = new std::unique_ptr<T, std::function<void(T*)>>(v, f);
					return std::move(gen);
				}
				else {
					throw Handling::Abort(__FUNCSIG__, "Incomplete type cannot be created like this! Function invalid when creating a AdvancedShared.");
				}
			}

			template<typename T>
			template<typename Q, std::enable_if_t<is_type_complete_v<Q>, int>>
			AdvancedShared<T>::AdvancedShared(const bool creat)
			{
				if (creat) create();
			}

			template<typename T>
			template<typename Q, std::enable_if_t<!is_type_complete_v<Q>, int>>
			inline AdvancedShared<T>::AdvancedShared()
			{
				// nothing to do.
			}

			template<typename T>
			template<typename Q, std::enable_if_t<is_type_complete_v<Q>, int>>
			AdvancedShared<T>::AdvancedShared(const T& v)
			{
				create(v);
			}

			template<typename T>
			AdvancedShared<T>::AdvancedShared(T* p, std::function<void(T*)> f)
			{
				if (f) create(p, f);
				else create(p);
			}

			template<typename T>
			inline AdvancedShared<T>::AdvancedShared(const AdvancedShared& c)
			{
				*this = c;
			}

			template<typename T>
			inline AdvancedShared<T>::AdvancedShared(AdvancedShared&& m)
			{
				*this = std::move(m);
			}

			template<typename T>
			inline void AdvancedShared<T>::operator=(const AdvancedShared& c)
			{
				data = c.data;
			}

			template<typename T>
			inline void AdvancedShared<T>::operator=(AdvancedShared&& m)
			{
				data = std::move(m.data);
			}

			template<typename T>
			template<typename Q, std::enable_if_t<is_type_complete_v<Q>, int>>
			void AdvancedShared<T>::create()
			{

				data = std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>>(gen_unique(new T()));
			}

			template<typename T>
			template<typename Q, std::enable_if_t<is_type_complete_v<Q>, int>>
			void AdvancedShared<T>::create(const T& val)
			{
				create();
				**this = val;
			}

			template<typename T>
			void AdvancedShared<T>::create(T* pre)
			{
				if (!pre) throw Handling::Abort(__FUNCSIG__, "Invalid pointer.");
				data = std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>>(gen_unique(pre));
			}

			template<typename T>
			void AdvancedShared<T>::create(T* pre, std::function<void(T*)> f)
			{
				if (!pre || !f) throw Handling::Abort(__FUNCSIG__, "Invalid pointer or function.");
				data = std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>>(gen_unique(pre, f));
			}

			template<typename T>
			void AdvancedShared<T>::swap(T*& oth)
			{
				if (data) {
					T* b4 = data->get();
					data->reset(oth);
					oth = b4;
				}
				else {
					create(oth);
					oth = nullptr;
				}
			}

			template<typename T>
			inline void AdvancedShared<T>::swap_destroy(T* oth)
			{
				auto del = data->get_deleter();

				if (data) {
					T* b4 = data->get();
					data->reset(oth);
					del(b4);
				}
				else {
					create(oth);
					oth = nullptr;
				}
			}

			template<typename T>
			void AdvancedShared<T>::swap(AdvancedShared& s)
			{
				data.swap(s.data);
			}

			template<typename T>
			void AdvancedShared<T>::reset_this()
			{
				data.reset();
			}

			template<typename T>
			void AdvancedShared<T>::reset_all()
			{
				if (data) data->reset();
			}

			template<typename T>
			AdvancedShared<T>::operator T* ()
			{
				if (!data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return data->get();
			}

			template<typename T>
			AdvancedShared<T>::operator const T* () const
			{
				if (!data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return data->get();
			}

			template<typename T>
			T& AdvancedShared<T>::operator*()
			{
				if (!data || !*data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return *data->get();
			}
			template<typename T>
			const T& AdvancedShared<T>::operator*() const
			{
				if (!data || !*data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return *data->get();
			}

			template<typename T>
			inline AdvancedShared<T>::operator bool() const
			{
				return !null();
			}

			template<typename T>
			inline bool AdvancedShared<T>::operator!() const
			{
				return null();
			}

			template<typename T>
			inline bool AdvancedShared<T>::null() const
			{
				return (data.get() == nullptr || data->get() == nullptr);
			}

			template<typename T>
			size_t AdvancedShared<T>::use_count() const
			{
				return data.use_count();
			}

			template<typename T>
			T* AdvancedShared<T>::get()
			{
				if (!data || !*data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return data->get();
			}

			template<typename T>
			T* AdvancedShared<T>::get() const
			{
				if (!data || !*data) throw Handling::Abort(__FUNCSIG__, "Null data.");
				return data->get();
			}

			template<typename T>
			const std::shared_ptr<std::unique_ptr<T, std::function<void(T*)>>>& AdvancedShared<T>::get_internal_data() const
			{
				return data;
			}
		}
	}
}