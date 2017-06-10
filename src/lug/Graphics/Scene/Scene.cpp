#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

Scene::Scene(const std::string& name) : Resource(Resource::Type::Scene, name), _root{*this, "root"} {}

Node* Scene::createSceneNode(const std::string& name) {
    _nodes.emplace_back(*this, name);

    Node* node = &_nodes.back();

    return node;
}

std::unique_ptr<MovableCamera> Scene::createMovableCamera(const std::string& name, Render::Camera* camera) {
    return std::make_unique<MovableCamera>(name, camera);
}

Node* Scene::getSceneNode(const std::string& name) {
    return _root.getNode(name);
}

const Node* Scene::getSceneNode(const std::string& name) const {
    return _root.getNode(name);
}

void Scene::fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const {
    _root.fetchVisibleObjects(renderView, camera, renderQueue);
}

} // Scene
} // Graphics
} // lug
