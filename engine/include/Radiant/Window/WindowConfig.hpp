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
	class Color;
	enum AspectRatio;
}

namespace rdt {

	class RDT_WINDOW_API WindowConfig {
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
}