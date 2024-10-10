#include "module.hpp"


#include "ref.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    CoreModule::CoreModule(flecs::world &p_world) {
        p_world.module<CoreModule>();

        CoreModule::_register_ref(p_world);
    }


    void CoreModule::_register_ref(flecs::world &p_world) {
        // Refcounted
        p_world.add<RefCounted>();
    }
}