#pragma once

#include <lug\Maths\Vector.hpp>

namespace lug {
namespace Maths {

class Quaternion {
    public:
    Quaternion(double x, double y, double z, double w);
    Quaternion(Vec3d v, double w);
    Quaternion(const Quaternion &otherQuaternion);
    Quaternion &operator=(const Quaternion &leftOperand);
    virtual ~Quaternion();

    Quaternion conjugate();
    Quaternion invert();
    Quaternion normalize();
    const double angleWith(const Quaternion &otherQuaternion);
    Quaternion slerp(const Quaternion &otherQuaternion, double t);

    const double &getAngle() const;
    void setAngle(const double &);

    const double &getX() const;
    void setX(const double &);

    const double &getY() const;
    void setY(const double &);

    const double &getZ( )const;
    void setZ(const double &);

    const double &getW() const;
    void setW(const double &);

    const Vec3d &getAxis() const;
    void setAxis(const Vec3d &);

    const Quaternion &getIdentity() const;
    void setIdentity(const Quaternion &);



    protected:
    double _angle;
    Vec3d _Axis;
    Quaternion *_identity;
    bool _isIdentity;
    bool _isNormalized;
    double _X, _Y, _Z, _W;

   
};
//Quaternion/Quaternion operator
Quaternion operator+(const Quaternion &leftOperand, const Quaternion &rightOperand);
Quaternion operator-(const Quaternion &leftOperand, const Quaternion &rightOperand);
Quaternion operator*(const Quaternion &leftOperand, const Quaternion &rightOperand);
const bool operator==(const Quaternion &leftOperand, const Quaternion &rightOperand);

//Quaternion/real operator
Quaternion operator*(const Quaternion &leftOperand, const double scalar);
Quaternion operator+(const Quaternion &leftOperand, const double scalar);
Quaternion operator-(const Quaternion &leftOperand, const double scalar);
Quaternion operator/(const Quaternion &leftOperand, const double scalar);

}
}


