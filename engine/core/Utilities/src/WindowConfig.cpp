#include "pch.h"
#include "WindowConfig.h"

#include "Color.h"
#include "Common.h"

// ================================================
// Window Config Implementation
// ================================================
namespace rdt {
	class WindowConfigImpl : public rdt::WindowConfig {
		std::string windowName = "Radiant App";
		unsigned int m_windowWidth = 1280;
		unsigned int m_windowHeight = 720;
		AspectRatio m_aspectRatio = NoAspectRatio;
		Color m_backgroundColor = WHITE;
		bool m_should_maximize_window = false;

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

		void SetBackgroundColor(const Color& color) override final {
			m_backgroundColor = color;
		}
		Color GetBackgroundColor() const override final {
			return m_backgroundColor;
		}

		void SetShouldMaximizeWindow(bool shouldMaximize) override final {
			m_should_maximize_window = shouldMaximize;
		}
		bool ShouldMaximizeWindow() const override final {
			return m_should_maximize_window;
		}
	};
}

// ================================================
std::shared_ptr<rdt::WindowConfig> rdt::CreateWindowConfig()
{
	return std::make_shared<WindowConfigImpl>();
}
