/***************************************************************/
/*  Window/WindowConfig.hpp                                    */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include <Radiant/Window/Export.hpp>

/***************************************************************
* Forward Delcarations
***************************************************************/
namespace rdt {
	struct Vec2i;
	enum AspectRatio;
}

namespace rdt {

	enum WindowConfigFlags {
		WindowConfigFlags_None = 0,
		WindowConfigFlags_Fullscreen	 = 1 << 0, // Sets the window to fullscreen, encapsulating the entire monitor
		WindowConfigFlags_MaximizeWindow = 1 << 1, // When the window is launched, it will be maximized by default

		WindowConfigFlags_Default = WindowConfigFlags_None,
	};

	class RDT_WINDOW_API WindowConfig {
	public:
		virtual ~WindowConfig() {}

		// Returns a reference to the config flags that can be changed to
		// change the behavior of the window
		virtual WindowConfigFlags& GetConfigFlags() = 0;

		virtual void SetWindowName(const char* name) = 0;
		virtual const char* GetWindowName() const = 0;
		
		// Sets the window's width at launch in pixels 
		virtual void SetWindowWidth(unsigned int windowWidth) = 0;

		// Sets the window's height at launch in pixels 
		virtual void SetWindowHeight(unsigned int windowHeight) = 0;
		virtual void SetWindowSize(unsigned int windowWidth, unsigned int windowHeight) = 0;
		virtual Vec2i GetWindowSize() const = 0;

		// Sets the aspect ratio that is maintained by the camera viewport
		virtual void SetAspectRatio(AspectRatio aspectRatio) = 0;
		virtual AspectRatio GetAspectRatio() const = 0;
	};
}