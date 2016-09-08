#include "Components/position.h"
#include "Components/sprite.h"
#include "Components/directional_animation.h"
#include "Components/direction.h"
#include "ECS_core/entity_manager.h"
#include "renderer.h"
#include "Rendering/sprite_renderable.h"
#include "Rendering/sprite_batch.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include <vector>

Renderer::Renderer(sf::RenderWindow& window) : window_{window} {}

void Renderer::update(ecs::EntityManager& entity_manager, float dt) {
  window_.clear();

  //
  // Loop through all elements to create a map with depth keys and values of
  // An unordered_multimap with keys of the texture_name and value of a sprite_renderable
  //

  // map< depth, unordered_map<texture_name, std::vector<sprite_renderable> > >
  using renderable_map_t = std::unordered_map< std::string, std::vector<SpriteRenderable> >;
  std::map< int, renderable_map_t > depth_map;

  // Loop through standard static sprites
  entity_manager.for_each<Position, Sprite>([&] (Position& position, Sprite& sprite) {
    // Calculate bounding rectangle for sprite
    const int upper_left_x = position.x - sprite.size.x/2.0;
    const int upper_left_y = position.y - sprite.size.y/2.0;
    const sf::FloatRect screen_rect(upper_left_x, upper_left_y, sprite.size.x, sprite.size.y);
    const sf::FloatRect texture_rect(sprite.upper_left.x, sprite.upper_left.y, sprite.size.x, sprite.size.y);


    //
    // Fill map
    //

    // Add new depth renderable_map at given depth if one doesn't exist
    if(!depth_map.count(sprite.depth_layer))
      depth_map.emplace(sprite.depth_layer, renderable_map_t{});

    // Add new texture name, at given depth, if one doesn't exist
    if(!depth_map[sprite.depth_layer].count(sprite.filename))
      depth_map[sprite.depth_layer].emplace(sprite.filename, std::vector<SpriteRenderable>{});

    // Add new SpriteRenderable
    depth_map[sprite.depth_layer][sprite.filename].emplace_back(texture_cache_.get(sprite.filename),
                                                                texture_rect, screen_rect);
  });

  // Loop through directional animations sprites
  // Handle all animation logic here
  entity_manager.for_each<Position, Direction, DirectionalAnimation>([&] (Position& position,
                                                                          Direction& direction,
                                                                          DirectionalAnimation& dir_animation) {
    // Extract sprite from directional animation
    Animation& animation = dir_animation.animations[direction.value];
    const Sprite& sprite = animation.frames[animation.current_frame];

    // Progress animation
    animation.current_frame_ticks++;
    if(animation.current_frame_ticks % animation.ticks_per_frame == 0) {
      animation.current_frame++;
      if(animation.current_frame >= animation.frames.size())
        animation.current_frame = 0;
    }

    // Calculate bounding rectangle for sprite within texture and screen
    const int upper_left_x = position.x - sprite.size.x/2.0;
    const int upper_left_y = position.y - sprite.size.y/2.0;
    const sf::FloatRect screen_rect(upper_left_x, upper_left_y, sprite.size.x, sprite.size.y);
    const sf::FloatRect texture_rect(sprite.upper_left.x, sprite.upper_left.y, sprite.size.x, sprite.size.y);

    //
    // Fill map
    //

    // Add new depth renderable_map at given depth if one doesn't exist
    if(!depth_map.count(sprite.depth_layer))
      depth_map.emplace(sprite.depth_layer, renderable_map_t{});

    // Add new texture name, at given depth, if one doesn't exist
    if(!depth_map[sprite.depth_layer].count(sprite.filename))
      depth_map[sprite.depth_layer].emplace(sprite.filename, std::vector<SpriteRenderable>{});

    // Add new SpriteRenderable
    depth_map[sprite.depth_layer][sprite.filename].emplace_back(texture_cache_.get(sprite.filename),
                                                                texture_rect,
                                                                screen_rect);
  });

  // Build sprite batches from map
  // Each depth/texture combination requires it's own batch
  std::vector< SpriteBatch > sprite_batches;
  for(auto& kvp_depth : depth_map) { // Iterate through each depth
    const auto& renderable_map = kvp_depth.second;
    for(auto& kvp_renderable : renderable_map) { // Iterate through each renderable_map
      const auto& renderable_vector = kvp_renderable.second;
      sprite_batches.emplace_back(renderable_vector);
    }
  }

  // Draw renderable sprites batched by texture from back to front
  for(const auto& sprite_batch : sprite_batches)
    window_.draw(sprite_batch);

  window_.display();
};
