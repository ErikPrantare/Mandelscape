/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANDELLANDSCAPE_GLFW_ENUMS_HPP
#define MANDELLANDSCAPE_GLFW_ENUMS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input {

enum class ButtonState {
    Release = GLFW_RELEASE,
    Press   = GLFW_PRESS,
    Repeat  = GLFW_REPEAT,
};

enum class HatState {
    Centered   = GLFW_HAT_CENTERED,
    Up         = GLFW_HAT_UP,
    Right      = GLFW_HAT_RIGHT,
    Down       = GLFW_HAT_DOWN,
    Left       = GLFW_HAT_LEFT,
    RightUp   = GLFW_HAT_RIGHT_UP,
    RightDown = GLFW_HAT_RIGHT_DOWN,
    LeftUp    = GLFW_HAT_LEFT_UP,
    LeftDown  = GLFW_HAT_LEFT_DOWN,
};

enum class Key {
    Unknown       = GLFW_KEY_UNKNOWN,
    Space         = GLFW_KEY_SPACE,
    Apostrophe    = GLFW_KEY_APOSTROPHE,
    Comma         = GLFW_KEY_COMMA,
    Minus         = GLFW_KEY_MINUS,
    Period        = GLFW_KEY_PERIOD,
    Slash         = GLFW_KEY_SLASH,
    Key0         = GLFW_KEY_0,
    Key1         = GLFW_KEY_1,
    Key2         = GLFW_KEY_2,
    Key3         = GLFW_KEY_3,
    Key4         = GLFW_KEY_4,
    Key5         = GLFW_KEY_5,
    Key6         = GLFW_KEY_6,
    Key7         = GLFW_KEY_7,
    Key8         = GLFW_KEY_8,
    Key9         = GLFW_KEY_9,
    Semicolon     = GLFW_KEY_SEMICOLON,
    Equal         = GLFW_KEY_EQUAL,
    A             = GLFW_KEY_A,
    B             = GLFW_KEY_B,
    C             = GLFW_KEY_C,
    D             = GLFW_KEY_D,
    E             = GLFW_KEY_E,
    F             = GLFW_KEY_F,
    G             = GLFW_KEY_G,
    H             = GLFW_KEY_H,
    I             = GLFW_KEY_I,
    J             = GLFW_KEY_J,
    K             = GLFW_KEY_K,
    L             = GLFW_KEY_L,
    M             = GLFW_KEY_M,
    N             = GLFW_KEY_N,
    O             = GLFW_KEY_O,
    P             = GLFW_KEY_P,
    Q             = GLFW_KEY_Q,
    R             = GLFW_KEY_R,
    S             = GLFW_KEY_S,
    T             = GLFW_KEY_T,
    U             = GLFW_KEY_U,
    V             = GLFW_KEY_V,
    W             = GLFW_KEY_W,
    X             = GLFW_KEY_X,
    Y             = GLFW_KEY_Y,
    Z             = GLFW_KEY_Z,
    LeftBracket  = GLFW_KEY_LEFT_BRACKET,
    Backslash     = GLFW_KEY_BACKSLASH,
    RightBracket = GLFW_KEY_RIGHT_BRACKET,
    GraveAccent  = GLFW_KEY_GRAVE_ACCENT,
    World1       = GLFW_KEY_WORLD_1,
    World2       = GLFW_KEY_WORLD_2,
    Escape        = GLFW_KEY_ESCAPE,
    Enter         = GLFW_KEY_ENTER,
    Tab           = GLFW_KEY_TAB,
    Backspace     = GLFW_KEY_BACKSPACE,
    Insert        = GLFW_KEY_INSERT,
    Delete       = GLFW_KEY_DELETE,
    Right         = GLFW_KEY_RIGHT,
    Left          = GLFW_KEY_LEFT,
    Down          = GLFW_KEY_DOWN,
    Up            = GLFW_KEY_UP,
    PageUp       = GLFW_KEY_PAGE_UP,
    PageDown     = GLFW_KEY_PAGE_DOWN,
    Home          = GLFW_KEY_HOME,
    End           = GLFW_KEY_END,
    CapsLock     = GLFW_KEY_CAPS_LOCK,
    ScrollLock   = GLFW_KEY_SCROLL_LOCK,
    NumLock      = GLFW_KEY_NUM_LOCK,
    PrintScreen  = GLFW_KEY_PRINT_SCREEN,
    Pause         = GLFW_KEY_PAUSE,
    F1            = GLFW_KEY_F1,
    F2            = GLFW_KEY_F2,
    F3            = GLFW_KEY_F3,
    F4            = GLFW_KEY_F4,
    F5            = GLFW_KEY_F5,
    F6            = GLFW_KEY_F6,
    F7            = GLFW_KEY_F7,
    F8            = GLFW_KEY_F8,
    F9            = GLFW_KEY_F9,
    F10           = GLFW_KEY_F10,
    F11           = GLFW_KEY_F11,
    F12           = GLFW_KEY_F12,
    F13           = GLFW_KEY_F13,
    F14           = GLFW_KEY_F14,
    F15           = GLFW_KEY_F15,
    F16           = GLFW_KEY_F16,
    F17           = GLFW_KEY_F17,
    F18           = GLFW_KEY_F18,
    F19           = GLFW_KEY_F19,
    F20           = GLFW_KEY_F20,
    F21           = GLFW_KEY_F21,
    F22           = GLFW_KEY_F22,
    F23           = GLFW_KEY_F23,
    F24           = GLFW_KEY_F24,
    F25           = GLFW_KEY_F25,
    Kp0          = GLFW_KEY_KP_0,
    Kp1          = GLFW_KEY_KP_1,
    Kp2          = GLFW_KEY_KP_2,
    Kp3          = GLFW_KEY_KP_3,
    Kp4          = GLFW_KEY_KP_4,
    Kp5          = GLFW_KEY_KP_5,
    Kp6          = GLFW_KEY_KP_6,
    Kp7          = GLFW_KEY_KP_7,
    Kp8          = GLFW_KEY_KP_8,
    Kp9          = GLFW_KEY_KP_9,
    KpDecimal    = GLFW_KEY_KP_DECIMAL,
    KpDivide     = GLFW_KEY_KP_DIVIDE,
    KpMultiply   = GLFW_KEY_KP_MULTIPLY,
    KpSubtract   = GLFW_KEY_KP_SUBTRACT,
    KpAdd        = GLFW_KEY_KP_ADD,
    KpEnter      = GLFW_KEY_KP_ENTER,
    KpEqual      = GLFW_KEY_KP_EQUAL,
    LeftShift    = GLFW_KEY_LEFT_SHIFT,
    LeftControl  = GLFW_KEY_LEFT_CONTROL,
    LeftAlt      = GLFW_KEY_LEFT_ALT,
    LeftSuper    = GLFW_KEY_LEFT_SUPER,
    RightShift   = GLFW_KEY_RIGHT_SHIFT,
    RightControl = GLFW_KEY_RIGHT_CONTROL,
    RightAlt     = GLFW_KEY_RIGHT_ALT,
    RightSuper   = GLFW_KEY_RIGHT_SUPER,
    Menu          = GLFW_KEY_MENU,
    Last          = GLFW_KEY_LAST,
};

