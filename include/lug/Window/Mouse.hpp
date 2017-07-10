#pragma once

#include <lug/Window/Export.hpp>
#if defined(LUG_SYSTEM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
#elif defined(LUG_SYSTEM_LINUX)
    #include <X11/keysym.h>
#elif defined(LUG_SYSTEM_ANDROID)
    // TODO
#else
    // Theoretically this should never happen since the Config.cmake will
    // warn the user before, but #error anyway
    #error "Unsupported operating system or environment"
#endif

namespace lug {
namespace Window {
namespace Mouse {

/**
* @brief      Abstraction of Mouse buttons
*/
enum class LUG_WINDOW_API Button : int {
    Unknown,    // Unhandled button

    Left,       // The left mouse button
    Right,      // The right mouse button
    Middle,     // The middle (wheel) mouse button
    XButton1,   // The first extra mouse button
    XButton2    // The second extra mouse button
};

} // namespace Mouse
} // namespace Window
} // namespace lug
