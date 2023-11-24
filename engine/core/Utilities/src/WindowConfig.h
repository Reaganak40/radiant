/*******************************************************************
*	File: WindowConfig.h
*	Module: Utils
*
*	Window config describes the specifications of a window from a
*	high-level. This is meant to provide a data component that
*	can be seen by any module that needs it (including the client),
*	while remaining seprated by the internal implementation that
*	utilizes the Window Config
*******************************************************************/
#pragma once
#include "utils_api.h"

// Forward Declarations
namespace rdt {
	struct Vec2i;
	class Color;
}

namespace rdt {
	enum AspectRatio {
		NoAspectRatio,  // No aspect ratio is maintained (stretch)
		AR_16_9,		// Window Projection = (1920px, 1080px)
		AR_1_1,			// Window Projection = (1000px, 1000px)
	};

	class UTILS_API WindowConfig {
	public:
		virtual ~WindowConfig() {}

		virtual void SetWindowName(const char* name) = 0;
		virtual const char* GetWindowName() const = 0;
		
		virtual void SetWindowWidth(unsigned int windowWidth) = 0;
		virtual void SetWindowHeight(unsigned int windowHeight) = 0;
		virtual void SetWindowSize(unsigned int windowWidth, unsigned int windowHeight) = 0;
		virtual Vec2i GetWindowSize() const = 0;

		virtual void SetAspectRatio(AspectRatio aspectRatio) = 0;
		virtual AspectRatio GetAspectRatio() const = 0;

		virtual void SetBackgroundColor(const Color& color) = 0;
		virtual Color GetBackgroundColor() const = 0;

		virtual void SetShouldMaximizeWindow(bool shouldMaximize = true) = 0;
		virtual bool ShouldMaximizeWindow() const = 0;
	};

	UTILS_API std::shared_ptr<WindowConfig> CreateWindowConfig();
}