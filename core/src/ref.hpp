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

        EntityRef &add(flecs::entity_t p_first, flecs::entity_t p_second);

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