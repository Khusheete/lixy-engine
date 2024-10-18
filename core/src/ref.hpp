#pragma once


#include "thirdparty/flecs/flecs.h"
#include <utility>


namespace lixy {

    class EntityRef {
    public:
        inline flecs::entity get_entity() { return reference; };

        template<typename Component>
        const Component *get() const {
            return reference.get<Component>();
        }

        template<typename Component>
        Component *get_mut() {
            return reference.get_mut<Component>();
        }

        template<typename Component>
        bool has() const {
            return reference.has<Component>();
        }

        template<typename Component>
        EntityRef &set(const Component &p_component) {
            reference.set<Component>(p_component);
            return *this;
        }

        template<typename Component>
        EntityRef &add() {
            reference.add<Component>();
            return *this;
        }

        template<typename Component>
        EntityRef &remove() {
            reference.remove<Component>();
            return *this;
        }

        template<class Component, class ...Args>
        EntityRef &emplace(Args &&...p_args) {
            reference.emplace<Component>(std::forward<Args>(p_args)...);
            return *this;
        }

        bool is_alive() const;

        static EntityRef create_reference(flecs::world &p_world);

        EntityRef() = default;
        EntityRef(const EntityRef &p_other);
        EntityRef &operator=(const EntityRef &p_other);
        EntityRef(EntityRef &&p_other);
        EntityRef &operator=(EntityRef &&p_other);
        virtual ~EntityRef();

    private:
        EntityRef(flecs::entity p_entity);

    private:
        flecs::entity reference = flecs::entity::null();
    };


    struct RefCounted {
        uint64_t reference_count;
    };
}