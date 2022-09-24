#ifndef IOCOD_SYSTEM_H
#define IOCOD_SYSTEM_H

#include <string>

class System {
public:
    virtual ~System() {}

    virtual void alert(const std::string &message, 
                       const std::string &title) = 0;
    virtual const char *name() = 0;
};

extern System *sys;

#endif /* IOCOD_SYSTEM_H */
