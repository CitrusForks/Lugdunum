#include <lug/Window/Window.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

#include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

#include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

#include <lug/Window/Android/WindowImplAndroid.hpp>

#else

// Theorically this should never happen since the Config.cmake will
// warn the user before, but #error anyway
#error "Unsupported operating system or environment"

#endif

namespace lug {
namespace Window {

Window::Window() : _impl{ new priv::WindowImpl(this) } {
    initKeyState();
}

Window::Window(Window&& rhs) : _impl{rhs._impl} {
    rhs._impl = nullptr;
    _keyState = std::move(rhs._keyState);
}

Window& Window::operator=(Window&& rhs) {
    _impl = rhs._impl;
    rhs._impl = nullptr;
    _keyState = std::move(rhs._keyState);

    return *this;
}

Window::~Window() {
    close();
}

std::unique_ptr<Window> Window::create(uint16_t width, uint16_t height, const std::string& title, Style style) {
    std::unique_ptr<Window> win(new Window());

    if (!win->createWindow(width, height, title, style)) {
        return nullptr;
    }

    return win;
}

bool Window::createWindow(uint16_t width, uint16_t height, const std::string& title, Style style) {
    if (_impl != nullptr) {

        // Specifiy the width and height of our window, for now it's the only thing we can specify along with the title and style that is
        _mode.width = width;
        _mode.height = height;

        return _impl->create(title, style);
    }

    return false;
}

void Window::initKeyState() {
    // Basic keys
    _keyState[Keyboard::Key::A] = false;
    _keyState[Keyboard::Key::B] = false;
    _keyState[Keyboard::Key::C] = false;
    _keyState[Keyboard::Key::D] = false;
    _keyState[Keyboard::Key::E] = false;
    _keyState[Keyboard::Key::F] = false;
    _keyState[Keyboard::Key::G] = false;
    _keyState[Keyboard::Key::H] = false;
    _keyState[Keyboard::Key::I] = false;
    _keyState[Keyboard::Key::J] = false;
    _keyState[Keyboard::Key::K] = false;
    _keyState[Keyboard::Key::L] = false;
    _keyState[Keyboard::Key::M] = false;
    _keyState[Keyboard::Key::N] = false;
    _keyState[Keyboard::Key::O] = false;
    _keyState[Keyboard::Key::P] = false;
    _keyState[Keyboard::Key::Q] = false;
    _keyState[Keyboard::Key::R] = false;
    _keyState[Keyboard::Key::S] = false;
    _keyState[Keyboard::Key::T] = false;
    _keyState[Keyboard::Key::U] = false;
    _keyState[Keyboard::Key::V] = false;
    _keyState[Keyboard::Key::W] = false;
    _keyState[Keyboard::Key::X] = false;
    _keyState[Keyboard::Key::Y] = false;
    _keyState[Keyboard::Key::Z] = false;

    _keyState[Keyboard::Key::Num0] = false;
    _keyState[Keyboard::Key::Num1] = false;
    _keyState[Keyboard::Key::Num2] = false;
    _keyState[Keyboard::Key::Num3] = false;
    _keyState[Keyboard::Key::Num4] = false;
    _keyState[Keyboard::Key::Num5] = false;
    _keyState[Keyboard::Key::Num6] = false;
    _keyState[Keyboard::Key::Num7] = false;
    _keyState[Keyboard::Key::Num8] = false;
    _keyState[Keyboard::Key::Num9] = false;

    // Modifiers
    _keyState[Keyboard::Key::LControl] = false;
    _keyState[Keyboard::Key::LShift] = false;
    _keyState[Keyboard::Key::LAlt] = false;
    _keyState[Keyboard::Key::LSystem] = false;
    _keyState[Keyboard::Key::RControl] = false;
    _keyState[Keyboard::Key::RShift] = false;
    _keyState[Keyboard::Key::RAlt] = false;
    _keyState[Keyboard::Key::RSystem] = false;

    // Advanced keys
    _keyState[Keyboard::Key::Menu] = false;
    _keyState[Keyboard::Key::LBracket] = false;
    _keyState[Keyboard::Key::RBracket] = false;
    _keyState[Keyboard::Key::SemiColon] = false;
    _keyState[Keyboard::Key::Comma] = false;
    _keyState[Keyboard::Key::Period] = false;
    _keyState[Keyboard::Key::Quote] = false;
    _keyState[Keyboard::Key::Slash] = false;
    _keyState[Keyboard::Key::BackSlash] = false;
    _keyState[Keyboard::Key::Tilde] = false;
    _keyState[Keyboard::Key::Equal] = false;
    _keyState[Keyboard::Key::Dash] = false;
    _keyState[Keyboard::Key::Space] = false;
    _keyState[Keyboard::Key::Return] = false;
    _keyState[Keyboard::Key::BackSpace] = false;
    _keyState[Keyboard::Key::Tab] = false;
    _keyState[Keyboard::Key::PageUp] = false;
    _keyState[Keyboard::Key::PageDown] = false;
    _keyState[Keyboard::Key::End] = false;
    _keyState[Keyboard::Key::Home] = false;
    _keyState[Keyboard::Key::Insert] = false;
    _keyState[Keyboard::Key::Delete] = false;
    _keyState[Keyboard::Key::Add] = false;
    _keyState[Keyboard::Key::Subtract] = false;
    _keyState[Keyboard::Key::Multiply] = false;
    _keyState[Keyboard::Key::Divide] = false;
    _keyState[Keyboard::Key::Left] = false;
    _keyState[Keyboard::Key::Right] = false;
    _keyState[Keyboard::Key::Up] = false;
    _keyState[Keyboard::Key::Down] = false;
    _keyState[Keyboard::Key::Pause] = false;
    _keyState[Keyboard::Key::CapsLock] = false;
    _keyState[Keyboard::Key::Escape] = false;

    // AZERTY Specifics
    _keyState[Keyboard::Key::Twosuperior] = false;
    _keyState[Keyboard::Key::Ampersand] = false;
    _keyState[Keyboard::Key::Eacute] = false;
    _keyState[Keyboard::Key::QuoteDouble] = false;
    _keyState[Keyboard::Key::LParen] = false;
    _keyState[Keyboard::Key::Egrave] = false;
    _keyState[Keyboard::Key::Underscore] = false;
    _keyState[Keyboard::Key::Ccedilla] = false;
    _keyState[Keyboard::Key::Agrave] = false;
    _keyState[Keyboard::Key::RParen] = false;
    _keyState[Keyboard::Key::DeadCircumflex] = false;
    _keyState[Keyboard::Key::Ugrave] = false;
    _keyState[Keyboard::Key::Asterisk] = false;
    _keyState[Keyboard::Key::Dollar] = false;
    _keyState[Keyboard::Key::Colon] = false;
    _keyState[Keyboard::Key::Exclam] = false;
    _keyState[Keyboard::Key::Less] = false;
    _keyState[Keyboard::Key::Greater] = false;

    // Numpad
    _keyState[Keyboard::Key::Numpad0] = false;
    _keyState[Keyboard::Key::Numpad1] = false;
    _keyState[Keyboard::Key::Numpad2] = false;
    _keyState[Keyboard::Key::Numpad3] = false;
    _keyState[Keyboard::Key::Numpad4] = false;
    _keyState[Keyboard::Key::Numpad5] = false;
    _keyState[Keyboard::Key::Numpad6] = false;
    _keyState[Keyboard::Key::Numpad7] = false;
    _keyState[Keyboard::Key::Numpad8] = false;
    _keyState[Keyboard::Key::Numpad9] = false;

    // Function keys
    _keyState[Keyboard::Key::F1] = false;
    _keyState[Keyboard::Key::F2] = false;
    _keyState[Keyboard::Key::F3] = false;
    _keyState[Keyboard::Key::F4] = false;
    _keyState[Keyboard::Key::F5] = false;
    _keyState[Keyboard::Key::F6] = false;
    _keyState[Keyboard::Key::F7] = false;
    _keyState[Keyboard::Key::F8] = false;
    _keyState[Keyboard::Key::F9] = false;
    _keyState[Keyboard::Key::F10] = false;
    _keyState[Keyboard::Key::F11] = false;
    _keyState[Keyboard::Key::F12] = false;
    _keyState[Keyboard::Key::F13] = false;
    _keyState[Keyboard::Key::F14] = false;
    _keyState[Keyboard::Key::F15] = false;
}

bool Window::isOpen() const {
    return _impl != nullptr;
}

bool Window::pollEvent(Event& event) {
    if (_impl != nullptr) {
        const bool value = _impl->pollEvent(event);

        // If we've receive Destroy just close window without returning the event to the user
        if (value && event.type == EventType::Destroy) {
            close();
            return false;
        }

        if (event.type == EventType::KeyPressed) {
            _keyState[event.key.code] = true;
        } else if (event.type == EventType::KeyReleased) {
            _keyState[event.key.code] = false;
        }
        return value;
    }
    return false;
}

void Window::enableKeyRepeat(bool enable) {
    if (_impl != nullptr) {
        _impl->enableKeyRepeat(enable);
    }
}

void Window::close() {
    if (_impl != nullptr) {
        _impl->close();
        delete _impl;
        _impl = nullptr;
    }
}

bool Window::isKeyPressed(Keyboard::Key key) const {
    return (_keyState.at(key));
}

} // namespace Window
} // namespace lug
