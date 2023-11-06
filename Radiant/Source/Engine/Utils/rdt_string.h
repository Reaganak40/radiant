/*******************************************************************
*	Module:  Utils
*	File:    rdt_string.h
*
*	Author: Reagan Kelley
*
*   A wrapper data structure for strings, that can be safely used
*	in place of std::string. Allows for safe dll-exports of string.
*******************************************************************/
#pragma once
#include "Core.h"

namespace rdt {

	class RADIANT_API rdt_string {
	private:
		unsigned int string_id;
	public:
		rdt_string(const char* str = "");
		rdt_string(const rdt_string& other);

		~rdt_string();

		const char* c_str() const;
		void clear();

		rdt_string& operator=(const rdt_string& other);
		rdt_string& operator=(const std::string& other);
		rdt_string& operator=(const char* other);
	};
}