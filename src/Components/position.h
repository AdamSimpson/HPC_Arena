#pragma once

#include "ECS_core/component.h"
#include "SFML/System/Vector2.hpp"

struct Position: public sf::Vector2i, public ecs::Component<Position> {};