enum class MouseButton {
    Button1 = GLFW_MOUSE_BUTTON_1,
    Button2 = GLFW_MOUSE_BUTTON_2,
    Button3 = GLFW_MOUSE_BUTTON_3,
    Button4 = GLFW_MOUSE_BUTTON_4,
    Button5 = GLFW_MOUSE_BUTTON_5,
    Button6 = GLFW_MOUSE_BUTTON_6,
    Button7 = GLFW_MOUSE_BUTTON_7,
    Button8 = GLFW_MOUSE_BUTTON_8,
    Last     = GLFW_MOUSE_BUTTON_LAST,
    Left     = GLFW_MOUSE_BUTTON_LEFT,
    Right    = GLFW_MOUSE_BUTTON_RIGHT,
    Middle   = GLFW_MOUSE_BUTTON_MIDDLE,
};

enum class Joystick {
    Joystick1  = GLFW_JOYSTICK_1,
    Joystick2  = GLFW_JOYSTICK_2,
    Joystick3  = GLFW_JOYSTICK_3,
    Joystick4  = GLFW_JOYSTICK_4,
    Joystick5  = GLFW_JOYSTICK_5,
    Joystick6  = GLFW_JOYSTICK_6,
    Joystick7  = GLFW_JOYSTICK_7,
    Joystick8  = GLFW_JOYSTICK_8,
    Joystick9  = GLFW_JOYSTICK_9,
    Joystick10 = GLFW_JOYSTICK_10,
    Joystick11 = GLFW_JOYSTICK_11,
    Joystick12 = GLFW_JOYSTICK_12,
    Joystick13 = GLFW_JOYSTICK_13,
    Joystick14 = GLFW_JOYSTICK_14,
    Joystick15 = GLFW_JOYSTICK_15,
    Joystick16 = GLFW_JOYSTICK_16,
    Last        = GLFW_JOYSTICK_LAST,
};

