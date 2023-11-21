#include "pch.h"
#include "rdt_array.h"

namespace rdt {
	template<>
	void rdt_array<char*>::setVal(size_t index, char* const val)
	{
		size_t nLength = strlen(val);
		
		if (m_arr[index] != nullptr) {
			delete m_arr[index];
		}
		m_arr[index] = new char[nLength];
		strcpy_s(m_arr[index], nLength, val);
	}

	template<>
	void rdt_array<char*>::destroyArr()
	{
		if (m_arr != nullptr) {
			for (size_t i = 0; i < m_size; i++) {
				delete m_arr[i];
			}
			delete m_arr;
		}
		m_size = 0;
	}
}