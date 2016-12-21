#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderWindow.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API RenderWindow final : public ::lug::Graphics::RenderWindow {
public:
    RenderWindow() = delete;

    RenderWindow(Renderer& renderer);

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;

    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

    ~RenderWindow();

    static std::unique_ptr<RenderWindow> create(Renderer& renderer, uint16_t width, uint16_t height, const std::string& title, lug::Window::Style style);

    bool init() override final;

private:
    bool createSurface();
    void destroy();

private:
    Renderer& _renderer;
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
};

} // Vulkan
} // Graphics
} // lug
