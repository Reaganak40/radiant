#include "pch.h"
#include "State.h"
#include "Logging/Log.h"

glCore::glState* glCore::glState::m_singleton = nullptr;

glCore::glState::glState()
{
	if (!Log::IsInitialized()) {
		Log::Init();
	}
	
	GL_CORE_TRACE("Initialized new OpenGL state...");

	/* Initialize the library */
	if (!glfwInit()) {
		GL_CORE_ERROR("Could not initliaze glfw.");
	}
}

glCore::glState::~glState()
{
	for (auto& [name, application] : m_applications) {
		delete application;
	}

	GL_CORE_TRACE("Terminating OpenGL state...");
	glfwTerminate();
}

glCore::glState* glCore::glState::Get()
{
	if (m_singleton == nullptr) {
		m_singleton = new glState;
	}
	return m_singleton;
}

void glCore::glState::Reset()
{
	if (m_singleton != nullptr) {
		delete m_singleton;
		m_singleton = nullptr;
	}
}

glCore::glApplication& glCore::glState::RegisterApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		m_applications[name] = new glApplication(name.c_str());
	}

	return *m_applications.at(name);
}

glCore::glApplication& glCore::glState::GetApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		return RegisterApplication(name);
	}

	return *m_applications.at(name);
}

bool glCore::glState::ApplicationExists(const std::string& name)
{
	return m_applications.find(name) != m_applications.end();
}

bool glCore::glState::RemoveApplication(const std::string& name)
{
	if (!ApplicationExists(name)) {
		return false;
	}

	delete m_applications.at(name);
	m_applications.erase(name);
	return true;
}

size_t glCore::glState::ApplicationCount()
{
	return m_applications.size();
}
