#pragma once

#include "ECS_core/component.h"
#include <vector>

struct Collisions: public ecs::Component<Collisions> {
  std::vector<int> collision_ids; // Collided with entity ID's
};