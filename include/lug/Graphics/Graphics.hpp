#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Scene.hpp>
#include <lug/Graphics/Mesh.hpp>
#include <lug/Graphics/Camera.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Graphics {
public:
    struct InitInfo {
        Renderer::Type rendererType;
        Renderer::InitInfo rendererInitInfo;
    };

public:
    Graphics(const char* appName, uint32_t appVersion);

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;

    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    ~Graphics() = default;

    bool init(const InitInfo& initInfo);

    bool isModuleLoaded(Module::Type type) const;
    const std::set<Module::Type>& getLoadedModules() const;

    Renderer* getRenderer() const;

    std::unique_ptr<Scene> createScene();
    std::unique_ptr<Mesh> createMesh(const std::string& name);
    std::unique_ptr<Camera> createCamera(const std::string& name);

private:
    const char* _appName;
    uint32_t _appVersion;

    std::set<Module::Type> _loadedModules{};
    std::unique_ptr<Renderer> _renderer{nullptr};
    Renderer::Type _rendererType;
};

#include <lug/Graphics/Graphics.inl>

} // Graphics
} // lug
