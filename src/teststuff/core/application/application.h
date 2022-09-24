#pragma once

#include <cassert>
#include <string>
#include <filesystem>

int main(int argc, char *argv[]);

namespace iocod {
struct AppCommandLineArgs {
    int count;
    char **args = nullptr;

    const char *operator[](size_t index) const
    {
        assert(index < count);
        return args[index];
    }
};

class Application {
public:
    Application(const AppCommandLineArgs &args) :
        command_line_(args)
    {
        instance_ = this;
        std::filesystem::current_path(working_directory_);
    }
    virtual ~Application();

    static Application &get() { return *instance_; }

private:
    void run();

private:
    std::filesystem::path working_directory_;
    AppCommandLineArgs command_line_;

    static Application *instance_;
    friend int ::main(int argc, char *argv[]);
};

}
