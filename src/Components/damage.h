#pragma once

#include "ECS_core/component.h"

struct Damage: public ecs::Component<Damage> {
  int value = 100;
};
