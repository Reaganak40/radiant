#pragma once
#include "Core.h"

namespace rdt {

	EXPORT_STL(std::shared_ptr<std::string>);

	class RADIANT_API rdt_string {
	private:
		std::shared_ptr<std::string> m_string;

	public:
		rdt_string();
		rdt_string(const char* str);
		~rdt_string();


		void operator=(const char* str)
		{
			*m_string = str;
		}

		void operator=(const std::string& str)
		{
			*m_string = str;
		}

		operator const char*() const
		{
			return m_string->c_str();
		}
	};
}