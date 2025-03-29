#ifndef TYPES_H
#define TYPES_H

#include <expected>
#include <memory>

template <class T>
using Ref = std::unique_ptr<T>;

template <class T>
using Handle = std::shared_ptr<T>;

template <class T, class E>
using Res = std::expected<T, E>;

#endif
