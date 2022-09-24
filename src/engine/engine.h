#pragma once

namespace iocod {

class engine {
public:
    CLASS_NON_COPYABLE(engine);
    engine() = default;
    ~engine() = default;

    void init();
    void run();
    void shutdown();

protected:
    bool m_quit = false;
};

// namespace iocod
};
