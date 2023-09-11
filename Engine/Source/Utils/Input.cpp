#include "pch.h"
#include "Input.h"
#include "Renderer/Renderer.h"

namespace rdt {
    Input* Input::m_singleton = nullptr;

    Input::Input()
        : m_window(nullptr), m_mouse_changed(false),
        m_keyboard_state{BitSet(InputState::NAIS), BitSet(InputState::NAIS)}
    {
        m_current_state = 0;
    }

    Input::~Input()
    {
    }


    void Input::Initialize()
    {
        Destroy();
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
        int flag = 0;

        // InputStates follow this order
        // Key Press = offset 0
        // Key Down  = offset 1
        // Key Up    = offset 3

        if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
            flag = (key - GLFW_KEY_A) * 3;
        }
        else if (key >= GLFW_KEY_RIGHT && key <= GLFW_KEY_UP) {
            flag = RIGHT_KEY_PRESS + (key - GLFW_KEY_RIGHT) * 3;
        }


        switch (action) {
        case GLFW_PRESS:
            break;
        case GLFW_RELEASE:
            flag += 2; 
            break;
        case GLFW_REPEAT:
            // Does not currently support text input (typing)
            return;
        default:
            printf("Warning: Unidentified action: %d\n", action);
            return;
        }

        m_singleton->m_keyboard_state[m_singleton->m_current_state].ActivateFlag(flag);

    }

    void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        m_singleton->m_mouse_state[m_singleton->m_current_state].position.x = xpos;
        m_singleton->m_mouse_state[m_singleton->m_current_state].position.y = ypos;
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

    bool Input::CheckKeyboardState(const std::vector<InputState>& stateQuery)
    {
        return m_singleton->CheckStateImpl((unsigned int*)stateQuery.data(), stateQuery.size());
    }

    void Input::PollInputsImpl()
    {
        // InputStates follow this order
        // Key Press = offset 0
        // Key Down  = offset 1
        // Key Up    = offset 3

        unsigned int stateQuery[2];

        for (unsigned int stateCode = A_KEY_PRESS; stateCode < (NAIS-2); stateCode += 3) {

            // if key has not been released
            if (!m_keyboard_state[m_current_state].CheckFlag(stateCode + 2)) {
                stateQuery[0] = stateCode;
                stateQuery[1] = stateCode + 1;

                // if not a key press event
                if (CheckStateImpl(stateQuery, 2)) {
                    
                    // add key down state
                    m_keyboard_state[m_current_state].ActivateFlag(stateCode + 1);
                }
            }
        }

        m_current_state = (m_current_state == 0 ? 1 : 0);
        m_keyboard_state[m_current_state].Clear();

        if (!m_mouse_changed) {
            m_mouse_state[m_current_state] = GetMouseStateImpl();
        }
        m_mouse_changed = false;

        m_window_state[m_current_state].windowResize = false;

    }

    bool Input::CheckStateImpl(unsigned int* stateQuery, unsigned int count)
    {
        return m_keyboard_state[(m_current_state == 0 ? 1 : 0)].CheckFlags(stateQuery, count);
    }

    MouseState Input::GetMouseStateImpl()
    {
        return m_mouse_state[(m_current_state == 0 ? 1 : 0)];
    }

    bool Input::CheckWindowResizeImpl()
    {
        return m_window_state[(m_current_state == 0 ? 1 : 0)].windowResize;
    }

    Vec2d Input::GetMouseCoordsImpl(MouseCond cond) {
        MouseState ms = GetMouseStateImpl();

        if (cond == SCREEN_COORDS) {
            return ms.position;
        }

        ms.position.y = Renderer::GetWindowHeight() - ms.position.y;
        return ms.position + Renderer::GetCameraCoordinates2D();
    }

}