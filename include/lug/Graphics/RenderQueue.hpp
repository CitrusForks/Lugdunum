#pragma once

#include <vector>

namespace lug {
namespace Graphics {

class MovableObject;

class RenderQueue {
public:
    RenderQueue() = default;

    RenderQueue(const RenderQueue&) = delete;
    RenderQueue(RenderQueue&&) = delete;

    RenderQueue& operator=(const RenderQueue&) = delete;
    RenderQueue& operator=(RenderQueue&&) = delete;

    ~RenderQueue() = default;

    void addMovableObject(MovableObject* object);
    void clear();

    const std::vector<MovableObject*>& getObjects() const;

private:
    std::size_t _size{0};
    std::vector<MovableObject*> _queue{4000}; // TODO: Change that
};

#include <lug/Graphics/RenderQueue.inl>

} // Graphics
} // lug
