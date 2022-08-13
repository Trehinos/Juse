#pragma once

#include "types.h"

namespace Juse {
namespace Types {
    struct Type {
    };

    struct Scalar : public Type {
        virtual S8 toString() const;
        virtual bool equals(const Scalar s) const;
    };

    template <typename T>
    concept IsScalar = IsAnyOf<T, Scalar>;

    template <IsScalar T>
    struct Collection : public Scalar {
        virtual void add(const T element);
        virtual Vector<T> toVector() const;
    };

    class Number : public Scalar {
        U64 data;

    public:
        Number(I64 = 0);
        Number(double);
        I64 parseInt() const;
        double parseFloat() const;
    };

    class String : public Scalar {
        S8 string;

    public:
        String(S8 = "");
    };

    template <IsScalar T>
    class Array : public Collection<T> {
        Vector<T> data;

    public:
        Array(Vector<T> = {});
        virtual void add(const T element);
        virtual Vector<T> toVector() const;
    };

}
}