enum class GamepadButton {
    A            = GLFW_GAMEPAD_BUTTON_A,
    B            = GLFW_GAMEPAD_BUTTON_B,
    X            = GLFW_GAMEPAD_BUTTON_X,
    Y            = GLFW_GAMEPAD_BUTTON_Y,
    LeftBumper  = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    RightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    Back         = GLFW_GAMEPAD_BUTTON_BACK,
    Start        = GLFW_GAMEPAD_BUTTON_START,
    Guide        = GLFW_GAMEPAD_BUTTON_GUIDE,
    LeftThumb   = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    RightThumb  = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    DpadUp      = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    DpadRight   = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    DpadDown    = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    DpadLeft    = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
    Last         = GLFW_GAMEPAD_BUTTON_LAST,
    Cross        = GLFW_GAMEPAD_BUTTON_CROSS,
    Circle       = GLFW_GAMEPAD_BUTTON_CIRCLE,
    Square       = GLFW_GAMEPAD_BUTTON_SQUARE,
    Triangle     = GLFW_GAMEPAD_BUTTON_TRIANGLE,
};

enum class GamePadAxis {
    LeftX        = GLFW_GAMEPAD_AXIS_LEFT_X,
    LeftY        = GLFW_GAMEPAD_AXIS_LEFT_Y,
    RightX       = GLFW_GAMEPAD_AXIS_RIGHT_X,
    RightY       = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    LeftTrigger  = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    Last          = GLFW_GAMEPAD_AXIS_LAST,
};

enum class Mod {
    Shift     = GLFW_MOD_SHIFT,
    Control   = GLFW_MOD_CONTROL,
    Alt       = GLFW_MOD_ALT,
    Super     = GLFW_MOD_SUPER,
    CapsLock = GLFW_MOD_CAPS_LOCK,
    NumLock  = GLFW_MOD_NUM_LOCK,
};

}    // namespace Input

