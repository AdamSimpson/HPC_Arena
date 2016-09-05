#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"
#include <string>

struct CollisionBounds: public ecs::Component<CollisionBounds> {
  sf::Vector2u size;
//  sf::Vector2u position_offset{0,0}; // Offset from entity position to center bounding box around
};
