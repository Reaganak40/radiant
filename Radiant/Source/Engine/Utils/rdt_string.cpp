#include "pch.h"
#include "rdt_string.h"

namespace rdt {

	constexpr unsigned int NO_STRING_ID = 0;

	class rdt_string_handler {
		static std::unordered_map<unsigned int, std::string> strings;
		static unsigned int stringCounter;

	public:

		static std::string* GetString(unsigned int string_id) {
			if (strings.find(string_id) == strings.end()) {
				return nullptr;
			}

			return &strings.at(string_id);
		}

		static unsigned MakeString(const std::string& str) {
			unsigned int nID = ++stringCounter;
			strings[nID] = str;
			return nID;
		}

		static void Destroy(unsigned int string_id) {
			strings.erase(string_id);
		}

		static void Copy(unsigned int string_id_dest, unsigned int string_id_src) {
			if (strings.find(string_id_dest) == strings.end()) {
				return;
			}

			if (strings.find(string_id_src) == strings.end()) {
				return;
			}

			strings.at(string_id_dest) = strings.at(string_id_src);
		}
	};
	std::unordered_map<unsigned int, std::string> rdt_string_handler::strings;
	unsigned int rdt_string_handler::stringCounter = 0;

	rdt_string::rdt_string(const char* str)
		: string_id(NO_STRING_ID)
	{
		string_id = rdt_string_handler::MakeString(str);
	}

	rdt_string::rdt_string(const rdt_string& other)
		: string_id(NO_STRING_ID)
	{
		if (string_id == NO_STRING_ID) {
			string_id = rdt_string_handler::MakeString("");
		}
		rdt::rdt_string_handler::Copy(string_id, other.string_id);
	}

	rdt_string::~rdt_string()
	{
		if (string_id != NO_STRING_ID) {
			rdt_string_handler::Destroy(string_id);
		}
	}
	const char* rdt_string::c_str() const
	{
		return rdt_string_handler::GetString(string_id)->c_str();
	}
	void rdt_string::clear()
	{
		rdt_string_handler::GetString(string_id)->clear();
	}
	rdt_string& rdt_string::operator=(const rdt_string& other)
	{
		rdt::rdt_string_handler::Copy(string_id, other.string_id);
		return *this;
	}
	rdt_string& rdt_string::operator=(const std::string& other)
	{
		*rdt_string_handler::GetString(string_id) = other;
		return *this;
	}
	rdt_string& rdt_string::operator=(const char* other)
	{
		*rdt_string_handler::GetString(string_id) = other;
		return *this;
	}
}
