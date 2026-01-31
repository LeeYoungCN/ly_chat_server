#pragma once
#ifndef COMMON_UTILS_STRING_UTILS_H
#define COMMON_UTILS_STRING_UTILS_H

#include <cstdarg>
#include <string>

#include "common/types/type_traits.h"

namespace common::string {
std::string va_list_to_string(const char* format, va_list args);
bool string_is_null_or_empty(const char* str);

template <typename T, std::enable_if_t<type_traits::is_convertible_to_string_v<T>, int> = 0>
std::string type_to_string(const T& type)
{
    std::string msg;
    if constexpr (type_traits::is_direct_string_type_v<T>) {
        msg = std::string(type);
    } else if constexpr (type_traits::is_numeric_type_v<T>) {
        msg = std::to_string(type);
    }
    return msg;
}

}  // namespace common::string

#endif  // COMMON_UTILS_STRING_UTILS_H
