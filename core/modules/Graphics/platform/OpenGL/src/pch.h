#pragma once

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Graphics
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Logging
#include <CodeAnalysis/Warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

// STL
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <bitset>
#include <filesystem>
