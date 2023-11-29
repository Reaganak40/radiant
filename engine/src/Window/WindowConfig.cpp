#include "pch.h"
#include <Radiant/Window/WindowConfig.hpp>
#include <Radiant/Window/AspectRatio.hpp>
#include <Radiant/System/MathTypes.hpp>

// ================================================
// Window Config Implementation
// ================================================
namespace rdt {
	class WindowConfigImpl : public rdt::WindowConfig {
		
		WindowConfigFlags m_flags = WindowConfigFlags_Default;
		std::string windowName = "Radiant App";
		unsigned int m_windowWidth = 1280;
		unsigned int m_windowHeight = 720;
		AspectRatio m_aspectRatio = NoAspectRatio;

		WindowConfigFlags& GetConfigFlags() override final {
			return m_flags;
		}

		void SetWindowName(const char* name) override final {
			windowName = name;
		}
		const char* GetWindowName() const override final {
			return windowName.c_str();
		}

		void SetWindowWidth(unsigned int windowWidth) override final {
			m_windowWidth = windowWidth;
		}
		void SetWindowHeight(unsigned int windowHeight) override final {
			m_windowHeight = windowHeight;
		}
		void SetWindowSize(unsigned int windowWidth, unsigned int windowHeight) override final {
			m_windowWidth = windowWidth;
			m_windowHeight = windowHeight;
		}

		Vec2i GetWindowSize() const override final {
			return Vec2i(m_windowWidth, m_windowHeight);
		}

		void SetAspectRatio(AspectRatio aspectRatio) override final {
			m_aspectRatio = aspectRatio;
		}
		AspectRatio GetAspectRatio() const override final {
			return m_aspectRatio;
		}
	};
}