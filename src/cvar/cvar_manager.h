#pragma once
#ifndef CVAR_MANAGER_H
#define CVAR_MANAGER_H

#include "cvar.h"
#include "singleton.h"

class cvar_manager : public singleton<cvar_manager> {
    CLASS_SINGLETON(cvar_manager);
};

#endif /* CVAR_MANAGER_H */
