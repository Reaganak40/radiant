#pragma once
#include "pch.h"

class Renderer {
private:
	Renderer();
	~Renderer();
	static Renderer* m_instance;

	GLFWwindow* m_window;
	std::string m_window_name;
	unsigned int m_window_width;
	unsigned int m_window_height;

public:
	static Renderer* GetInstance();
	static void Initialize();
	static void Destroy();
	static void CreateWindow(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight) {
		m_instance->CreateWindowImpl(windowName, windowWidth, windowHeight);
	}

	static bool ShouldWindowClose() { return glfwWindowShouldClose(m_instance->m_window); }

	static void Draw() { m_instance->DrawImpl(); }

private:
	void CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight);
	void DrawImpl();



};
