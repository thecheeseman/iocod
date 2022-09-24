// from UE5
#pragma once

/**
 * @def ENUM_CLASS_FLAGS
 * @brief Macro for generating all bitwise operators for enum classes
 * so that they can be used as regular enums if need be.
 */
#define ENUM_CLASS_FLAGS(E)                                                     \
	inline E& operator|=(E& l, E r)                                             \
    {                                                                           \
        return l = (E)((__underlying_type(E)) l | (__underlying_type(E)) r);    \
    }                                                                           \
	inline E& operator&=(E& l, E r)                                             \
    {                                                                           \
        return l = (E)((__underlying_type(E)) l & (__underlying_type(E)) r);    \
    }                                                                           \
	inline E& operator^=(E& l, E r)                                             \
    {                                                                           \
        return l = (E)((__underlying_type(E)) l ^ (__underlying_type(E)) r);    \
    }                                                                           \
	inline constexpr E operator| (E l, E r)                                     \
    {                                                                           \
        return (E)((__underlying_type(E)) l | (__underlying_type(E)) r);        \
    }                                                                           \
	inline constexpr E operator& (E l, E r)                                     \
    {                                                                           \
        return (E)((__underlying_type(E)) l & (__underlying_type(E)) r);        \
    }                                                                           \
	inline constexpr E operator^ (E l, E r)                                     \
    {                                                                           \
        return (E)((__underlying_type(E)) l ^ (__underlying_type(E)) r);        \
    }                                                                           \
	inline constexpr bool operator! (E e)                                       \
    {                                                                           \
        return !(__underlying_type(E)) e;                                       \
    }                                                                           \
	inline constexpr E operator~ (E e)                                          \
    {                                                                           \
        return (E)~(__underlying_type(E))e;                                     \
    }

template<typename E>
constexpr bool ENUM_HAS_ALL_FLAGS(E flags, E e)
{
    return (((__underlying_type(E)) flags) & (__underlying_type(E)) e) ==
        ((__underlying_type(E)) e);
}

template<typename E>
constexpr bool ENUM_HAS_ANY_FLAGS(E flags, E e)
{
    return (((__underlying_type(E)) flags) & (__underlying_type(E)) e) != 0;
}

template<typename E>
void ENUM_ADD_FLAGS(E& flags, E add)
{
    flags |= add;
}

template<typename E>
void ENUM_REMOVE_FLAGS(E& flags, E remove)
{
    flags &= ~remove;
}
