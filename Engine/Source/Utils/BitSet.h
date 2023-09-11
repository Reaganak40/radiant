#pragma once
#include "pch.h"
#include <cstdint>

namespace rdt {

	class BitSet {
	private:
		std::vector<uint32_t> m_bit_field;
	public:
		BitSet(unsigned int max_flags);
		~BitSet();

		/*
			Marks the bit at the given index as a 1.
		*/
		void ActivateFlag(unsigned int index);

		/*
			Marks the bit at the given index as a 0.
		*/
		void DeactivateFlag(unsigned int index);

		/*
			Returns true if the bit at the given index is a 1.
		*/
		bool CheckFlag(unsigned int index);

		/*
			Checks all the flags and returns true if at least one is marked.
		*/
		bool CheckFlags(unsigned int* flags, unsigned int flagCount);

		/*
			Resets the bit field to all 0s.
		*/
		void Clear();

	};

}