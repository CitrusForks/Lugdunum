#include <lug/Window/Window.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

int main() {
    auto logger = lug::System::makeLogger("myLogger");
    logger->addHandler(lug::System::makeHandler<lug::System::StdoutHandler>("myLoggerStdout"));

    lug::System::Logger::logger.addHandler(lug::System::makeHandler<lug::System::StdoutHandler>("stdout"));

    lug::Graphics::Graphics graphics{
        {
            "hello",
            {0, 1, 0}
        }
    };

    // graphics.setRendererType(Renderer::Type::Vulkan);
    // graphics.addModule(...);

    if (!graphics.init()) {
        return 1;
    }

    // Get the render window
    lug::Graphics::Renderer* renderer = graphics.getRenderer();

    auto window = renderer->createWindow(800, 600, "Default Window", lug::Window::Style::Default);

    if (!window) {
        logger->error("Failed to init window");
        return 1;
    }

    // Window loop
    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            if (event.type == lug::Window::EventType::RESIZE) {
                logger->info("Resizing the app");
            } else if (event.type == lug::Window::EventType::CLOSE) {
                logger->info("Closing the app");
                window->close();
                return 0;
            }
        }

        if (!window->beginFrame()) {
            logger->fatal("beginFrame() failed");
            return 1;
        }

        // float clearColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
        //window->clearScreen(clearColor);
        // TODO: Render objects
        if (!window->endFrame()) {
            logger->fatal("beginFrame() failed");
            return 1;
        }
    }

    return 0;
}
