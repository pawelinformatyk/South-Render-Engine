#pragma once

namespace South
{

template<typename T>
struct TVector final
{
    T X;
    T Y;
    T Z;

    TVector();
    TVector(T X, T Y, T Z);

    T GetSize() const;

    T DistanceTo(const TVector& Other) const;
    T DistanceSquaredTo(const TVector& Other) const;

    void Normalize();

    T GetNormalized() const;

    void operator+(const TVector& Other) const;
};

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
T TVector<T>::DistanceTo(const TVector& Other) const
{
    return std::sqrt(DistanceSquaredTo(Other));
}

template<typename T>
T TVector<T>::DistanceSquaredTo(const TVector& Other) const
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