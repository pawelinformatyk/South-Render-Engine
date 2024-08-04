#pragma once

namespace South
{

template<typename T>
struct TVector
{
    static_assert(std::is_arithmetic_v<T>);

    T X;
    T Y;
    T Z;

    TVector();
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

    T GetNormalized() const;

    static TVector Rand(const T RangeBegin, const T RangeEnd);

    TVector operator+(const TVector& Rhs) const;
    TVector operator+(T XYZ) const;
    TVector operator-(const TVector& Rhs) const;
    TVector operator-(T XYZ) const;
    TVector operator*(const TVector& Rhs) const;
    TVector operator*(T XYZ) const;

    TVector& operator+=(const TVector& Vec);
    TVector& operator*=(const TVector& Vec);

    // #TODO: Inline?
    static const TVector ZeroVector;
    static const TVector OneVector;
    static const TVector ForwardVector;
    static const TVector RightVector;
    static const TVector UpVector;
};


using Vector    = TVector<double>;
using VectorFlt = TVector<float>;
using VectorInt = TVector<int>;

template<typename T>
TVector<T>::TVector() : X(0), Y(0), Z(0)
{
}

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
T TVector<T>::GetNormalized() const
{
    auto Vec = *this;

    Vec.Normalize();

    return Vec;
}

template<typename T>
TVector<T> TVector<T>::Rand(const T RangeBegin, const T RangeEnd)
{
    const T RangeLength = std::abs(RangeEnd - RangeBegin);

    // #TODO: Random utils
    const T X = std::rand() / RAND_MAX * RangeLength - RangeBegin;
    const T Y = std::rand() / RAND_MAX * RangeLength - RangeBegin;
    const T Z = std::rand() / RAND_MAX * RangeLength - RangeBegin;

    return {X, Y, Z};
}

// ~Begin Operators

template<typename T>
TVector<T> TVector<T>::operator+(const TVector<T>& Rhs) const
{
    return {X + Rhs.X, Y + Rhs.Y, Z + Rhs.Z};
}

template<typename T>
TVector<T> TVector<T>::operator+(const T XYZ) const
{
    return {X + XYZ, Y + XYZ, Z + XYZ};
}

template<typename T>
TVector<T> TVector<T>::operator-(const TVector<T>& Rhs) const
{
    return {X - Rhs.X, Y - Rhs.Y, Z - Rhs.Z};
}

template<typename T>
TVector<T> TVector<T>::operator-(const T XYZ) const
{
    return {X - XYZ, Y - XYZ, Z - XYZ};
}

template<typename T>
TVector<T> operator+(const T XYZ, const TVector<T>& Vec)
{
    return Vec + XYZ;
}

template<typename T>
TVector<T> TVector<T>::operator*(const TVector<T>& Rhs) const
{
    return {X * Rhs.Y, Y * Rhs.Y, Z * Rhs.Z};
}

template<typename T>
TVector<T> TVector<T>::operator*(const T XYZ) const
{
    return {X * XYZ, Y * XYZ, Z * XYZ};
}

template<typename T>
TVector<T> operator*(const T XYZ, const TVector<T>& Vec)
{
    return Vec * XYZ;
}


template<typename T>
TVector<T>& TVector<T>::operator+=(const TVector<T>& Vec)
{
    *this = (*this + Vec);

    return *this;
}

template<typename T>
TVector<T>& TVector<T>::operator*=(const TVector<T>& Vec)
{
    this = (*this * Vec);

    return *this;
}

// ~End Operators

template<typename T>
const TVector<T> TVector<T>::ZeroVector(0);

template<typename T>
const TVector<T> TVector<T>::OneVector(1);

template<typename T>
const TVector<T> TVector<T>::ForwardVector(1, 0, 0);

template<typename T>
const TVector<T> TVector<T>::RightVector(0, 1, 0);

template<typename T>
const TVector<T> TVector<T>::UpVector(0, 0, 1);

} // namespace South