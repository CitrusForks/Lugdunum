#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/RenderPass.hpp>
#include <lug/Graphics/Vulkan/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <memory>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Pipeline {
public:
    explicit Pipeline(VkPipeline pipeline = VK_NULL_HANDLE, const Device* device = nullptr,
        std::unique_ptr<PipelineLayout> layout = nullptr, std::unique_ptr<RenderPass> renderPass = nullptr);

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& pipeline);

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& pipeline);

    ~Pipeline();

    operator VkPipeline() const {
        return _pipeline;
    }

    void bind(const CommandBuffer* commandBuffer);

    void destroy();

    RenderPass* getRenderPass() const;

    static std::unique_ptr<Pipeline> createGraphicsPipeline(const Device* device);

private:
    VkPipeline _pipeline{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::unique_ptr<PipelineLayout> _layout;
    std::unique_ptr<RenderPass> _renderPass;
};

} // Vulkan
} // Graphics
} // lug
