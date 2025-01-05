/*
* Copyright 2024 Souchet Ferdinand
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
* Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


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
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count += 1;
        }
    }


    EntityRef &EntityRef::operator=(const EntityRef &p_other) {
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count -= 1;
        }

        reference = p_other.reference;
        if (reference.is_alive()) {
            reference.get_mut<RefCounted>()->reference_count += 1;
        }
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