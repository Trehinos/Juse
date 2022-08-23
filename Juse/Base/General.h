#pragma once

#include <any>
#include <optional>
#include <array>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <concepts>
#include <functional>

namespace Juse
{
    /* Usual types */
    template <class T>
    using UPtr = std::unique_ptr<T>;
    template <class T>
    using SPtr = std::shared_ptr<T>;
    template <typename T>
    using Ref = std::reference_wrapper<T>;
    template <typename T>
    using Opt = std::optional<T>;
    template <typename T>
    using Wrap = Opt<Ref<T>>;

    template <typename T, typename U>
    using Pair = std::pair<T, U>;
    template <typename T, typename U>
    using Map = std::map<T, U>;
    template <typename T>
    using Vector = std::vector<T>;
    template <typename T, size_t s>
    using Array = std::array<T, s>;

    /*
    * Utility concept for some templates.
    */
    template <typename T, typename... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    template <class T>
    const auto makeU = std::make_unique<T>;
    template <class T>
    const auto makeS = std::make_shared<T>;
    template <class T>
    inline UPtr<T> unique(T& t) { return std::make_unique<T>(t); }
    template <class T>
    inline SPtr<T> share(T& t) { return std::make_shared<T>(t); }
    template <typename T>
    inline Ref<T> ref(T& v) { return std::ref<T>(v); }
    template <typename T>
    inline Opt<T> opt(T v) { return std::make_optional<T>(v); }
    template <typename T>
    inline Wrap<T> wrap(T& v) { return Wrap<T>{opt<Ref<T>>(ref<T>(v))}; }

    template <typename T>
    inline T optval(Opt<T> v) { return v.has_value() ? v.value() : T {}; }
    template <typename T>
    inline T& rawref(Ref<T> v) { return v.get(); }
    template <typename T>
    inline T& rawwrap(Wrap<T> v) {
        return rawref<T>(
            v.has_value()
            ? optval<Ref<T>>(v)
            : toref<T>(T {})
            );
    }

    const size_t SIZE8 = 1;
    const size_t SIZE16 = 2;
    const size_t SIZE24 = 3;
    const size_t SIZE32 = 4;
    const size_t SIZE40 = 5;
    const size_t SIZE48 = 6;
    const size_t SIZE64 = 8;
    const size_t SIZE80 = 10;
    const size_t SIZE96 = 12;
    const size_t SIZE112 = 14;
    const size_t SIZE128 = 16;

    static const Map<size_t, size_t> sizes {
        { SIZE8, 8 }, { SIZE16, 16 }, { SIZE24, 24 }, { SIZE32, 32 }, { SIZE40, 40 },
        { SIZE48, 48 }, { SIZE64, 64 }, { SIZE80, 80 }, { SIZE96, 96 },
        { SIZE112, 112 }, { SIZE128, 128 }
    };

}