#pragma once

#include <list>
#include <string>
#include <vector>

#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Mesh {
public:
    class PrimitiveSet {
    public:
        PrimitiveSet() = default;

        PrimitiveSet(const PrimitiveSet&) = delete;
        PrimitiveSet(PrimitiveSet&&) = default;

        PrimitiveSet& operator=(const PrimitiveSet&) = delete;
        PrimitiveSet& operator=(PrimitiveSet&&) = delete;

        ~PrimitiveSet() = default;

        void setMode(Render::Mesh::PrimitiveSet::Mode mode);
        void setMaterial(Resource::SharedPtr<Render::Material> material);

        // TODO: Non raw mode

        // Raw mode
        void addAttributeBuffer(void* data, uint32_t size, Render::Mesh::PrimitiveSet::Attribute::Type type);

        Render::Mesh::PrimitiveSet::Mode getMode() const;
        Resource::SharedPtr<Render::Material> getMaterial() const;
        const std::vector<Render::Mesh::PrimitiveSet::Attribute>& getAttributes() const;

    private:
        Render::Mesh::PrimitiveSet::Mode _mode{Render::Mesh::PrimitiveSet::Mode::Triangles};
        Resource::SharedPtr<Render::Material> _material{nullptr};

        std::vector<Render::Mesh::PrimitiveSet::Attribute> _attributes;
    };

    friend class PrimitiveSet;

public:
    explicit Mesh(Renderer& renderer);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    virtual ~Mesh() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    /**
     * @brief      Adds a primitive set to the builder and returns it.
     */
    PrimitiveSet* addPrimitiveSet();

    virtual Resource::SharedPtr<Render::Mesh> build() = 0;

protected:
    Renderer& _renderer;

    std::string _name;
    std::list<PrimitiveSet> _primitiveSets;
};

#include <lug/Graphics/Builder/Mesh.inl>

} // Builder
} // Graphics
} // lug
