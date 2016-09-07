#include "Components/position.h"
#include "Components/collision_bounds.h"
#include "ECS_core/entity_manager.h"
#include "boundary.h"

void Boundary::update(ecs::EntityManager& entity_manager, float dt) {
  entity_manager.for_each<Position, CollisionBounds>([this, dt] (Position& position, CollisionBounds& bounds) {
    if(position.x < 0)
      position.x = 0;
    else if(position.x > (size_.x - bounds.size.x))
      position.x = size_.x - bounds.size.x;
    if(position.y < 0)
      position.y = 0;
    else if(position.y > (size_.y - bounds.size.y))
      position.y = size_.y - bounds.size.y;
  });
};
