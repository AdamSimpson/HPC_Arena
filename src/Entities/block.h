#pragma once

#include "ECS_core/entity.h"

class Block {
public:
  static void build(ecs::Entity& block,
                    int x_center=0, int y_center=0);
};
