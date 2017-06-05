inline Light::Type Light::getType() const {
    return _type;
}

inline void Light::setType(Type type) {
    _type = type;
}

inline void Light::setColor(const Math::Vec4f& color) {
    _color = color;
}

inline void Light::setDirection(const Math::Vec3f& direction) {
    _direction = direction;
}

inline void Light::setConstantAttenuation(float constantAttenuation) {
    _constantAttenuation = constantAttenuation;
}

inline void Light::setDistance(float distance) {
    _distance = distance;
}

inline void Light::setLinearAttenuation(float linearAttenuation) {
    _linearAttenuation = linearAttenuation;
}

inline void Light::setQuadraticAttenuation(float quadraticAttenuation) {
    _quadraticAttenuation = quadraticAttenuation;
}

inline void Light::setFalloffAngle(float falloffAngle) {
    _falloffAngle = falloffAngle;
}

inline void Light::setFalloffExponent(float falloffExponent) {
    _falloffExponent = falloffExponent;
}

inline const std::string& Light::getName() const {
    return _name;
}
