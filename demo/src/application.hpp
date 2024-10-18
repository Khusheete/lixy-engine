#pragma once

#include <memory>

#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "renderer/src/primitives/context.hpp"
#include "renderer/src/renderer.hpp"
#include "thirdparty/flecs/flecs.h"


class DemoApplication {
public:
    void main_loop();

    DemoApplication();
    virtual ~DemoApplication();

private:
    flecs::world world;

    flecs::entity rectangle;
    flecs::entity camera;

    float rotation_angle = 0.0;
};