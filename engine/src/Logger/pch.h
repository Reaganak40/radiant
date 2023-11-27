/***************************************************************
* Logger Module Precompiled Header
***************************************************************/
#pragma once

// Logging
#include <CodeAnalysis/Warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

#include <memory>
