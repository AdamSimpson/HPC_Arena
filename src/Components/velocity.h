#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"

struct Velocity: public sf::Vector2i, public ecs::Component<Velocity> {};
