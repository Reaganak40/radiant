/***************************************************************/
/*  Application/Application.hpp                                */
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
#include <Radiant/Application/Export.hpp>

/***************************************************************
* Forward Delcarations
***************************************************************/
namespace rdt {
	class Scene;
	class WindowConfig;
}

namespace rdt {

	// Serves as the central module that defines child applications
	// and controls the flow of the program. Application is
	// reponsible for launching windows, handling scenes between
	// the client and the back-end, and running the game loop.
	class RDT_APP_API Application
	{
	private:
		struct Impl;
		static Impl* m_impl;

	public:
		Application();
		virtual ~Application();

		// Runs and manages the game loop, and will continue to run until the application is closed.
		void Run();

	protected:
		
		// Returns the application's window configuration struct, which can be
		// edited before OnStart to configurate the game window.
		WindowConfig& GetWindowConfig();

		// Called right before the game loop begins to define and setup scenes.
		virtual void OnGameBegin() {}

		// Sets the starting scene that is called at the beginning of the
		// game. This function should be called in the body of OnGameBegin().
		void BeginScene(const char* sceneName);

	private:
	};

	// To be implemented by client, this function is used by the entry
	// point to create a new instance of a Radiant application. Should
	// return a child implementation of Application.
	Application* CreateApplication();
}