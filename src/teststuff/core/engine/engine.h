#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

class Engine {
public:
    virtual ~Engine();

    virtual void init();
    virtual void shutdown();
};

#endif /* CORE_ENGINE H */
