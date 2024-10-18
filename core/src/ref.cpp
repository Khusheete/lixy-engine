#include "ref.hpp"

#include "thirdparty/flecs/flecs.h"


namespace lixy {

    bool EntityRef::is_alive() const {
        return reference.is_alive();
    }


    EntityRef EntityRef::create_reference(flecs::world &p_world) {
        flecs::entity reference = p_world.entity()
            .set<RefCounted>({0});

        return EntityRef(reference);
    }


    // EntityRef::EntityRef() {}


    EntityRef::EntityRef(const EntityRef &p_other)
        : reference(p_other.reference)
    {
        reference.get_mut<RefCounted>()->reference_count += 1;
    }


    EntityRef &EntityRef::operator=(const EntityRef &p_other) {
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count -= 1;
        }

        reference = p_other.reference;
        reference.get_mut<RefCounted>()->reference_count += 1;
        return *this;
    }


    EntityRef::EntityRef(EntityRef &&p_other)
        : reference(p_other.reference)
    {
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count += 1;
        }
    }


    EntityRef &EntityRef::operator=(EntityRef &&p_other) {
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count -= 1;
        }

        reference = p_other.reference;
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count += 1;
        }
        return *this;
    }


    EntityRef::EntityRef(flecs::entity p_entity)
        : reference(p_entity)
    {
        reference.get_mut<RefCounted>()->reference_count += 1;
    }

    EntityRef::~EntityRef() {
        if (!reference.is_alive()) return;

        RefCounted *ref_count = reference.get_mut<RefCounted>();
        ref_count->reference_count -= 1;

        if (ref_count->reference_count == 0) {
            reference.destruct();
        }
    }
}