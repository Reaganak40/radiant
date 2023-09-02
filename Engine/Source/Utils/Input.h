#pragma once
#include "pch.h"
#include "MathTypes.h"

#define A_KEY_PRESS 1
#define A_KEY_DOWN 2
#define A_KEY_UP 4
#define S_KEY_PRESS 8
#define S_KEY_DOWN 16
#define S_KEY_UP 32
#define D_KEY_PRESS 64
#define D_KEY_DOWN 128
#define D_KEY_UP 256
#define W_KEY_PRESS 512
#define W_KEY_DOWN 1024
#define W_KEY_UP 2048
#define E_KEY_PRESS 4096
#define E_KEY_DOWN 8192
#define E_KEY_UP 16384

namespace Radiant {

    enum KeyState {
        KeyPressed,
        KeyDown,
        KeyUp,
    };

    enum KeyCode {
        A_KEYCODE,
        S_KEYCODE,
        D_KEYCODE,
        W_KEYCODE,
        E_KEYCODE,
    };

    struct InputState {
        unsigned int state;

        InputState()
            : state(0) {}

        bool CheckState(unsigned int query) {
            return (bool)(state & query);
        }
        void AddState(unsigned int nState) {
            state |= nState;
        }
        void ClearState() {
            state = 0;
        }
    };

    struct MouseState {
        Vec3f position;

        MouseState()
            : position(Vec3f()) {}
    };

    struct WindowState {
        bool windowResize;

        WindowState()
            : windowResize(false) {}
    };

    class Input {
    private:
        Input();
        ~Input();
        static Input* m_singleton;

        GLFWwindow* m_window;

        InputState m_keyboard_state[2];
        MouseState m_mouse_state[2];
        WindowState m_window_state[2];
        int m_current_state;
        bool m_mouse_changed;

    public:
        static Input* GetInstance() {
            if (m_singleton == nullptr) {
                m_singleton = new Input;
            }
            return m_singleton;
        }

        /* Sets up the input handling with the given glfw window. */
        static void Initialize();
        static void Destroy();

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);

        static void PollInputs() { m_singleton->PollInputsImpl(); }

        /* Get all trigged events by their assigned ListenerIDs. */
        static bool CheckKeyboardState(unsigned int stateCode) { return m_singleton->CheckStateImpl(stateCode); }

        static inline MouseState& GetMouseState() { return m_singleton->GetMouseStateImpl(); }

        static bool CheckWindowResize() { return m_singleton->CheckWindowResizeImpl(); }

    private:
        void PollInputsImpl();
        bool CheckStateImpl(unsigned int stateCode);
        MouseState& GetMouseStateImpl();
        bool CheckWindowResizeImpl();

    };

}