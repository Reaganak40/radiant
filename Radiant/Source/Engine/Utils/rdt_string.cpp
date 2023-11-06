#include "pch.h"
#include "rdt_string.h"

namespace rdt {
	rdt_string::rdt_string()
	{
		m_string = std::make_shared<std::string>("");
	}

	rdt_string::rdt_string(const char* str)
	{
		m_string = std::make_shared<std::string>(str);
	}

	rdt_string::~rdt_string()
	{
	}
}
