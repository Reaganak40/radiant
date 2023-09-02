#include "pch.h"
#include "Input.h"
#include "Renderer/Renderer.h"

namespace Radiant {
    Input* Input::m_singleton = nullptr;

    Input::Input()
        : m_window(nullptr), m_mouse_changed(false)
    {
        for (int i = 0; i < 2; i++) {
            m_keyboard_state[i].state = 0;
        }
        m_current_state = 0;
    }

    Input::~Input()
    {
    }


    void Input::Initialize()
    {
        if (m_singleton != nullptr) {
            delete m_singleton;
            m_singleton = nullptr;
        }
        GetInstance();
        m_singleton->m_window = Renderer::GetInstance()->m_window;

        glfwSetKeyCallback(m_singleton->m_window, Input::KeyCallback);
        glfwSetCursorPosCallback(m_singleton->m_window, Input::CursorPositionCallback);
        glfwSetMouseButtonCallback(m_singleton->m_window, Input::MouseButtonCallback);
        glfwSetWindowSizeCallback(m_singleton->m_window, Input::WindowSizeCallback);
    }

    void Input::Destroy()
    {
        if (m_singleton != nullptr) {
            delete m_singleton;
            m_singleton = nullptr;
        }
    }

    void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        int change = 0;
        switch (key) {
        case GLFW_KEY_A:
            change = A_KEY_PRESS;
            break;
        case GLFW_KEY_S:
            change = S_KEY_PRESS;
            break;
        case GLFW_KEY_D:
            change = D_KEY_PRESS;
            break;
        case GLFW_KEY_W:
            change = W_KEY_PRESS;
            break;
        case GLFW_KEY_E:
            change = E_KEY_PRESS;
            break;
        }

        switch (action) {
        case GLFW_PRESS:
            break;
        case GLFW_RELEASE:
            change <<= 2; // bitshift left by 2 to set to KEY_UP
            break;
        case GLFW_REPEAT:
            // Does not currently support text input (typing)
            return;
        default:
            printf("Warning: Unidentified action: %d\n", action);
            return;
        }

        m_singleton->m_keyboard_state[m_singleton->m_current_state].AddState(change);
    }

    void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        m_singleton->m_mouse_state[m_singleton->m_current_state].position.x = (float)xpos;
        m_singleton->m_mouse_state[m_singleton->m_current_state].position.y = (float)ypos;
        m_singleton->m_mouse_changed = true;
    }

    void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    }

    void Input::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        m_singleton->m_window_state[m_singleton->m_current_state].windowResize = true;
    }

    void Input::PollInputsImpl()
    {

        for (unsigned int stateCode = A_KEY_PRESS; stateCode <= E_KEY_UP; stateCode <<= 3) {

            // if key has not been released
            if (!m_keyboard_state[m_current_state].CheckState(stateCode << 2)) {
                // if not a key press event
                if (CheckStateImpl(stateCode | (stateCode << 1))) {
                    // add key down state
                    m_keyboard_state[m_current_state].AddState(stateCode << 1);
                }
            }
        }

        m_current_state = (m_current_state == 0 ? 1 : 0);
        m_keyboard_state[m_current_state].ClearState();

        if (!m_mouse_changed) {
            m_mouse_state[m_current_state] = GetMouseStateImpl();
        }
        m_mouse_changed = false;

        m_window_state[m_current_state].windowResize = false;

    }

    bool Input::CheckStateImpl(unsigned int stateCode)
    {
        return m_keyboard_state[(m_current_state == 0 ? 1 : 0)].CheckState(stateCode);
    }

    MouseState& Input::GetMouseStateImpl()
    {
        return m_mouse_state[(m_current_state == 0 ? 1 : 0)];
    }

    bool Input::CheckWindowResizeImpl()
    {
        return m_window_state[(m_current_state == 0 ? 1 : 0)].windowResize;
    }

}