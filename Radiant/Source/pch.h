#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

/**********************************
*
*		 VENDOR LIBRARIES
*
**********************************/

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Audio
#include <al.h>
#include <alc.h>
#include <AudioFile.h>

// Logging
#include <CodeAnalysis/Warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"
#pragma warning(pop)

// Graphics
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/**********************************
*
*	    Standard Libraries
*
**********************************/
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <queue>
#include <array>
#include <set>
#include <memory>
#include <mutex>
