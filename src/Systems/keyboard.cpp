#include "Components/position.h"
#include "Components/velocity.h"
#include "ECS_core/entity_manager.h"
#include "keyboard.h"
#include <SFML/Graphics.hpp>

// very simple keyboard input system for debug purposes
void Keyboard::update(ecs::EntityManager& entity_manager, float dt) {
  entity_manager.for_each_with_id<Position, Velocity>([dt] (int id, Position& position, Velocity& velocity) {
    if(id == 0) {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        position.y -= velocity.y;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        position.y += velocity.y;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        position.x -= velocity.x;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        position.x += velocity.x;
      }
    }
  });
};
