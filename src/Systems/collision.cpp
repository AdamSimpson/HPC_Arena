#include <cmath>
#include "collision.h"
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"
#include "ECS_core/entity_manager.h"
#include "ECS_core/entity.h"
#include "Components/position.h"
#include "Components/velocity.h"
#include "Components/collision_bounds.h"

// @TODO lots of re-use here : should probably store bounding rectangles or something
// @TODO SFML intersects() is overkill to test if two AABB's have intersected in our case

Collision::Collision(sf::Vector2u screen_dims, int spacing) {
  bin_spacing_ = spacing;

  // Collision grid dimensions
  // a 1 bin padding is added all around to take care of sprites leaving the screen
  sf::Vector2u grid_dim((unsigned int)std::ceil(screen_dims.x / (float)spacing) + 2,
                        (unsigned int)std::ceil(screen_dims.y / (float)spacing) + 2
  );

  // Allocate grid
  grid_.resize(grid_dim.y);
  for(auto& row : grid_) {
    row.resize(grid_dim.x);
  }

}

// Return an Rect defining the hitbox for a given entity in offset coordinates
sf::IntRect Collision::entity_bounds(ecs::Entity& entity) {
  const auto position = entity.component<Position>();
  const auto collision_bounds = entity.component<CollisionBounds>();

  // Offset position by a bin width so offscreen sprites don't produce negative grid indices
  const float offset_center_x = position.x + (float)bin_spacing_;
  const float offset_center_y = position.y + (float)bin_spacing_;

  // Calculate corners of collision bounding box
  const int bounds_width = collision_bounds.size.x;
  const int bounds_height = collision_bounds.size.y;
  const int upper_left_x = static_cast<int>(offset_center_x - bounds_width/2.0);
  const int upper_left_y = static_cast<int>(offset_center_y - bounds_height/2.0);

  return sf::IntRect(upper_left_x, upper_left_y, bounds_width, bounds_height);
}