namespace glfwHintsAndAttrib {

enum class Error {
    NoError           = GLFW_NO_ERROR,
    NotInitialized     = GLFW_NOT_INITIALIZED,
    NoCurrentContext  = GLFW_NO_CURRENT_CONTEXT,
    InvalidEnum        = GLFW_INVALID_ENUM,
    InvalidValue       = GLFW_INVALID_VALUE,
    OutOfMemory       = GLFW_OUT_OF_MEMORY,
    ApiUnavailable     = GLFW_API_UNAVAILABLE,
    VersionUnavailable = GLFW_VERSION_UNAVAILABLE,
    PlatformError      = GLFW_PLATFORM_ERROR,
    FormatUnavailable  = GLFW_FORMAT_UNAVAILABLE,
};

enum class Window {
    NoWindowContext        = GLFW_NO_WINDOW_CONTEXT,
    Focused                  = GLFW_FOCUSED,
    Iconified                = GLFW_ICONIFIED,
    Resizable                = GLFW_RESIZABLE,
    Visible                  = GLFW_VISIBLE,
    Decorated                = GLFW_DECORATED,
    AutoIconify             = GLFW_AUTO_ICONIFY,
    Floating                 = GLFW_FLOATING,
    Maximized                = GLFW_MAXIMIZED,
    CenterCursor            = GLFW_CENTER_CURSOR,
    TransparentFramebuffer  = GLFW_TRANSPARENT_FRAMEBUFFER,
    Hovered                  = GLFW_HOVERED,
    FocusOnShow            = GLFW_FOCUS_ON_SHOW,
    ScaleToMonitor         = GLFW_SCALE_TO_MONITOR,
    CocoaRetinaFramebuffer = GLFW_COCOA_RETINA_FRAMEBUFFER,
    CocoaFrameName         = GLFW_COCOA_FRAME_NAME,
    CocoaGraphicsSwitching = GLFW_COCOA_GRAPHICS_SWITCHING,
    X11ClassName           = GLFW_X11_CLASS_NAME,
    X11InstanceName        = GLFW_X11_INSTANCE_NAME,
};

enum class BitDepth {
    RedBits         = GLFW_RED_BITS,
    GreenBits       = GLFW_GREEN_BITS,
    BlueBits        = GLFW_BLUE_BITS,
    AlphaBits       = GLFW_ALPHA_BITS,
    DepthBits       = GLFW_DEPTH_BITS,
    StencilBits     = GLFW_STENCIL_BITS,
    AccumRedBits   = GLFW_ACCUM_RED_BITS,
    AccumGreenBits = GLFW_ACCUM_GREEN_BITS,
    AccumBlueBits  = GLFW_ACCUM_BLUE_BITS,
    AccumAlphaBits = GLFW_ACCUM_ALPHA_BITS,
};

enum class Unique {
    AuxBuffers          = GLFW_AUX_BUFFERS,
    Stereo               = GLFW_STEREO,
    Samples              = GLFW_SAMPLES,
    SrgbCapable         = GLFW_SRGB_CAPABLE,
    RefreshRate         = GLFW_REFRESH_RATE,
    Doublebuffer         = GLFW_DOUBLEBUFFER,
    JoystickHatButtons = GLFW_JOYSTICK_HAT_BUTTONS,
};

enum class ClientAPI {
    ClientApi            = GLFW_CLIENT_API,
    ContextVersionMajor = GLFW_CONTEXT_VERSION_MAJOR,
    ContextVersionMinor = GLFW_CONTEXT_VERSION_MINOR,
    ContextRevision      = GLFW_CONTEXT_REVISION,
    ContextRobustness    = GLFW_CONTEXT_ROBUSTNESS,
};

enum class OpenGL {
    OpenglForwardCompat = GLFW_OPENGL_FORWARD_COMPAT,
    OpenglDebugContext  = GLFW_OPENGL_DEBUG_CONTEXT,
    OpenglProfile        = GLFW_OPENGL_PROFILE,
};

enum class Context {
    ContextReleaseBehavior = GLFW_CONTEXT_RELEASE_BEHAVIOR,
    ContextNoError         = GLFW_CONTEXT_NO_ERROR,
    ContextCreationApi     = GLFW_CONTEXT_CREATION_API,
};

enum class Unknown1 {
    NoApi        = GLFW_NO_API,
    OpenglApi    = GLFW_OPENGL_API,
    OpenglEsApi = GLFW_OPENGL_ES_API,
};

enum class Unknown2 {
    NoRobustness         = GLFW_NO_ROBUSTNESS,
    NoResetNotification = GLFW_NO_RESET_NOTIFICATION,
    LoseContextOnReset = GLFW_LOSE_CONTEXT_ON_RESET,
};

enum class Unknown3 {
    OpenglAnyProfile    = GLFW_OPENGL_ANY_PROFILE,
    OpenglCoreProfile   = GLFW_OPENGL_CORE_PROFILE,
    OpenglCompatProfile = GLFW_OPENGL_COMPAT_PROFILE,
};

enum class Unknown4 {
    Cursor               = GLFW_CURSOR,
    StickyKeys          = GLFW_STICKY_KEYS,
    StickyMouseButtons = GLFW_STICKY_MOUSE_BUTTONS,
    LockKeyMods        = GLFW_LOCK_KEY_MODS,
    RawMouseMotion     = GLFW_RAW_MOUSE_MOTION,
};

enum class Unknown5 {
    CursorNormal   = GLFW_CURSOR_NORMAL,
    CursorHidden   = GLFW_CURSOR_HIDDEN,
    CursorDisabled = GLFW_CURSOR_DISABLED,
};

enum class Unknown6 {
    AnyReleaseBehavior   = GLFW_ANY_RELEASE_BEHAVIOR,
    ReleaseBehaviorFlush = GLFW_RELEASE_BEHAVIOR_FLUSH,
    ReleaseBehaviorNone  = GLFW_RELEASE_BEHAVIOR_NONE,
};

enum class Unknown7 {
    NativeContextApi = GLFW_NATIVE_CONTEXT_API,
    EglContextApi    = GLFW_EGL_CONTEXT_API,
    OsmesaContextApi = GLFW_OSMESA_CONTEXT_API,
};

enum class CursorShape {
    ArrowCursor     = GLFW_ARROW_CURSOR,
    IbeamCursor     = GLFW_IBEAM_CURSOR,
    CrosshairCursor = GLFW_CROSSHAIR_CURSOR,
    HandCursor      = GLFW_HAND_CURSOR,
    HresizeCursor   = GLFW_HRESIZE_CURSOR,
    VresizeCursor   = GLFW_VRESIZE_CURSOR,
};

enum class Connection {
    Connected    = GLFW_CONNECTED,
    Disconnected = GLFW_DISCONNECTED,
};

enum class MacOS {
    CocoaChdirResources = GLFW_COCOA_CHDIR_RESOURCES,
    CocoaMenubar         = GLFW_COCOA_MENUBAR,
};

}    // namespace glfwHintsAndAttrib

#endif    // MANDELLANDSCAPE_GLFW_ENUMS_HPP
