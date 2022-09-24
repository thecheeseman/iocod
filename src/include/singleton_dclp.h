#pragma once
#ifndef SINGLETON_DCLP_H
#define SINGLETON_DCLP_H

#include "misc/class_non_copyable.h"

#include <atomic>
#include <mutex>

#define CLASS_SINGLETON_DCLP(class_name) \
    friend class singleton_dclp; \
    class_name(); \
    ~class_name();

template <typename T>
class singleton_dclp {
public:
    static T* instance()
    {
        auto* instance = instance_.load(std::memory_order_acquire);
        if (!instance) {
            std::lock_guard { mutex_ };
            instance = instance_.load(std::memory_order_relaxed);
            if (!instance) {
                instance = new T;
                instance_.store(instance, std::memory_order_release);
            }
        }

        return instance;
    }

    static void destroy()
    {
        if (auto* instance = instance_.exchange(nullptr, std::memory_order_acq_rel))
            delete instance;
    }

protected:
    singleton_dclp() = default;
    virtual ~singleton_dclp() = default;
    CLASS_NON_COPYABLE(singleton_dclp);
    
    inline static std::atomic<T*> instance_ { nullptr };
    inline static std::mutex mutex_;
};

#endif /* SINGLETON_DCLP_H */
