#include "block.h"
#include "ECS_core/entity.h"
#include "SFML/System/Vector2.hpp"
#include "Components/position.h"
#include "Components/sprite.h"
#include "Components/collision_bounds.h"
#include "Components/health.h"

void Block::build(ecs::Entity& block,
                   int x_center, int y_center) {
  block.add_component<Position>();
  block.add_component<Sprite>();
  block.add_component<CollisionBounds>();
  block.add_component<Health>();

  block.component<Sprite>().filename = "resources/block.jpg";
  block.component<Sprite>().size = sf::Vector2u{102, 102};

  block.component<CollisionBounds>().size = block.component<Sprite>().size;

  block.component<Position>().x = x_center;
  block.component<Position>().y = y_center;
}
