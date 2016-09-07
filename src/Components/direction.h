#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"
#include <vector>

enum Orientation {
  up,
  down,
  left,
  right
};

struct Direction: public ecs::Component<Direction> {
  Orientation value;
};