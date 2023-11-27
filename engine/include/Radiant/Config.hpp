/***************************************************************/
/*  Config.hpp                                                 */
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
* Define the Radiant Version
***************************************************************/
#define RDT_VERSION_MAJOR 0
#define RDT_VERSION_MINOR 1
#define RDT_VERSION_PATCH 0
#define RDT_VERSION_IS_RELEASE false

/***************************************************************
* Define sysem macros
***************************************************************/
#ifdef RDT_PLATFORM_WINDOWS
	#define RDT_API_EXPORT __declspec(dllexport)
	#define RDT_API_IMPORT __declspec(dllimport)
#else
	#error Unsupported Platform - Windows Only
#endif

/***************************************************************
* Define the core platform library to use
***************************************************************/
#if defined(RDT_USE_DIRECTX)
	#error DirectX not currently available! Use OpenGL instead.
#elif defined(RDT_USE_OPENGL)
#else
	#pragma message("Graphics platform not defined... using OpenGL by default.") 
	#define RDT_USE_OPENGL
#endif
