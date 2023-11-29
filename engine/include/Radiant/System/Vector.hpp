/***************************************************************/
/*  System/Vector.hpp                                          */
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
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t


namespace rdt {
	
	template<typename T>
	class vector {
	private:
		T* data;         // contiguous block of T data
		size_t count;    // number of elements in array
		size_t capacity; // number of elements that can fit in the array
	public:

		struct Iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type   = std::ptrdiff_t;
			using value_type         = T;
			using pointer            = T*;
			using reference          = T&;

		private:
			pointer m_ptr;
		public:

			reference operator*() const { return *m_ptr; }
			pointer operator->()        { return m_ptr; }

			// Prefix increment
			Iterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		};

		Iterator begin() { return Iterator(&data[0]);       }
		Iterator end()   { return Iterator(&data[count]); }

		Iterator find(const T& val)
		{
			for (auto it = begin(); it != end(); ++it) {
				if ((*it) == val) {
					return it;
				}
			}

			return end();
		}

		vector()
			: data(nullptr), count(0) {}

		void push_back(const T& element)
		{
			if (size == capacity)
			{
				realloc(capacity * 2);
			}
			data[size++] = element;
			count++;
		}

		T operator [](size_t index) const 
		{
			return data[index];
		}

		T& operator [](size_t index)
		{ 
			return data[index];
		}

		void insert(size_t index, const T& element)
		{
			if (count == capacity)
			{
				realloc(capacity * 2);
			}

			size_t front = count;
			while (front > index) {
				data[front] = data[--front];
			}
			data[count] = element;
			count++;
		}

		void erase(size_t index)
		{
			for (; index < (size-1); index++) {
				data[index] = data[++index];
			}
			count--;
		}

		size_t count() { return count; }

	private:

		void realloc(size_t nCapacity)
		{
			T* nArr = new T[nCapacity];

			for (size_t i = 0; i < count; i++) {
				nArr[i] = data[i];
			}

			delete data;
			data = nArr;
			capacity = nCapacity;
		}
	};

}