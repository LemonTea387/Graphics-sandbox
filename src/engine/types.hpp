#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <expected>
#include <memory>
#include "engine/engine_error.hpp"
template <class T>
using Ref = std::unique_ptr<T>;

template <class T>
using Res = std::expected<T, EngineError>;

template <class T>
using RefRes = std::expected<Ref<T>, EngineError>;

#endif
