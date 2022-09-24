#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include "misc/class_non_copyable.h"

#define CLASS_SINGLETON(class_name) \
    friend class singleton; \
    class_name(); \
    ~class_name();

template <typename T>
class singleton {
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

protected:
    singleton() = default;
    ~singleton() = default;
    CLASS_NON_COPYABLE(singleton);
};

#endif /* SINGLETON_H */
