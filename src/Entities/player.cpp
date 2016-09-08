#include "player.h"
#include "ECS_core/entity.h"
#include "SFML/System/Vector2.hpp"
#include "Components/position.h"
#include "Components/velocity.h"
#include "Components/sprite.h"
#include "Components/collision_bounds.h"
#include "Components/health.h"
#include "Components/direction.h"
#include "Components/directional_animation.h"

void Player::build(ecs::Entity& player,
                   int x_center, int y_center) {
  player.add_component<Position>();
  player.add_component<Velocity>();
  player.add_component<CollisionBounds>();
  player.add_component<Health>();
  player.add_component<Direction>();
  player.add_component<DirectionalAnimation>();

  player.component<Direction>().value = left;

  // Left animation
  Sprite left_sprite;
  left_sprite.filename = "resources/tank-directions.jpg";
  left_sprite.upper_left = {0,0};
  left_sprite.size = {102, 62};

  Animation left_animation;
  left_animation.ticks_per_frame = 1;
  left_animation.current_frame = 0;
  left_animation.frames.push_back(left_sprite);

  // Right animation
  Sprite right_sprite;
  right_sprite.filename = "resources/tank-directions.jpg";
  right_sprite.upper_left = {102,0};
  right_sprite.size = {102, 62};

  Animation right_animation;
  right_animation.ticks_per_frame = 1;
  right_animation.current_frame = 0;
  right_animation.frames.push_back(right_sprite);

  // Up animation
  Sprite up_sprite;
  up_sprite.filename = "resources/tank-directions.jpg";
  up_sprite.upper_left = {204,0};
  up_sprite.size = {102, 62};

  Animation up_animation;
  up_animation.ticks_per_frame = 1;
  up_animation.current_frame = 0;
  up_animation.frames.push_back(up_sprite);

  // Down animation
  Sprite down_sprite;
  down_sprite.filename = "resources/tank-directions.jpg";
  down_sprite.upper_left = {306,0};
  down_sprite.size = {102, 62};

  Animation down_animation;
  down_animation.ticks_per_frame = 1;
  down_animation.current_frame = 0;
  down_animation.frames.push_back(down_sprite);

  player.component<DirectionalAnimation>().animations[Orientation ::left] = left_animation;
  player.component<DirectionalAnimation>().animations[Orientation ::right] = right_animation;
  player.component<DirectionalAnimation>().animations[Orientation ::up] = up_animation;
  player.component<DirectionalAnimation>().animations[Orientation ::down] = down_animation;

  player.component<CollisionBounds>().size = {100, 60};

  player.component<Position>().x = x_center;
  player.component<Position>().y = y_center;

  player.component<Velocity>().x = 20.0;
  player.component<Velocity>().y = 20.0;
}
