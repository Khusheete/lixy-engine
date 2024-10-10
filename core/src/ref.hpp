#pragma once


#include "thirdparty/flecs/flecs.h"
#include <utility>


namespace lixy {

    class EntityRef {
    public:
        inline flecs::entity get_entity() { return reference; };

        template<class Component>
        const Component *get() const {
            return reference.get<Component>();
        }

        template<class Component>
        Component *get_mut() {
            return reference.get_mut<Component>();
        }

        template<class Component>
        EntityRef &add() {
            reference.add<Component>();
            return *this;
        }

        template<class Component, class ...Args>
        EntityRef &emplace(Args &&...args) {
            reference.emplace<Component>(std::forward<Args>(args)...);
            return *this;
        }

        static EntityRef create_reference(flecs::world &p_world);

        EntityRef() = default;
        EntityRef(const EntityRef &p_other);
        EntityRef(EntityRef &&p_other);
        EntityRef &operator=(EntityRef &&p_other);
        virtual ~EntityRef();

    private:
        EntityRef(flecs::entity p_entity);

    private:
        flecs::entity reference;
    };


    struct RefCounted {
        uint64_t reference_count;
    };
}