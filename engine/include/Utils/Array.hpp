#pragma once
#include "utils_api.h"

namespace rdt {
	
	template<typename T>
	class rdt_array {
	private:
		T* m_arr;
		size_t m_size;
	public:

		rdt_array()
			: m_arr(nullptr), m_size(0)
		{
		}
		
		rdt_array(T* arr, size_t numItems)
			: m_arr(nullptr), m_size(0)
		{
			initArr(numItems);
			for (size_t i = 0; i < numItems; i++)
			{
				setVal(i, arr[i]);
			}
		}

		rdt_array(size_t size)
			: m_arr(nullptr), m_size(0)
		{
			initArr(size);
		}

		~rdt_array()
		{
			destroyArr();
		}

		 rdt_array(const rdt_array& other) = delete;

		size_t size()
		{
			return m_size;
		}

		T* data()
		{
			return m_arr;
		}

		T operator  [](int i) const { return m_arr[i]; }
		T& operator [](int i)	    { return m_arr[i]; }
		void setVal(size_t index, T val);

		void overwrite(T* arr, size_t numItems)
		{
			destroyArr();
			initArr(numItems);

			for (size_t i = 0; i < numItems; i++)
			{
				setVal(i, arr[i]);
			}
		}
		
		void Reset(size_t nSize)
		{
			destroyArr();
			initArr(nSize);
		}
	private:
		void initArr(size_t size);
		void destroyArr()
		{
			if (m_arr != nullptr) {
				delete m_arr;
				m_arr = nullptr;
			}
			m_size = 0;
		}
	};

	template<typename T>
	inline void rdt_array<T>::initArr(size_t size)
	{
		m_arr = new T[size];
		m_size = size;
		if (std::is_pointer<T>::value)
		{
			for (size_t i = 0; i < size; i++)
			{
				m_arr[i] = nullptr;
			}
		}
	}


	template <typename T>
	void rdt_array<T>::setVal(size_t index, const T val)
	{
		m_arr[index] = val;
	}


	template<>
	void rdt_array<char*>::setVal(size_t index, char* val);
	template<>
	void rdt_array<char*>::destroyArr();

	UTILS_EXTERN template class UTILS_API rdt_array<char*>;
	using STRING_ARRAY = rdt_array<char*>;
}