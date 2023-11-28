/***************************************************************/
/*  Utils/Array.hpp                                            */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include <Radiant/Utils/Export.hpp>

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

	RDT_UTILS_EXTERN template class RDT_UTILS_API rdt_array<char*>;
	using STRING_ARRAY = rdt_array<char*>;
}