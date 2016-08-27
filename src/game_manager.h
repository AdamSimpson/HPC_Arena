#pragma once

#include <SFML/Graphics.hpp>
#include "ECS_core/entity_manager.h"
#include "ECS_core/entity.h"
#include "ECS_core/system_manager.h"

class GameManager {
public:
  GameManager();
  void play();

private:
  template<typename BuilderType, typename... Args>
  int new_entity(Args... args) {
    int id = entity_manager_.new_entity();
    ecs::Entity ent{entity_manager_, id};
    BuilderType::build(ent, args...);
    return id;
  }

  template<typename SystemType, typename... Args>
  void enable_system(Args&&... args) {
    system_manager_.enable<SystemType>(args...);
  }

  void update_systems(float dt) {
    system_manager_.update(entity_manager_, dt);
  }

  ecs::EntityManager entity_manager_;
  ecs::SystemManager system_manager_;
  sf::RenderWindow window_;
};
