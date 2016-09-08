#pragma once

#include <SFML/Graphics.hpp>

// Struct containing all the information required to render a sprite
struct SpriteRenderable {
  const sf::Texture& texture;
  const sf::FloatRect texture_rect;  // Rect describing location of sprite within texture(in pixel coordinates)
  const sf::FloatRect screen_rect;   // Rect describing where on screen sprite is drawn(in pixel coordinates)
  SpriteRenderable(const sf::Texture& texture, const sf::FloatRect& texture_rect, const sf::FloatRect& screen_rect):
                   texture{texture}, texture_rect{texture_rect}, screen_rect{screen_rect} {}
};
