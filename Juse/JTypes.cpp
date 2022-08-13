#include "JTypes.h"

using Juse::Types::IsScalar;

Juse::S8 Juse::Types::Scalar::toString() const
{
    return S8 { "" };
}

bool Juse::Types::Scalar::equals(const Scalar s) const
{
    return toString() == s.toString();
}

Juse::Types::Number::Number(double d)
{
    data = U64(d);
}

Juse::Types::Number::Number(Juse::I64 i)
{
    data = U64(i);
}

template <IsScalar T>
Juse::Types::Array<T>::Array(const Vector<T> v)
    : data(v)
{
}

template <IsScalar T>
void Juse::Types::Collection<T>::add(const T element)
{
}

template <IsScalar T>
void Juse::Types::Array<T>::add(const T element)
{
    data.push_back(element);
}

template <IsScalar T>
Juse::Vector<T> Juse::Types::Collection<T>::toVector() const
{
    return Vector<T> {};
}

template <IsScalar T>
Juse::Vector<T> Juse::Types::Array<T>::toVector() const
{
    return data;
}

void test()
{
    using namespace Juse;
    using namespace Juse::Types;

    Juse::Vector<Scalar> types {};
    Number n(I64(4));
    types.push_back(n);
}