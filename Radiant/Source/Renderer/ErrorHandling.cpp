#include "pch.h"
#include "ErrorHandling.h"
#include <sstream>

namespace Radiant {

	void glClearError() {
		int count = 0;
		while (glGetError() != GL_NO_ERROR) {
			count++;
		}
		if (count > 0) {
			std::cout << "Warning: found (" << count << ") unbounded errors\n";
		}
	}

	bool glLogError(const char* function, const char* filename, int line) {

		bool noErrors = true;
		while (GLenum error = glGetError()) {
			std::cout << "[Error::Opengl-Core]\n" <<
				"\tfunction: " << function << std::endl <<
				"\tfilepath: " << filename << std::endl <<
				"\tline: " << line << std::endl <<
				"\terror: (" << glErrorContext(error) << ")\n";
			noErrors = false;
		}

		return noErrors;
	}

	std::string glErrorContext(unsigned int error) {

		switch (error) {

		case 0x500:
			return "GL_INVALID_ENUM";
		case 0x501:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		default:
			std::stringstream ss;
			ss << "0x" << std::hex << error;
			return ss.str();
		}
		return "FAIL";
	}

	void GLAPIENTRY MessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		std::string severityStr;
		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			severityStr = "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityStr = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			severityStr = "HIGH";
			break;
		default:
			severityStr = "UNKNOWN";
		}

		fprintf(stderr, "GL Error: %s type = 0x%x, severity = %s:\n\tmessage = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severityStr.c_str(), message);

		if (severity == GL_DEBUG_SEVERITY_HIGH || severityStr == "UNKNOWN") {
			ASSERT(false);
		}
	}

	void EnableErrorCallback() {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
	}

}