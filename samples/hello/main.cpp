#include <iostream>
#include <lug/Graphic/Graphic.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

int main() {
    lug::Graphic::Graphic graphic{{"hello", {0, 1, 0}}};

    //graphic.setRendererType(Renderer::Type::Vulkan);
    //graphic.addModule(...);

    // Add or remove modules
    // Set the renderer type

    graphic.init();

    return 0;
}
