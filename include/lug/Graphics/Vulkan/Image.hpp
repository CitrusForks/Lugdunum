#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API Image {
public:
    Image(VkImage Image = VK_NULL_HANDLE, const Device* device = nullptr, bool swapchainImage = false, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);

    Image(const Image&) = delete;
    Image(Image&& Image);

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&& Image);

    ~Image();

    operator VkImage() const {
        return _image;
    }

    void destroy();

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    bool _swapchainImage;

    VkImageAspectFlags _aspect;
};

} // Vulkan
} // Graphics
} // lug
