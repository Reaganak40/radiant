#include "pch.h"
#include "Input.h"
#include "Utils.h"
#include "Engine/Graphics/Renderer.h"

namespace rdt {

    struct Input::Impl {
        float m_timestep;

        GLFWwindow* m_window;

        // the buffer index where new input is being added.
        // The last polled input will be m_current_state-1.
        int m_state_index;

        float m_timestamps[STATE_CACHE_SIZE];
        BitSet m_keyboard_state[STATE_CACHE_SIZE];
        MouseState m_mouse_state[STATE_CACHE_SIZE];
        WindowState m_window_state[STATE_CACHE_SIZE];
        bool m_mouse_changed;

        Impl()
            : m_window(nullptr), m_mouse_changed(false)
        {
            m_state_index = 0;
            m_timestep = 0;
            for (int i = 0; i < STATE_CACHE_SIZE; i++) {
                m_keyboard_state[i].SetNewFlagMax(NAIS);
                m_timestamps[i] = 0.0f;
            }
        }

        ~Impl()
        {
        }

        bool CheckState(unsigned int* stateQuery, size_t count, unsigned int target)
        {
            return m_keyboard_state[(m_state_index - target + STATE_CACHE_SIZE) % STATE_CACHE_SIZE].CheckFlags(stateQuery, count);
        }

        MouseState GetMouseState()
        {
            return m_mouse_state[(m_state_index - 1 + STATE_CACHE_SIZE) % STATE_CACHE_SIZE];
        }


        void PollInputs()
        {
            // InputStates follow this order
            // Key Press = offset 0
            // Key Down  = offset 1
            // Key Up    = offset 3

            unsigned int stateQuery[2];

            for (unsigned int stateCode = A_KEY_PRESS; stateCode < (NAIS - 2); stateCode += 3) {

                // if key has not been released
                if (!m_keyboard_state[m_state_index].CheckFlag(stateCode + 2)) {
                    stateQuery[0] = stateCode;
                    stateQuery[1] = stateCode + 1;

                    // if not a key press event
                    if (CheckState(stateQuery, 2, 1)) {

                        // add key down state
                        m_keyboard_state[m_state_index].ActivateFlag(stateCode + 1);
                    }
                }
            }

            m_timestamps[m_state_index] = m_timestep;

            // Go to the next buffer in cache arrays
            m_state_index = (m_state_index + 1) % STATE_CACHE_SIZE;
            m_keyboard_state[m_state_index].Clear();

            if (!m_mouse_changed) {
                m_mouse_state[m_state_index] = GetMouseState();
            }
            m_mouse_changed = false;

            m_window_state[m_state_index].windowResize = false;
        }

        bool CheckWindowResize()
        {
            return m_window_state[(m_state_index - 1 + STATE_CACHE_SIZE) % STATE_CACHE_SIZE].windowResize;
        }

        float GetTimeSinceKeyState(unsigned int* stateQuery, size_t count, const float maxTime)
        {
            int index = (m_state_index - 1 + STATE_CACHE_SIZE) % STATE_CACHE_SIZE;
            float current = 0.0f;
            float limit = Utils::Max(0, m_timestamps[index] - maxTime);

            do {
                if (m_keyboard_state[index].CheckFlags(stateQuery, count)) {
                    return current;
                }
                float timeLapsed = m_timestamps[index];
                index = (index - 1 + STATE_CACHE_SIZE) % STATE_CACHE_SIZE;
                current += timeLapsed - m_timestamps[index];
            } while (m_timestamps[index] > limit && index != m_state_index);

            return maxTime;
        }

        Vec2d GetMouseCoords(MouseCond cond) {
            MouseState ms = GetMouseState();

            if (cond == SCREEN_COORDS) {
                return ms.position;
            }

            ms.position.y = Renderer::GetWindowHeight() - ms.position.y;
            return ms.position + Renderer::GetCameraCoordinates2D();
        }
    };

    // =============================================================================

    Input* Input::m_instance = nullptr;

    Input::Input()
        : m_impl(new Input::Impl)
    {
       
    }

    Input::~Input()
    {
        delete m_impl;
    }


    void Input::Initialize()
    {
        Destroy();
        m_instance = new Input;
        
        m_instance->m_impl->m_window = (GLFWwindow*)Renderer::GetWindowInstance();
        glfwSetKeyCallback(m_instance->m_impl->m_window, Input::KeyCallback);
        glfwSetCursorPosCallback(m_instance->m_impl->m_window, Input::CursorPositionCallback);
        glfwSetMouseButtonCallback(m_instance->m_impl->m_window, Input::MouseButtonCallback);
        glfwSetWindowSizeCallback(m_instance->m_impl->m_window, Input::WindowSizeCallback);
    }

    void Input::Destroy()
    {
        if (m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
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
        else if (key == GLFW_KEY_SPACE) {
            flag = SPACE_KEY_PRESS;
        }
        else if (key == GLFW_KEY_LEFT_CONTROL) {
            flag = CTRL_KEY_PRESS;
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

        m_instance->m_impl->m_keyboard_state[m_instance->m_impl->m_state_index].ActivateFlag(flag);

    }

    void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        m_instance->m_impl->m_mouse_state[m_instance->m_impl->m_state_index].position.x = xpos;
        m_instance->m_impl->m_mouse_state[m_instance->m_impl->m_state_index].position.y = ypos;
        m_instance->m_impl->m_mouse_changed = true;
    }

    void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    }

    void Input::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        m_instance->m_impl->m_window_state[m_instance->m_impl->m_state_index].windowResize = true;
    }

    bool Input::CheckKeyboardState(const std::vector<InputState>& stateQuery, unsigned int targetFrame)
    {
        return m_instance->CheckStateImpl((unsigned int*)stateQuery.data(), stateQuery.size(), targetFrame);
    }

    void Input::UpdateTimeImpl(const float deltaTime)
    {
        m_impl->m_timestep += deltaTime;
    }

    void Input::PollInputsImpl()
    {
        m_impl->PollInputs();
    }

    bool Input::CheckStateImpl(unsigned int* stateQuery, size_t count, unsigned int target)
    {
        return m_impl->CheckState(stateQuery, count, target);
    }

    MouseState Input::GetMouseStateImpl()
    {
        return m_impl->GetMouseState();
    }

    bool Input::CheckWindowResizeImpl()
    {
        return m_impl->CheckWindowResize();
    }

    float Input::GetTimeSinceKeyStateImpl(unsigned int* stateQuery, size_t count, const float maxTime)
    {
        return m_impl->GetTimeSinceKeyState(stateQuery, count, maxTime);
    }

    Vec2d Input::GetMouseCoordsImpl(MouseCond cond) {
        return m_impl->GetMouseCoords(cond);
    }
}