#pragma once

#include "ECS_core/entity.h"

class Player {
public:
  static void build(ecs::Entity& player,
                    int x_center=0, int y_center=0);
};
