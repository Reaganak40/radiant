/*******************************************************************
*	File:    Logger.h
*	Module:  Logger
*
*	Author: Reagan Kelley
*
*	Logging dll library utilizing spdlog
*******************************************************************/
#pragma once

#include "../logger_api.h"
#include "../../src/Log.h"

#ifdef APIENTRY
	#undef APIENTRY
#endif
// include declarations of spdlog
#include "spdlog/spdlog.h"

