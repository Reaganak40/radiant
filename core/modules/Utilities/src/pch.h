#pragma once

#define _USE_MATH_DEFINES
#include <cmath>


#ifdef RDT_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <type_traits>