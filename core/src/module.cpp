#include "module.hpp"


#include "ref.hpp"
#include "transform.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    CoreModule::CoreModule(flecs::world &p_world) {
        p_world.module<CoreModule>();

        // Register components
        p_world.add<RefCounted>();
        p_world.add<Transform>();
    }
}