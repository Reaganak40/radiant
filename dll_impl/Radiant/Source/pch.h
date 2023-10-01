#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

/**********************************
*
*		 VENDOR LIBRARIES
*
**********************************/

// Graphics
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Audio
//#include <AL/al.h>
//#include <AL/alc.h>
//#include <AudioFile-1.1.1/AudioFile.h>

// Logging
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

/**********************************
*
*	    Standard Libraries
*
**********************************/
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <array>
#include <set>
#include <memory>
#include <mutex>
