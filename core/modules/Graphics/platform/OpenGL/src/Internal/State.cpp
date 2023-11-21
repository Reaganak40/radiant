#include "pch.h"
#include "State.h"

#include <Radiant/Logger.h>

rdt::glCore::glState* rdt::glCore::glState::m_singleton = nullptr;

rdt::glCore::glState::glState()
{
	RDT_CORE_TRACE("Initialized new OpenGL state...");

	/* Initialize the library */
	if (!glfwInit()) {
		RDT_CORE_ERROR("Could not initliaze glfw.");
	}
}

rdt::glCore::glState::~glState()
{
	for (auto& [name, application] : m_applications) {
		delete application;
	}

	RDT_CORE_TRACE("Terminating OpenGL state...");
	glfwTerminate();
}

rdt::glCore::glState* rdt::glCore::glState::Get()
{
	if (m_singleton == nullptr) {
		m_singleton = new glState;
	}
	return m_singleton;
}

void rdt::glCore::glState::Reset()
{
	if (m_singleton != nullptr) {
		delete m_singleton;
		m_singleton = nullptr;
	}
}

rdt::glCore::glApplication& rdt::glCore::glState::RegisterApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		m_applications[name] = new glApplication(name.c_str());
	}

	return *m_applications.at(name);
}

rdt::glCore::glApplication& rdt::glCore::glState::GetApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		return RegisterApplication(name);
	}

	return *m_applications.at(name);
}

bool rdt::glCore::glState::ApplicationExists(const std::string& name)
{
	return m_applications.find(name) != m_applications.end();
}

bool rdt::glCore::glState::RemoveApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		return false;
	}

	delete m_applications.at(name);
	m_applications.erase(name);
	return true;
}

size_t rdt::glCore::glState::ApplicationCount()
{
	return m_applications.size();
}
