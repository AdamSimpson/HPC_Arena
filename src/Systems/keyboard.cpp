#include "Components/position.h"
#include "Components/velocity.h"
#include "Components/direction.h"
#include "Components/directional_animation.h"
#include "ECS_core/entity_manager.h"
#include "keyboard.h"
#include <SFML/Graphics.hpp>

// very simple keyboard input system for debug purposes
void Keyboard::update(ecs::EntityManager& entity_manager, float dt) {
  entity_manager.for_each_with_id<Position, Direction, DirectionalAnimation, Velocity>([dt] (int id,
                                                                                             Position& position,
                                                                                             Direction& direction,
                                                                                             DirectionalAnimation& dir_animation,
                                                                                             Velocity& velocity) {
    if(id == 0) {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if(direction.value != Orientation::up) {
          dir_animation.animations[direction.value].current_frame = 0;
          direction.value = Orientation::up;
        }
        position.y -= velocity.y;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(direction.value != Orientation::down) {
          dir_animation.animations[direction.value].current_frame = 0;
          direction.value = Orientation::down;
        }
        position.y += velocity.y;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if(direction.value != Orientation::left) {
          dir_animation.animations[direction.value].current_frame = 0;
          direction.value = Orientation::left;
        }
        position.x -= velocity.x;
      }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if(direction.value != Orientation::right) {
          dir_animation.animations[direction.value].current_frame = 0;
          direction.value = Orientation::right;
        }
        position.x += velocity.x;
      }
    }
  });
};
