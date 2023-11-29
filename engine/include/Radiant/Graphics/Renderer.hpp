/***************************************************************/
/*  Graphics/Renderer.hpp                                      */
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
#include <Radiant/Graphics/Export.hpp>

/***************************************************************
* Foward Declarations
***************************************************************/
namespace rdt {
	using ModelID = unsigned int;
	using TextureID = unsigned int;

	struct Transform;
	struct AtlasProfile;

	class Color;
	class Camera;
}

namespace rdt {

	// Renderer is a global pipeline tool that is used by the client
	// to render meshes, manipulate the camera, and draw scenes.
	class RDT_GRAPHICS_API Renderer {
	public:
		// Sets the color that is used to clear the screen each frame.
		// default: BLACK
		static void SetBackgroundColor(const Color& color);

		//Starts a new render command context, defining a new mesh.
		// \param layer - 2D render layer that is new mesh will be draw to.
		static void Begin(unsigned int layer = 0);

		// Ends the current render command context, creating
		// a mesh through the commands between Begin() and this
		// End(), and adds the new mesh to the draw queue.
		static void End();

		// Sets the model that the current mesh will use, referencing
		// it by ID.
		// \param modelID - the registered identifier for the model.
		static void SetModel(ModelID modelID);

		// Sets the transform to be applied to the target model.
		static void SetTransform(const Transform& transform);

		// Sets the texture to be applied to the mesh's model.
		// \param texture - the global identifier of a registered texture
		static void SetTexture(TextureID texture);

		// Sets the atlas profile to be applied to the texture.
		static void SetAtlasProfile(const AtlasProfile& profile);

		// Horizontally flips the texture when drawn
		static void FlipTextureHorizontal();

		// Sets the color the fills in the mesh's model.
		static void SetFillColor(const Color& color);

	private:
		friend class Application;
		static void Initialize();
		static void Destroy();
		static void OnBeginFrame();
		static void Render();
		static void OnEndFrame();
	};
}