#pragma once

#include "ECS_core/component.h"

struct Health: public ecs::Component<Health> {
  int current = 100;
  int max = 100;
};
