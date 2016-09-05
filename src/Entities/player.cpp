#include "player.h"
#include "ECS_core/entity.h"
#include "SFML/System/Vector2.hpp"
#include "Components/position.h"
#include "Components/velocity.h"
#include "Components/sprite.h"
#include "Components/collision_bounds.h"
#include "Components/health.h"

void Player::build(ecs::Entity& player,
                   int x_center, int y_center) {
  player.add_component<Position>();
  player.add_component<Velocity>();
  player.add_component<Sprite>();
  player.add_component<CollisionBounds>();
  player.add_component<Health>();

  player.component<Sprite>().filename = "resources/tank.jpg";
  player.component<Sprite>().size = sf::Vector2u{100, 60};

  player.component<CollisionBounds>().size = player.component<Sprite>().size;

  player.component<Position>().x = x_center;
  player.component<Position>().y = y_center;

  player.component<Velocity>().x = 20.0;
  player.component<Velocity>().y = 0.0;
}
