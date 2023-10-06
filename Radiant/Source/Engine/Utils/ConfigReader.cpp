#include "pch.h"
#include "ConfigReader.h"
#include "Utils.h"


namespace rdt::core {
	ConfigReader::ConfigReader()
	{
		filepath = "nopath";
	}
	ConfigReader::~ConfigReader()
	{
	}

	void ConfigReader::SetTargetFile(std::string defaultFilepath)
	{
		filepath = defaultFilepath;
	}

	bool ConfigReader::Read(const std::string& configFile)
	{
		if (configFile.empty()) {
			return Read(filepath);
		}

		m_sections.clear();
		if (!Utils::PathExists(configFile)) {
			return false;
		}

		std::ifstream infile;
		infile.open(configFile);

		std::string section;
		std::string line;
		while (std::getline(infile, line)) {
			
			if (line[0] == '[' && line[line.length() - 1] == ']') {
				section = line.substr(1, line.length() - 2);
			}
			else {
				std::vector<std::string> tokens;
				Utils::Tokenize(line, " = ", tokens);

				if (tokens.size() == 2) {
					m_sections[section][tokens[0]] = tokens[1];
				}
			}
		}

	}

	void ConfigReader::Write(const std::string& configFile)
	{
		if (configFile.empty()) {
			Write(filepath);
			return;
		}

		if (!Utils::PathExists(Utils::ParentPath(configFile))) {
			return;
		}

		std::ofstream myfile;
		myfile.open(configFile);
		
		for (auto& [section, attributes] : m_sections) {
			myfile << "[" << section << "]\n";

			for (auto& [key, attribute] : attributes) {
				myfile << key << " = " << attribute << "\n";
			}
			myfile << "\n";
		}

		myfile.close();
	}

	bool ConfigReader::IsInConfigFile(const std::string& section, const std::string& key)
	{
		if (m_sections.find(section) == m_sections.end()) {
			return false;
		}
		const auto& attributes = m_sections.at(section);
		if (attributes.find(key) == attributes.end()) {
			return false;
		}
		return true;
	}
}
