#pragma once

#include "vec3.hpp"
#include <compare>

namespace South
{

template<typename T>
struct TVector
{
    static_assert(std::is_arithmetic_v<T>);

    T X = 0;
    T Y = 0;
    T Z = 0;

    TVector() = default;
    TVector(T XYZ);
    TVector(T X, T Y);
    TVector(T X, T Y, T Z);

    template<typename CastType>
    explicit operator TVector<CastType>()
    {
        return {static_cast<CastType>(X), static_cast<CastType>(Y), static_cast<CastType>(Z)};
    }

    T GetSize() const;

    T GetDistanceTo(const TVector& Other) const;
    T GetDistanceSquaredTo(const TVector& Other) const;

    void Normalize();

    TVector<T> GetNormalized() const;

    static TVector Rand(const T RangeBegin, const T RangeEnd);

    TVector<T> operator+(const TVector<T>& Other) const
    {
        return {X + Other.X, Y + Other.Y, Z + Other.Z};
    }

    TVector<T> operator+(const T Other) const
    {
        return {X + Other, Y + Other, Z + Other};
    }

    TVector<T> operator-(const TVector<T>& Other) const
    {
        return {X - Other.X, Y - Other.Y, Z - Other.Z};
    }

    TVector<T> operator-(const T Other) const
    {
        return {X - Other, Y - Other, Z - Other};
    }

    TVector<T> operator*(const TVector<T>& Other) const
    {
        return {X * Other.Y, Y * Other.Y, Z * Other.Z};
    }

    TVector<T> operator*(const T Other) const
    {
        return {X * Other, Y * Other, Z * Other};
    }

    TVector<T> operator/(const TVector<T>& Other) const
    {
        return {X / Other.Y, Y / Other.Y, Z / Other.Z};
    }

    TVector<T> operator/(const T Other) const
    {
        return {X / Other, Y / Other, Z / Other};
    }

    TVector& operator+=(const TVector& Other)
    {
        *this = (*this + Other);

        return *this;
    }

    TVector& operator-=(const TVector& Other)
    {
        *this = (*this - Other);

        return *this;
    }

    TVector& operator*=(const TVector& Other)
    {
        *this = (*this * Other);

        return *this;
    }

    TVector& operator/=(const TVector& Other)
    {
        *this = (*this / Other);

        return *this;
    }

    friend auto operator<=>(const TVector& Lhs, const TVector& Rhs) = default;

    // #TODO: Inline?
    static const TVector ZeroVector;
    static const TVector OneVector;
    static const TVector ForwardVector;
    static const TVector RightVector;
    static const TVector UpVector;
};

using SVector    = TVector<double>;
using SVectorFlt = TVector<float>;
using SVectorInt = TVector<int>;

template<typename T>
TVector<T>::TVector(const T X, const T Y, const T Z) : X(X), Y(Y), Z(Z)
{
}

template<typename T>
TVector<T>::TVector(const T X, const T Y) : X(X), Y(Y), Z(0)
{
}

template<typename T>
TVector<T>::TVector(const T XYZ) : X(XYZ), Y(XYZ), Z(XYZ)
{
}

template<typename T>
T TVector<T>::GetSize() const
{
    return X * X + Y * Y + Z * Z;
}

template<typename T>
T TVector<T>::GetDistanceTo(const TVector& Other) const
{
    return std::sqrt(GetDistanceSquaredTo(Other));
}

template<typename T>
T TVector<T>::GetDistanceSquaredTo(const TVector& Other) const
{
    return (X - Other.X) * (X - Other.X) + (Y - Other.Y) * (Y - Other.Y) + (Z - Other.Z) * (Z - Other.Z);
}

template<typename T>
void TVector<T>::Normalize()
{
    const T Size = GetSize();

    X /= Size;
    Y /= Size;
    Z /= Size;
}

template<typename T>
TVector<T> TVector<T>::GetNormalized() const
{
    auto Vec = *this;

    Vec.Normalize();

    return Vec;
}

template<typename T>
TVector<T> TVector<T>::Rand(const T RangeBegin, const T RangeEnd)
{
    const T RangeLength = std::abs(RangeEnd - RangeBegin);
    const T Div         = RAND_MAX * RangeLength;

    // #TODO: Random utils
    const T X = std::rand() / Div - RangeBegin;
    const T Y = std::rand() / Div - RangeBegin;
    const T Z = std::rand() / Div - RangeBegin;

    return {X, Y, Z};
}

// ~Begin Operators

// #TODO: T1 = float, T2 = double?
template<typename T>
TVector<T> operator+(const T Lhs, const TVector<T>& Rhs)
{
    return Rhs + Lhs;
}

template<typename T>
TVector<T> operator-(const T Lhs, const TVector<T>& Rhs)
{
    return Rhs - Lhs;
}

template<typename T>
TVector<T> operator*(const T Lhs, const TVector<T>& Rhs)
{
    return Rhs * Lhs;
}

template<typename T>
TVector<T> operator/(const T Lhs, const TVector<T>& Rhs)
{
    return Rhs / Lhs;
}

// ~End Operators

template<typename T>
const TVector<T> TVector<T>::ZeroVector(0);

template<typename T>
const TVector<T> TVector<T>::OneVector(1);

// Left-handed system

template<typename T>
const TVector<T> TVector<T>::ForwardVector(0, 0, 1);

template<typename T>
const TVector<T> TVector<T>::RightVector(1, 0, 0);

template<typename T>
const TVector<T> TVector<T>::UpVector(0, 1, 0);

// Conversion
template<typename T>
inline glm::vec3 Convert(const South::TVector<T>& Vector)
{
    return glm::vec3 {Vector.X, Vector.Y, -Vector.Z};
}

inline South::SVectorFlt Convert(const glm::vec3& Vector)
{
    return South::SVectorFlt {Vector.x, Vector.y, -Vector.z};
}

} // namespace South