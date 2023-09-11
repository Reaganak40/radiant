#include "pch.h"
#include "BitSet.h"

namespace rdt {
	BitSet::BitSet(unsigned int max_flags)
	{
		m_bit_field = std::vector<uint32_t>((max_flags / 32) + 1, 0);
	}

	BitSet::~BitSet()
	{
	}

	void BitSet::ActivateFlag(unsigned int index)
	{
		m_bit_field.at(index / 32) |= (1 << (index % 32));
	}

	void BitSet::DeactivateFlag(unsigned int index)
	{
		m_bit_field.at(index / 32) ^= (1 << (index % 32));
	}

	bool BitSet::CheckFlag(unsigned int index)
	{
		return (bool)(m_bit_field.at(index / 32) & (1 << (index % 32)));
	}

	bool BitSet::CheckFlags(unsigned int* flags, unsigned int flagCount)
	{
		for (unsigned int i = 0; i < flagCount; i++) {
			if (CheckFlag(flags[i])) {
				return true;
			}
		}
		return false;
	}

	void BitSet::Clear()
	{
		for (auto& field : m_bit_field) {
			field = 0;
		}
	}

}