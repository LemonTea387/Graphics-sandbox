#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <expected>
#include <memory>

template <class T>
using Ref = std::unique_ptr<T>;

template <class T, class E>
using Res = std::expected<T, E>;

template <class T, class E>
using RefRes = std::expected<Ref<T>, E>;

#endif
