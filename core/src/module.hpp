#pragma once


#include "thirdparty/flecs/flecs.h"


namespace lixy {

    struct CoreModule {
        CoreModule(flecs::world &p_world);

    private:
        static void _register_ref(flecs::world &p_world);
    };
}