/*******************************************************************
*	File:    State.h
*	Module:  Internal
*
*	Author: Reagan Kelley
*
*	This file contains glState, the root coordinator that initalizes
*	new OpenGL applications, the OpenGL library, and the termination
*	of each respectively.
*******************************************************************/
#pragma once
#include "Application.h"
#include "Texture/TextureManager.h"

namespace rdt::glCore {

	class glState {
	private:
		glState();
		~glState();
		static glState* m_singleton;
	public:

		/*
			Gets the singleton glState instance
		*/
		static glState* Get();

		/*
			Deletes the singleton glState instance
		*/
		void Reset();

		/*
			Adds an application to the state
		*/
		glApplication* RegisterApplication(const std::string& name);

		/*
			Gets a registered application instance, or creates it
			if it does not exist.
		*/
		glApplication* GetApplication(const std::string& name);

		/*
			Returns true if an application under the given name exists.
		*/
		bool ApplicationExists(const std::string& name);

		/*
			Unregisters an application under the given name, freeing it, returns
			true if successful
		*/
		bool RemoveApplication(const std::string& name);

		/*
			Returns the number of registered applications
		*/
		size_t ApplicationCount();

		/*
			Returns the OpenGL state's texture manager
		*/
		TextureManager& GetTextureManager();

	private:
		std::unordered_map<std::string, glApplication*> m_applications;
	};
}