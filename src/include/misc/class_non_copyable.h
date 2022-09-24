// from UE5
#pragma once

/**
 * @brief Delete a type's copy & move constructors (non-copyable).
 *
 * Example usage:
 * @code
 * class MyClass {
 * public:
 *      CLASS_NON_COPYABLE(MyClass)
 *      MyClass() = default;
 *      // ...
 * };
 * @endcode
 */
#define CLASS_NON_COPYABLE(type) \
    type(const type&) = delete; \
    type(type&&) noexcept = delete; \
    type& operator=(const type&) = delete; \
    type& operator=(type&&) noexcept = delete;
