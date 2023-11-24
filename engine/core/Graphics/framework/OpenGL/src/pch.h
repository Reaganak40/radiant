#pragma once

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Image Processing
#include <stb/stb_image.h>

// Graphics
#include "GLAD/glad.h"
#ifdef APIENTRY
	#undef APIENTRY
#endif
#include "GLFW/glfw3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// STL
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <bitset>
#include <filesystem>
#include <sstream>
#include <fstream>
