/***************************************************************/
/*  (impl) Scene/SceneManager.hpp                              */
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
#include <Radiant/Scene/Scene.hpp>
#include <Radiant/Scene/Layer.hpp>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;
}

namespace rdt::scene {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

	public:
		static SceneManager& Get();
		static void Destroy();
		
		SceneID RegisterScene(const char* sceneName, Scene* scene);
		SceneID GetSceneID(const char* sceneName);
		Scene* SetScene(const char* sceneName);

		LayerID RegisterLayer(const char* layerName, Layer* nLayer);
		LayerID GetLayerID(const char* layerName);

		Layer* AttachLayerToScene(const char* layerName, SceneID scene);

	private:

		std::unordered_map<std::string, SceneID> sceneAliasToId;
		std::unordered_map<SceneID, Scene*> m_scenes;
		SceneID m_current_scene_id;

		std::unordered_map<std::string, LayerID> layerAliasToId;
		std::unordered_map<LayerID, Layer*> m_layers;

		SceneID sceneIdCounter;
		LayerID layerIdCounter;

		bool SceneExists(const std::string& sceneName);
		bool SceneExists(SceneID sceneID);

		bool LayerExists(const std::string& layerName);
		bool LayerExists(LayerID layer);
		
		/*
			Gets the next available sceneID
		*/
		SceneID NextSceneID();

		/*
			Gets the next available layerID
		*/
		LayerID NextLayerID();
	};
}