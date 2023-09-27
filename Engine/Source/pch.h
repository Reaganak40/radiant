#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

/**********************************
*
*		 VENDOR LIBRARIES
* 
**********************************/

// Graphics
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Audio
#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile-1.1.1/AudioFile.h>

/**********************************
*
*	    Standard Libraries
*
**********************************/
#include <stdio.h>
#include <iostream>
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