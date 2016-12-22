#include <lug/Graphics/Vulkan/RenderWindow.hpp>
#include <lug/System/Debug.hpp>
#include <lug/System/Logger.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

#include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

#include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

#include <lug/Window/Android/WindowImplAndroid.hpp>

#endif

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderWindow::RenderWindow(Renderer &renderer) : _renderer(renderer) {}

RenderWindow::~RenderWindow() {
    destroy();
}

std::unique_ptr<RenderWindow>
RenderWindow::create(Renderer &renderer, uint16_t width, uint16_t height, const std::string &title,
                     lug::Window::Style style) {
    std::unique_ptr<RenderWindow> win(new RenderWindow(renderer));

    if (!win->createWindow(width, height, title, style)) {
        return nullptr;
    }

    if (!win->init()) {
        return nullptr;
    }

    return win;
}

/**
 * Init surface
 * @return Success
 */
bool RenderWindow::initSurface() {
#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    VkWin32SurfaceCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.hinstance = _impl->getHinstance(),
        createInfo.hwnd = _impl->getHandle()
    };

    VkResult result = vkCreateWin32SurfaceKHR(_renderer.getInstance(), &createInfo, nullptr, &_surface);
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    VkXlibSurfaceCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.dpy = _impl->getDisplay(),
        createInfo.window = _impl->getWindow()
    };

    VkResult result = vkCreateXlibSurfaceKHR(_renderer.getInstance(), &createInfo, nullptr, &_surface);
#else
#error "RenderWindow::initSurface Unknow platform"
#endif

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererWindow: Can't initialize surface: {}", result);
        return false;
    }
    return true;
}

bool RenderWindow::initSwapchain() {
    VkResult result;
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // Get swapchain capabilities
    {
        result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->handle, _surface, &info->swapChain.capabilities);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't get surface capabilities: {}", result);
            return false;
        }

        uint32_t formatsCount = 0;
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, _surface, &formatsCount, nullptr);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve formats count: {}", result);
            return false;
        }

        info->swapChain.formats.resize(formatsCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, _surface, &formatsCount, info->swapChain.formats.data());
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve formats: {}", result);
            return false;
        }

        uint32_t presentModesCount = 0;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, _surface, &presentModesCount, nullptr);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve present modes count: {}", result);
            return false;
        }

        info->swapChain.presentModes.resize(presentModesCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, _surface, &presentModesCount, info->swapChain.presentModes.data());
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve present modes: {}", result);
            return false;
        }
    }

    // Get present queues
    {
        for (auto& queue : _renderer.getQueues()) {
            VkBool32 supported = 0;
            result = vkGetPhysicalDeviceSurfaceSupportKHR(info->handle, queue.getFamilyIdx(), _surface, &supported);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererWindow: Can't check if queue supports presentation: {}", result);
                return false;
            }
            queue.supportsPresentation(!!supported);
        }
    }

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkSurfaceFormatKHR swapchainFormat{
            swapchainFormat.format = VK_FORMAT_B8G8R8A8_UNORM,
            swapchainFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    };
    uint32_t minImageCount = 3;
    VkExtent2D extent{};
    VkSurfaceTransformFlagsKHR transform{};

    // Check requirements for swapchain
    {
        // Check the preset mode
        if (std::find(info->swapChain.presentModes.begin(), info->swapChain.presentModes.end(), swapchainPresentMode) == info->swapChain.presentModes.end()) {
            LUG_LOG.error("RendererWindow: Missing VK_PRESENT_MODE_MAILBOX_KHR mode");
            return false;
        }

        // Check the formats
        if (std::find_if(info->swapChain.formats.begin(), info->swapChain.formats.end(), [&swapchainFormat](const VkSurfaceFormatKHR& lhs) {
            return lhs.colorSpace == swapchainFormat.colorSpace && swapchainFormat.format == lhs.format;
        }) == info->swapChain.formats.end()) {
            LUG_LOG.error("RendererWindow: Missing VK_FORMAT_B8G8R8A8_UNORM/VK_COLOR_SPACE_SRGB_NONLINEAR_KHR format");
            return false;
        }

        // Check image counts
        if (info->swapChain.capabilities.maxImageCount > 0 && info->swapChain.capabilities.maxImageCount < minImageCount) {
            LUG_LOG.error("RendererWindow: Not enough images ({} required), found {}", minImageCount, info->swapChain.capabilities.maxImageCount);
            return false;
        }

        // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
        if (info->swapChain.capabilities.currentExtent.height == 0xFFFFFFFF
            && info->swapChain.capabilities.currentExtent.width == 0xFFFFFFFF) {
            extent.height = _mode.height;
            extent.width = _mode.width;
        } else {
            extent.height = info->swapChain.capabilities.currentExtent.height;
            extent.width = info->swapChain.capabilities.currentExtent.width;
        }

        // Find the transformation of the surface
        if (info->swapChain.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
            // We prefer a non-rotated transform
            transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            transform = info->swapChain.capabilities.currentTransform;
        }
    }

    // Create the swapchain
    {
        VkSwapchainCreateInfoKHR createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.surface = _surface,
            createInfo.minImageCount = minImageCount,
            createInfo.imageFormat = swapchainFormat.format,
            createInfo.imageColorSpace = swapchainFormat.colorSpace,
            {}, // createInfo.imageExtent
            createInfo.imageArrayLayers = 1,
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            createInfo.queueFamilyIndexCount = 0,
            createInfo.pQueueFamilyIndices = nullptr,
            createInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(transform),
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            createInfo.presentMode = swapchainPresentMode,
            createInfo.clipped = VK_TRUE,
            createInfo.oldSwapchain = VK_NULL_HANDLE
        };

        // Thank you clang
        createInfo.imageExtent.width = extent.width;
        createInfo.imageExtent.height = extent.height;

        std::vector<uint32_t> queueFamilyIndices(1);

        const Queue* presentationQueue = _renderer.getQueue(0, true);
        if (!presentationQueue) {
            LUG_LOG.error("RendererWindow: Can't find presentation queue");
            return false;
        }
        queueFamilyIndices[0] = presentationQueue->getFamilyIdx();

        // Check if the presentation and graphics queue are the same
        if (!_renderer.isSameQueue(0, true, VK_QUEUE_GRAPHICS_BIT, false)) {
            const Queue* graphicsQueue = _renderer.getQueue(VK_QUEUE_GRAPHICS_BIT, false);
            if (!graphicsQueue) {
                LUG_LOG.error("RendererWindow: Can't find presentation queue");
                return false;
            }
            queueFamilyIndices.push_back(graphicsQueue->getFamilyIdx());
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        }

        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();

        VkSwapchainKHR swapchainKHR;
        result = vkCreateSwapchainKHR(_renderer.getDevice(), &createInfo, nullptr, &swapchainKHR);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't initialize swapchain: {}", result);
            return false;
        }

        _swapchain = Swapchain(swapchainKHR, &_renderer.getDevice());

        if (!_swapchain.initImages(swapchainFormat))
            return false;
    }

    return true;
}

bool RenderWindow::init() {
    return initSurface() && initSwapchain();
}

void RenderWindow::destroy() {
    _swapchain.destroy();
    if (_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(_renderer.getInstance(), _surface, nullptr);
        _surface = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