void Collision::update(ecs::EntityManager& entity_manager, float dt) {
  // Reset collision grid
  for(auto& row : grid_) {
    for(auto& cell : row) {
      cell.clear();
    }
  }

  // Place entities in appropriate bins
  entity_manager.for_each_with_id<Position, CollisionBounds>([this] (int entity_id,
                                                                    Position& position,
                                                                    CollisionBounds& collision_bounds) {
    //
    // Place each corner of bounding box in collision grid bin
    //

    // Offset position by a bin width so offscreen sprites don't produce negative grid indices
    const float offset_center_x = position.x + (float)bin_spacing_;
    const float offset_center_y = position.y + (float)bin_spacing_;

    // Calculate corners of collision bounding box
    const int bounds_width = collision_bounds.size.x;
    const int bounds_height = collision_bounds.size.y;
    const int upper_left_x = static_cast<int>(offset_center_x - bounds_width/2.0);
    const int upper_left_y = static_cast<int>(offset_center_y - bounds_height/2.0);

    // Upper left
    const int bin_ul_x = upper_left_x/bin_spacing_;
    const int bin_ul_y = upper_left_y/bin_spacing_;
    // Upper right
    const int bin_ur_x = (upper_left_x + bounds_width) / bin_spacing_;
    const int bin_ur_y = bin_ul_y;
    // Lower right
    const int bin_lr_x = bin_ur_x;
    const int bin_lr_y = (upper_left_y - bounds_height) / bin_spacing_;
    // Lower left
    const int bin_ll_x = bin_ul_x;
    const int bin_ll_y = bin_lr_y;

    // Add entities ID to each grid bin it's in
    // Make sure not to add entity multiple times to grid bin
    grid_[bin_ul_y][bin_ul_x].emplace_back(entity_id);

    if(bin_ur_x != bin_ul_x)
      grid_[bin_ur_y][bin_ur_x].emplace_back(entity_id);
    if(bin_lr_y != bin_ur_y)
      grid_[bin_lr_y][bin_lr_x].emplace_back(entity_id);
    if(bin_ll_y != bin_ul_y)
      grid_[bin_ll_y][bin_ll_x].emplace_back(entity_id);
  });

  // Loop over entities that have a velocity component
  // and check for any collisions, mark both collider/collidee with collided component
  // It is assumed here there are no interactions between static(no velocity component) objects
  entity_manager.for_each_with_id<Position, Velocity, CollisionBounds>([&] (int entity_id,
                                                                             Position& position,
                                                                             Velocity& velocity,
                                                                             CollisionBounds& collision_bounds) {
    // Compute grid position of each corner

    // Offset position by a bin width so offscreen sprites don't produce negative grid indices
    const float offset_center_x = position.x + (float)bin_spacing_;
    const float offset_center_y = position.y + (float)bin_spacing_;

    // Calculate corners of collision bounding box
    const int bounds_width = collision_bounds.size.x;
    const int bounds_height = collision_bounds.size.y;
    const int upper_left_x = static_cast<int>(offset_center_x - bounds_width/2.0);
    const int upper_left_y = static_cast<int>(offset_center_y - bounds_height/2.0);

    // Upper left
    const int bin_ul_x = upper_left_x/bin_spacing_;
    const int bin_ul_y = upper_left_y/bin_spacing_;
    // Upper right
    const int bin_ur_x = (upper_left_x + bounds_width) / bin_spacing_;
    const int bin_ur_y = bin_ul_y;
    // Lower right
    const int bin_lr_x = bin_ur_x;
    const int bin_lr_y = (upper_left_y - bounds_height) / bin_spacing_;
    // Lower left
    const int bin_ll_x = bin_ul_x;
    const int bin_ll_y = bin_lr_y;

    // bounding rectangle
    const sf::IntRect ent_rect(upper_left_x, upper_left_y, bounds_width, bounds_height);

    // Check each bin for entities
    // If two dynamic entities collide the convention is that the lowest ID will create the collision entry
    // The higher ID will ignore the collision to ensure no duplicate collision instances
    for(const int id : grid_[bin_ul_y][bin_ul_x]) {
      ecs::Entity test_entity(entity_manager, id);
      const auto test_rect = entity_bounds(test_entity);
      const bool collided = ent_rect.intersects(test_rect);
    }

    if(bin_ur_x != bin_ul_x) {
      for(const int id : grid_[bin_ur_y][bin_ur_x]) {
        ecs::Entity test_entity(entity_manager, id);
        const auto test_rect = entity_bounds(test_entity);
        const bool collided = ent_rect.intersects(test_rect);
      }
    }
    if(bin_lr_y != bin_ur_y) {
      for (const int id : grid_[bin_lr_y][bin_lr_x]) {
        ecs::Entity test_entity(entity_manager, id);
        const auto test_rect = entity_bounds(test_entity);
        const bool collided = ent_rect.intersects(test_rect);
      }
    }
    if(bin_ll_y != bin_ul_y) {
      for (const int id : grid_[bin_ll_y][bin_ll_x]) {
        ecs::Entity test_entity(entity_manager, id);
        const auto test_rect = entity_bounds(test_entity);
        const bool collided = ent_rect.intersects(test_rect);
      }
    }

  });

  // Loop over grid and detect collisions, add collided component which holds entity collided with
  // something something don't duplicate collisions
/*
  int collisions =0;
  for(auto& row : grid_) {
    for(auto& cell : row) {
      if(cell.size())
        collisions+=cell.size();
    }
  }
  std::cout<<"entities in cells: "<<collisions<<std::endl;

    // Apply damage to entities that have collided that have health
    // Or if both entities have health move them apart
    entity_manager.for_each<Collided, Health>([dt] (Collided& collided, Health& health) {

    });

    // Remove damage causing entities if they've collided with an entity having health
    entity_manager.for_each<Collided, Damage>([dt] (Collided& collided, Health& health) {

    });
    */
};
