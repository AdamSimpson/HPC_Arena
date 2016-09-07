#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"
#include <vector>
#include "sprite.h"

struct Animation: public ecs::Component<Animation> {
  std::vector<Sprite> frames;
  int ticks_per_frame;
  int current_frame = 0;
  int current_frame_ticks = 0;
};
