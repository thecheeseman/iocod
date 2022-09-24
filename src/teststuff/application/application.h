#ifndef IOCOD_APPLICATION_H
#define IOCOD_APPLICATION_H

class Application {
public:
    virtual ~Application() {}

    virtual bool init() = 0;
    virtual bool init_gui() = 0;
    virtual bool frame_begin() = 0;
    virtual bool frame_gui() = 0;
    virtual bool frame_render() = 0;
    virtual bool frame_end() = 0;
    virtual void shutdown() = 0;

    void request_exit()
    {
        active = false;
    }

    bool is_active()
    {
        return active;
    }

protected:
    bool active = false;
};

extern Application *app;

#endif /* IOCOD_APPLICATION_H */
