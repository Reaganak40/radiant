#pragma once

namespace rdt::core {
	class ConfigReader {
	private:
		std::string filepath;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_sections;
	public:

		ConfigReader();
		~ConfigReader();

		/*
			Sets a filepath that can be used for default reading and writing to files.
		*/
		void SetTargetFile(std::string defaultFilepath);

		/*
			Reads a config file and updates the config state internals of this
			object instance. Returns false if the file does not exist.
		*/
		bool Read(const std::string& configFile = "");

		/*
			Writes the config state in this object instance to a config file
			specified in the provided filepath.
		*/
		void Write(const std::string& configFile = "");

		template<typename T>
		bool GetAttribute(const std::string& section, const std::string& key, T& out) { return false; }

		template<>
		bool GetAttribute<std::string>(const std::string& section, const std::string& key, std::string& out) {
			if (!IsInConfigFile(section, key)) {
				return false;
			}
			out = m_sections.at(section).at(key);
			return true;
		}

		template<>
		bool GetAttribute<int>(const std::string& section, const std::string& key, int& out) {
			if (!IsInConfigFile(section, key)) {
				return false;
			}
			out = std::stoi(m_sections.at(section).at(key));
			return true;
		}

		// =====================================================================================

		template<typename T>
		void SetAttribute(const std::string& section, const std::string& key, const T& out) { m_sections[section][key] = std::to_string(out); }

		template<>
		void SetAttribute<std::string>(const std::string& section, const std::string& key, const std::string& out) {
			m_sections[section][key] = out;
		}


		
	private:
		/*
			Returns true if the section.key pair exists in the config file.
		*/
		bool IsInConfigFile(const std::string& section, const std::string& key);


	};
}