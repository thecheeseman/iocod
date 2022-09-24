#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <string>

class System {
public:
    virtual ~System() {}

    virtual void print(const std::string &message) = 0;
    virtual void message_box(const std::string &message,
                             const std::string &title) = 0;
};

class SystemNull : public System {
public:
    virtual void print(const std::string &message) {}
    virtual void message_box(const std::string &message,
                             const std::string &title) {}
};

extern System *sys;

class SystemLocator {
public:
    static System &get_system()
    {
        return *system_;
    }

    static void provide(System *system)
    {
        if (system == nullptr)
            system_ = &nullsystem_;
        else
            system_ = system;
    }

private:
    static SystemNull nullsystem_;
    static System *system_;
};

#endif /* CORE_SYSTEM_H */
