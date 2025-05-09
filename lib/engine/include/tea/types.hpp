#ifndef TYPES_H
#define TYPES_H

#include <expected>
#include <memory>

template <class T>
using Ref = std::reference_wrapper<T>;

template <class T>
using Box = std::unique_ptr<T>;

template <class T>
using Handle = std::shared_ptr<T>;

template <class T, class E>
using Res = std::expected<T, E>;

template <class E>
using Err = std::unexpected<E>;

template <class T>
using Option = std::optional<T>;

#endif
