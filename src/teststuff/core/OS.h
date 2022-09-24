#ifndef OS_H
#define OS_H

#include <iostream>

class OS {
public:
    virtual ~OS() {}

    void test()
    {
        std::cout << "test\n";
    }
    
    void cool()
    {
        std::cout << "cool\n";
    }
    
    virtual const char *name() const = 0;
};

extern OS *os;

#endif /* OS_H */
