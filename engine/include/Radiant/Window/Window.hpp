/***************************************************************/
/*  Window/Window.hpp                                          */
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
	class WindowConfig;
}
namespace rdt {

	// Defines the game window and provides an interface
	// to communicate with it.
	class RDT_WINDOW_API Window {
	public:

		// Returns the current instance's window configuration struct, which allows
		// the client to configure the window before launch
		virtual WindowConfig& GetWindowConfig() = 0;

		// Returns true if the window is launched and current running
		virtual bool IsOpen() = 0;

	private:
		friend class Application; // application handles the window internally

		// Creates a new instance of a Radiant window, but does not launch it.
		// There can only be one window instance at a time. If one is active
		// it will be deleted.
		static Window& Create();

		// Destroys the current window instance
		static void Destroy();

		// Gets the instantiated window
		static Window& GetInstance();

		// Launches the window instance under its current configurations
		virtual void LaunchWindow() = 0;
	};
}