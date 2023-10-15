#pragma once
#include "Core.h"
#include "MathTypes.h"
#include "BitSet.h"


namespace rdt {

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

        SPACE_KEY_PRESS,
        SPACE_KEY_DOWN,
        SPACE_KEY_UP,

        CTRL_KEY_PRESS,
        CTRL_KEY_DOWN,
        CTRL_KEY_UP,

        ZERO_KEY_PRESS,
        ZERO_KEY_DOWN,
        ZERO_KEY_UP,

        ONE_KEY_PRESS,
        ONE_KEY_DOWN,
        ONE_KEY_UP,

        TWO_KEY_PRESS,
        TWO_KEY_DOWN,
        TWO_KEY_UP,

        THREE_KEY_PRESS,
        THREE_KEY_DOWN,
        THREE_KEY_UP,

        FOUR_KEY_PRESS,
        FOUR_KEY_DOWN,
        FOUR_KEY_UP,

        FIVE_KEY_PRESS,
        FIVE_KEY_DOWN,
        FIVE_KEY_UP,

        SIX_KEY_PRESS,
        SIX_KEY_DOWN,
        SIX_KEY_UP,

        SEVEN_KEY_PRESS,
        SEVEN_KEY_DOWN,
        SEVEN_KEY_UP,

        EIGHT_KEY_PRESS,
        EIGHT_KEY_DOWN,
        EIGHT_KEY_UP,

        NINE_KEY_PRESS,
        NINE_KEY_DOWN,
        NINE_KEY_UP,

        NAIS // Not an input state
    };



    enum KeyState {
        KeyPressed,
        KeyDown,
        KeyUp,
    };

    struct MouseState {
        Vec2d position;
        bool mouse_moved = false;

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

// 120 frames = ~2 seconds of stored input
#define STATE_CACHE_SIZE 120

    class RADIANT_API Input {
    private:
        struct Impl;
        Impl* m_impl;

        Input();
        ~Input();
        static Input* m_instance;

    public:
        /* Sets up the input handling with the given glfw window. */
        static void Initialize();
        static void Destroy();


        /**********************************************
        * 
        *           Input Callback Functions
        * 
        ***********************************************/
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);

        /*
            Updates the inner clock of the input singleton, which is used
            to cache time-sensitive input data.
        */
        static void UpdateTime(const float deltaTime) { m_instance->UpdateTimeImpl(deltaTime); }

        /*
            Marks the end of input gathering for the current frame, and stored it to be
            queried in the next update cylce.
        */
        static void PollInputs() { m_instance->PollInputsImpl(); }

        /*
            Returns true if any of keystate in the input query is true.
            The target frame indicates how many frames ago to check for input, starting at 1 (current-frame), up to 5 frames ago.
        */
        static bool CheckKeyboardState(const std::vector<InputState>& stateQuery, unsigned int targetFrame = 1);

        static inline MouseState GetMouseState() { return m_instance->GetMouseStateImpl(); }

        /*
            Returns the xy coordinates of the mouse either in screen coordinates (relative to the window),
            or world coordnates (relative to the camera position).
        */
        static Vec2d GetMouseCoords(const MouseCond cond) { return m_instance->GetMouseCoordsImpl(cond); }

        static bool CheckWindowResize() { return m_instance->CheckWindowResizeImpl(); }

        static float GetTimeSinceKeyState(const std::vector<InputState>& stateQuery, const float maxTime = 1.0f) {
            return m_instance->GetTimeSinceKeyStateImpl((unsigned int*)stateQuery.data(), stateQuery.size(), maxTime);
        }

        /*
            Sets the render window that shoud be used to offset the mouse coordinates. By default
            is -1, which means to use the default viewport.
        */
        static void SetTargetRenderWindow(int renderWindowIndex);

    private:
        void UpdateTimeImpl(const float deltaTime);
        void PollInputsImpl();
        bool CheckStateImpl(unsigned int* stateQuery, size_t count, unsigned int target);
        MouseState GetMouseStateImpl();
        bool CheckWindowResizeImpl();

        float GetTimeSinceKeyStateImpl(unsigned int* stateQuery, size_t count, const float maxTime);
        Vec2d GetMouseCoordsImpl(const MouseCond cond);

    };

}