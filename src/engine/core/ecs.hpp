
#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "components/keyinput.hpp"
#include "components/transform.hpp"
#include "components/renderable.hpp"

using entity = std::size_t;

entity max_entity = 0;

struct registry {
    std::vector<lnx_transform_comp> transform;
    std::unordered_map<entity, lnx_transform_comp> transforms;
    std::unordered_map<entity, lnx_renderable_comp> renderable;
    std::unordered_map<entity, keyinputs_component> keys;
};

class RegistryHelper {
    public:
        entity create_entity() {
            static entity entities = 0;
            ++entities;
            max_entity = entities;
            return entities;
        }
};
