// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_VECTOR_H
#define CORE_VECTOR_H

#include <Core/Assert.h>

#include <cmath>

namespace iocod {

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;

    Vec3()
        : x{NAN},
          y{NAN},
          z{NAN}
    {
    }

    explicit Vec3(f32 xyz)
        : x{xyz},
          y{xyz},
          z{xyz}
    {
        Assert(IsValid());
    }

    Vec3(f32 x, f32 y, f32 z)
        : x{x},
          y{y},
          z{z}
    {
        Assert(IsValid());
    }

    bool IsValid() const;
    f32* Base();
    f32 const* Base() const;
    void Zero();
    f32 Length() const;
    f32 LengthSquared() const;
    f32 Normalize();

    f32 operator[](u32 index) const;
    f32& operator[](u32 index);
    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;
    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(const Vec3& other);
    Vec3& operator/=(const Vec3& other);
    Vec3& operator*=(f32 scalar);
    Vec3& operator/=(f32 scalar);
    Vec3& operator+=(f32 scalar);
    Vec3& operator-=(f32 scalar);
    Vec3& operator-();
    Vec3& operator=(const Vec3& other);

    // copy functions
    Vec3 operator-() const;
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(const Vec3& other) const;
    Vec3 operator/(const Vec3& other) const;
    Vec3 operator*(f32 scalar) const;
    Vec3 operator/(f32 scalar) const;
};

inline bool Vec3::IsValid() const
{
    return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

inline f32* Vec3::Base()
{
    return &x;
}

inline f32 const* Vec3::Base() const
{
    return &x;
}

inline void Vec3::Zero()
{
    x = y = z = 0.0f;
}

inline f32 Vec3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

inline f32 Vec3::LengthSquared() const
{
    return (x * x + y * y + z * z);
}

inline f32 Vec3::Normalize()
{
    f32 length = Length();
    if (length > 0.0f) {
        f32 inv = 1.0f / length;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    return length;
}

inline f32 Vec3::operator[](u32 index) const
{
    Assert((index < 3));
    return (&x)[index];
}

inline f32& Vec3::operator[](u32 index)
{
    Assert((index < 3));
    return (&x)[index];
}

inline bool Vec3::operator==(const Vec3& other) const
{
    Assert(other.IsValid());
    Assert(IsValid());
    return (x == other.x) && (y == other.y) && (z == other.z);
}

inline bool Vec3::operator!=(const Vec3& other) const
{
    Assert(other.IsValid());
    Assert(IsValid());
    return (x != other.x) || (y != other.y) || (z != other.z);
}

inline Vec3& Vec3::operator+=(const Vec3& other)
{
    Assert(other.IsValid());
    Assert(IsValid());
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& other)
{
    Assert(other.IsValid());
    Assert(IsValid());
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& other)
{
    Assert(other.IsValid());
    x *= other.x;
    y *= other.y;
    z *= other.z;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& other)
{
    Assert(other.IsValid());
    Assert(other.x != 0.0f && other.y != 0.0f && other.z != 0.0f);
    x /= other.x;
    y /= other.y;
    z /= other.z;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator*=(f32 scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator/=(f32 scalar)
{
    Assert(scalar != 0.0f);
    f32 inv = 1.0f / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator+=(f32 scalar)
{
    x += scalar;
    y += scalar;
    z += scalar;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator-=(f32 scalar)
{
    x -= scalar;
    y -= scalar;
    z -= scalar;
    Assert(IsValid());
    return *this;
}

inline Vec3& Vec3::operator-()
{
    Assert(IsValid());
    x = -x;
    y = -y;
    z = -z;
}

inline Vec3& Vec3::operator=(const Vec3& other)
{
    Assert(other.IsValid());
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

// -------------------------------------
// Vector related operations
inline void VectorClear(Vec3& a)
{
    a.x = a.y = a.z = 0.0f;
}

inline void VectorCopy(const Vec3& src, Vec3& dest)
{
    Assert(src.IsValid());
    dest.x = src.x;
    dest.y = src.y;
    dest.z = src.z;
}

inline void VectorAdd(const Vec3& a, const Vec3& b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(b.IsValid());
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
}

inline void VectorSubtract(const Vec3& a, const Vec3& b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(b.IsValid());
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
}

inline void VectorMultiply(const Vec3& a, const Vec3& b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(b.IsValid());
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
}

#define VectorScale VectorMultiply

inline void VectorMultiply(const Vec3& a, f32 b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(!std::isnan(b));
    c.x = a.x * b;
    c.y = a.y * b;
    c.z = a.z * b;
}

inline void VectorDivide(const Vec3& a, const Vec3& b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(b.IsValid());
    Assert(b.x != 0.0f && b.y != 0.0f && b.z != 0.0f);
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
}

inline void VectorDivide(const Vec3& a, f32 b, Vec3& c)
{
    Assert(a.IsValid());
    Assert(b != 0.0f);
    f32 inv = 1.0f / b;
    c.x = a.x * inv;
    c.y = a.y * inv;
    c.z = a.z * inv;
}

inline void VectorLerp(const Vec3& src1, const Vec3& src2, f32 t, Vec3& dest)
{
    Assert(src1.IsValid());
    Assert(src2.IsValid());
    dest.x = src1.x + (src2.x - src1.x) * t;
    dest.y = src1.y + (src2.y - src1.y) * t;
    dest.z = src1.z + (src2.z - src1.z) * t;
}

inline Vec3 VectorLerp(const Vec3& src1, const Vec3& src2, f32 t)
{
    Vec3 result;
    VectorLerp(src1, src2, t, result);
    return result;
}

inline f32 DotProduct(const Vec3& a, const Vec3& b)
{
    Assert(a.IsValid());
    Assert(b.IsValid());
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// -------------------------------------

inline Vec3 Vec3::operator-() const
{
    return Vec3{-x, -y, -z};
}

inline Vec3 Vec3::operator+(const Vec3& other) const
{
    Vec3 result;
    VectorAdd(*this, other, result);
    return result;
}

inline Vec3 Vec3::operator-(const Vec3& other) const
{
    Vec3 result;
    VectorSubtract(*this, other, result);
    return result;
}

inline Vec3 Vec3::operator*(const Vec3& other) const
{
    Vec3 result;
    VectorMultiply(*this, other, result);
    return result;
}

inline Vec3 Vec3::operator/(const Vec3& other) const
{
    Vec3 result;
    VectorDivide(*this, other, result);
    return result;
}

inline Vec3 Vec3::operator*(f32 scalar) const
{
    Vec3 result;
    VectorMultiply(*this, scalar, result);
    return result;
}

inline Vec3 Vec3::operator/(f32 scalar) const
{
    Vec3 result;
    VectorDivide(*this, scalar, result);
    return result;
}

} // namespace iocod

#endif // CORE_VECTOR_H
