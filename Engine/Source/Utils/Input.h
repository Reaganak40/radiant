#pragma once
#include "pch.h"
#include "MathTypes.h"
#include "BitSet.h"


namespace Radiant {

    enum InputState {

        A_KEY_PRESS,
        A_KEY_DOWN,
        A_KEY_UP,

        B_KEY_PRESS,
        B_KEY_DOWN,
        B_KEY_UP,

        C_KEY_PRESS,
        C_KEY_DOWN,
        C_KEY_UP,

        D_KEY_PRESS,
        D_KEY_DOWN,
        D_KEY_UP,

        E_KEY_PRESS,
        E_KEY_DOWN,
        E_KEY_UP,

        F_KEY_PRESS,
        F_KEY_DOWN,
        F_KEY_UP,

        G_KEY_PRESS,
        G_KEY_DOWN,
        G_KEY_UP,

        H_KEY_PRESS,
        H_KEY_DOWN,
        H_KEY_UP,

        I_KEY_PRESS,
        I_KEY_DOWN,
        I_KEY_UP,

        J_KEY_PRESS,
        J_KEY_DOWN,
        J_KEY_UP,

        K_KEY_PRESS,
        K_KEY_DOWN,
        K_KEY_UP,

        L_KEY_PRESS,
        L_KEY_DOWN,
        L_KEY_UP,

        M_KEY_PRESS,
        M_KEY_DOWN,
        M_KEY_UP,

        N_KEY_PRESS,
        N_KEY_DOWN,
        N_KEY_UP,

        O_KEY_PRESS,
        O_KEY_DOWN,
        O_KEY_UP,

        P_KEY_PRESS,
        P_KEY_DOWN,
        P_KEY_UP,

        Q_KEY_PRESS,
        Q_KEY_DOWN,
        Q_KEY_UP,

        R_KEY_PRESS,
        R_KEY_DOWN,
        R_KEY_UP,

        S_KEY_PRESS,
        S_KEY_DOWN,
        S_KEY_UP,

        T_KEY_PRESS,
        T_KEY_DOWN,
        T_KEY_UP,

        U_KEY_PRESS,
        U_KEY_DOWN,
        U_KEY_UP,

        V_KEY_PRESS,
        V_KEY_DOWN,
        V_KEY_UP,

        W_KEY_PRESS,
        W_KEY_DOWN,
        W_KEY_UP,

        X_KEY_PRESS,
        X_KEY_DOWN,
        X_KEY_UP,

        Y_KEY_PRESS,
        Y_KEY_DOWN,
        Y_KEY_UP,

        Z_KEY_PRESS,
        Z_KEY_DOWN,
        Z_KEY_UP,

        RIGHT_KEY_PRESS,
        RIGHT_KEY_DOWN,
        RIGHT_KEY_UP,

        LEFT_KEY_PRESS,
        LEFT_KEY_DOWN,
        LEFT_KEY_UP,

        DOWN_KEY_PRESS,
        DOWN_KEY_DOWN,
        DOWN_KEY_UP,

        UP_KEY_PRESS,
        UP_KEY_DOWN,
        UP_KEY_UP,


        NAIS // Not an input state
    };



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

    struct MouseState {
        Vec2d position;

        MouseState()
            : position(Vec2d(0, 0)) {}
    };

    struct WindowState {
        bool windowResize;

        WindowState()
            : windowResize(false) {}
    };

    enum MouseCond {
        SCREEN_COORDS,
        WORLD_COORDS
    };

    class Input {
    private:
        Input();
        ~Input();
        static Input* m_singleton;

        GLFWwindow* m_window;
        BitSet m_keyboard_state[2];
        int m_current_state;

        MouseState m_mouse_state[2];
        WindowState m_window_state[2];
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
        static bool CheckKeyboardState(const std::vector<InputState>& stateQuery);

        static inline MouseState GetMouseState() { return m_singleton->GetMouseStateImpl(); }

        static Vec2d GetMouseCoords(const MouseCond cond) { return m_singleton->GetMouseCoordsImpl(cond); }

        static bool CheckWindowResize() { return m_singleton->CheckWindowResizeImpl(); }

    private:
        void PollInputsImpl();
        bool CheckStateImpl(unsigned int* stateQuery, unsigned int count);
        MouseState GetMouseStateImpl();
        bool CheckWindowResizeImpl();

        Vec2d GetMouseCoordsImpl(const MouseCond cond);

    };

}