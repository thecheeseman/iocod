#include <iostream>
#include "OS.h"

class OSWindows : public OS {
public:
    virtual const char *name() const
    {
        return "Windows";
    }
};

OSWindows os_windows;
OS *os = &os_windows;
