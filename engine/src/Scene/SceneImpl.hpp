/***************************************************************/
/*  (impl) Scene/SceneImpl.hpp                                 */
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
* Forward Declarations
***************************************************************/
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;
	class Layer;
}

namespace rdt::scene {

	struct SceneImpl {
		SceneID m_ID = RDT_NULL_SCENE_ID;
		std::string m_name;

		std::vector<Layer*> m_layer_stack;
		bool is_bound = false;

		SceneImpl();
		~SceneImpl();

		inline void SetSceneID(SceneID nID) { m_ID = nID; }
		void SetSceneName(const char* nName);

		void RemoveFromLayerStack(LayerID lID);
		void AddToLayerStack(Layer* layer, size_t index);
		
		void BindLayer(const std::string& layerName);
		void BindLayer(size_t stackIndex);
		void BindLayers();
		
		void ReleaseLayer(const std::string& layerName);
		void ReleaseLayer(size_t stackIndex);
		void ReleaseLayers();

		void DetachAll();
	};
}