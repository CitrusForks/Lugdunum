#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light/Light.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Light {

/**
 * @brief     Class for Directional
 */
class LUG_GRAPHICS_API Directional final : public Light {
public:
    struct LightData{
        Math::Vec3f ambient;
        float padding;
        Math::Vec3f diffuse;
        float padding2;
        Math::Vec3f specular;
        float padding3;
        Math::Vec3f direction;
    };

public:
    /**
     * @brief      Constructs a Directional
     *
     * @param[in]  name  The name of the Light
     * @param[in]  type  The type of the Directional
     */
    Directional(const std::string& name);

    Directional(const Directional&) = delete;
    Directional(Directional&&) = delete;

    Directional& operator=(const Directional&) = delete;
    Directional& operator=(Directional&&) = delete;

    ~Directional() override final = default;

    void setDirection(const Math::Vec3f& direction);

    void* getData(uint32_t& size) override final;

private:
    Math::Vec3f _direction{0.0f, 0.0f, 0.0f};

    LightData _data;
};

} // Light
} // Graphics
} // lug
