#include "camera.hpp"

#include "core/src/transform.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    flecs::entity Camera::create(flecs::world &p_world, const Camera &p_camera) {
        return p_world.entity()
            .add<Transform>()
            .set<Camera>(p_camera);
    }


    flecs::entity Camera::create(flecs::world &p_world) {
        return Camera::create(p_world, {});
    }
}