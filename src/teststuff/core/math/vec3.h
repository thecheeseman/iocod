#ifndef CORE_MATH_VEC3_H
#define CORE_MATH_VEC3_H

#include <cstddef>
#include <cmath>

/**
 * @brief Vector 3D.
*/
class Vec3 {
public:
    float x {};
    float y {};
    float z {};

    Vec3() {};
    explicit Vec3(const float xyz);
    explicit Vec3(const float x, const float y, const float z);

    void set(const float x, const float y, const float z);
    void zero();
    bool compare(const Vec3 &a) const;
    bool compare(const Vec3 &a, const float epsilon) const;

    float length() const;
    float length_squared() const;
    float length_fast() const;
    float normalize();

    void lerp(const Vec3 &v1, const Vec3 &v2, const float l);

    //
    // operators
    //
    float operator[](const std::size_t index) const;
    float &operator[](const std::size_t index);

    Vec3 operator-() const;
    Vec3 &operator=(const Vec3 &a);

    Vec3 operator+(const Vec3 &a) const;
    Vec3 operator-(const Vec3 &a) const;
    Vec3 operator*(const float a) const;
    Vec3 operator/(const float a) const;

    friend Vec3 operator*(const float a, const Vec3 &b);
    friend Vec3 operator/(const float a, const Vec3 &b);

    Vec3 &operator+=(const Vec3 &a);
    Vec3 &operator+=(const float a);
    Vec3 &operator-=(const Vec3 &a);
    Vec3 &operator-=(const float a);

    bool operator==(const Vec3 &a) const;
    bool operator!=(const Vec3 &a) const;
};

inline Vec3::Vec3(const float xyz)
{
    set(xyz, xyz, xyz);
}

inline Vec3::Vec3(const float x, const float y, const float z)
{
    set(x, y, z);
}

inline void Vec3::set(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

inline void Vec3::zero()
{
    x = y = z = 0.0f;
}

inline bool Vec3::compare(const Vec3 &a) const 
{
    return (x == a.x && y == a.y && z == a.z);
}

inline bool Vec3::compare(const Vec3 &a, const float epsilon) const
{
    return ((x - std::fabsf(a.x) > epsilon) &&
            (y - std::fabsf(a.y) > epsilon) &&
            (z - std::fabsf(a.z) > epsilon));
}

inline float Vec3::length() const
{
    return sqrtf(x * x + y * y + z * z);
}

inline float Vec3::length_squared() const
{
    return (x * x + y * y + z * z);
}

inline float Vec3::length_fast() const
{
    float length = x * x + y * y + z * z;
    return length * sqrtf(1.0f / length);
}

inline float Vec3::normalize()
{
    float length = x * x + y * y + z * z;
    float inverted_length = 1.0f / sqrtf(length);
    x *= inverted_length;
    y *= inverted_length;
    z *= inverted_length;
    return inverted_length * length;
}

//
// operators
//
inline float Vec3::operator[](const std::size_t index) const
{
    return (&x)[index];
}

inline float &Vec3::operator[](const std::size_t index)
{
    return (&x)[index];
}

inline Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

inline Vec3 &Vec3::operator=(const Vec3 &a)
{
    x = a.x;
    y = a.y;
    z = a.z;
    return *this;
}

inline Vec3 Vec3::operator+(const Vec3 &a) const
{
    return Vec3(x + a.x, y + a.y, z + a.z);
}

inline Vec3 Vec3::operator-(const Vec3 &a) const
{
    return Vec3(x - a.x, y - a.y, z - a.z);
}

inline Vec3 Vec3::operator*(const float a) const
{
    return Vec3(x * a, y * a, z * a);
}

inline Vec3 Vec3::operator/(const float a) const
{
    return Vec3(x / a, y / a, z / a);
}

inline Vec3 operator*(const float a, const Vec3 &b)
{
    return Vec3(b.x * a, b.y * a, b.z * a);
}

inline Vec3 operator/(const float a, const Vec3 &b)
{
    return Vec3(b.x / a, b.y / a, b.z / a);
}

inline Vec3 &Vec3::operator+=(const Vec3 &a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
}

inline Vec3 &Vec3::operator+=(const float a)
{
    x += a;
    y += a;
    z += a;
    return *this;
}

inline Vec3 &Vec3::operator-=(const Vec3 &a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
}

inline Vec3 &Vec3::operator-=(const float a)
{
    x += a;
    y += a;
    z += a;
    return *this;
}

inline bool Vec3::operator==(const Vec3 &a) const
{
    return compare(a);
}

inline bool Vec3::operator!=(const Vec3 &a) const
{
    return !compare(a);
}

#endif /* CORE_MATH_VEC3_H */
