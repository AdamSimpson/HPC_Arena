#include "Components/position.h"
#include "Components/velocity.h"
#include "ECS_core/entity_manager.h"
#include "movement.h"

void Movement::update(ecs::EntityManager& entity_manager, float dt) {
  entity_manager.for_each<Position, Velocity>([dt] (Position& position, Velocity& velocity) {
    position += velocity * (int)dt;
  });
};
