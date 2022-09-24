#include <iostream>
#include "OS.h"

class OSLinux : public OS {
public:
    virtual const char *name() const 
    { 
        return "Linux"; 
    }
};

OSLinux os_linux;
OS *os = &os_linux;
