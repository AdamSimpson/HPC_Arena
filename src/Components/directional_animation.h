#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"
#include <vector>
#include "animation.h"

struct DirectionalAnimation: public ecs::Component<DirectionalAnimation> {
  Animation animations[4]; // Indexed into using direction.h Orientation enum(left, right, up, down)
};