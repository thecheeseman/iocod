#include "iocod.h"

namespace iocod {

void engine::init()
{
    try {
        // try to do our initialization
        // if we fail during this step, just quit

        // -- this process is identical for 1.1/1.5/1.51
        // system init
        // setup paths
        // create console
        // setup timing
        // com_init
        // pb init
        // --
    } catch (const exception&) {
        system::error("Engine::init() failed");
    } catch (const fatal_exception&) {
        system::error("Engine::init() failed");
    }
}

void engine::run()
{
    while (!m_quit) {
        try {
            // handle events / message pumps
            // handle input
            // command buffer exec
            // server frame
            // client frame
        } catch (const exception&) {
            // errors can be script compile/run time errors
            // or other non-fatal stuff we can recover from
        }
    }
}

void engine::shutdown()
{

}

// namespace iocod
};
