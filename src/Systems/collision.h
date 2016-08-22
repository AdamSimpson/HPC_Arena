#pragma once

#include "ECS_core/system.h"
#include "ECS_core/entity.h"
#include "ECS_core/entity_manager.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"

class Collision: public ecs::System {
public:
  Collision(sf::Vector2u screen_dims, int spacing);
  void update(ecs::EntityManager& entity_manager, float dt) override;
private:
  // 2D collision grid of vectors to hold EntityID of contained entities
  std::vector<
          std::vector<
                  std::vector<int>>> grid_;

  int bin_spacing_;

  sf::IntRect entity_bounds(ecs::Entity& entity);
};
