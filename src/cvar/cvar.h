#pragma once
#ifndef CVAR_H
#define CVAR_H

#if 0
#include <type_traits>
#include <string_view>

template<typename T> struct is_valid_cvar_type { static const bool value = false; };
template<> struct is_valid_cvar_type<int>   { static const bool value = true; };
template<> struct is_valid_cvar_type<float> { static const bool value = true; };
template<> struct is_valid_cvar_type<std::string_view> { static const bool value = true; };

template<typename T>
class Cvar {
    static_assert(is_valid_cvar_type<T>::value, "invalid cvar type");

    T value_current;
    T value_default;
};
#endif

class cvar {
    
};

#endif /* CVAR_H */
