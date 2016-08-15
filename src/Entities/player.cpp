#include "player.h"
#include "ECS_core/entity.h"
#include "SFML/System/Vector2.hpp"
#include "Components/position.h"
#include "Components/velocity.h"
#include "Components/sprite.h"

ecs::Entity& Player::build(ecs::Entity& player) {
  player.add<Position>();
  player.add<Velocity>();
  player.add<Sprite>();

  player.component<Sprite>().filename = "../resources/tank.jpg";
  player.component<Sprite>().size = sf::Vector2u{100, 60};

  player.component<Position>().x = 0.0;
  player.component<Position>().y = 0.0;

  player.component<Velocity>().x = 10.0;
  player.component<Velocity>().y = 10.0;

  return player;
}
