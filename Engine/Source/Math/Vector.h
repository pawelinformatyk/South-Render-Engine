#pragma once

namespace South
{

template<typename T>
struct TVector
{
    T X;
    T Y;
    T Z;

    TVector();
    TVector(T X, T Y, T Z);

    T GetSize() const;

    T GetDistanceTo(const TVector& Other) const;
    T GetDistanceSquaredTo(const TVector& Other) const;

    void Normalize();

    T GetNormalized() const;

    void operator+(const TVector& Other) const;
};

using Vector  = TVector<double>;
using VectorF = TVector<float>;

template<typename T>
TVector<T>::TVector() : X(0), Y(0), Z(0)
{
}

template<typename T>
TVector<T>::TVector(T X, T Y, T Z) : X(X), Y(Y), Z(Z)
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
void TVector<T>::operator+(const TVector& Other) const
{
    X += Other.X;
    Y += Other.Y;
    Z += Other.Z;
}

}