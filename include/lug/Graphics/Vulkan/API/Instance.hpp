#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class LUG_GRAPHICS_API Instance {
public:
    explicit Instance(VkInstance instance = VK_NULL_HANDLE);

    Instance(const Instance&) = delete;
    Instance(Instance&& instance);

    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&& instance);

    ~Instance();

    explicit operator VkInstance() const {
        return _instance;
    }

    template <typename Function>
    Function getProcAddr(const char* name) const;

    void destroy();

private:
    VkInstance _instance{VK_NULL_HANDLE};
};

#include <lug/Graphics/Vulkan/API/Instance.inl>

} // API
} // Vulkan
} // Graphics
} // lug
