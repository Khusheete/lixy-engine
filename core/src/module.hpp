#pragma once


#include "thirdparty/flecs/flecs.h"


namespace lixy {

    struct CoreModule {
        CoreModule(flecs::world &p_world);
    };
}