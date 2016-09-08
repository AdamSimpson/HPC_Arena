#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "sprite_batch.h"
#include "Components/sprite.h"
#include "Components/position.h"
#include "sprite_renderable.h"

SpriteBatch::SpriteBatch(const std::vector<SpriteRenderable>& sprite_renderables):
  texture_{sprite_renderables[0].texture}
{

  std::cout<<"sf::Quads deprecated!\n";
  std::cout<<"sprite count: "<<sprite_renderables.size()<<std::endl;

  vertices_.setPrimitiveType(sf::Quads);
  vertices_.resize(sprite_renderables.size() * 4);

  for(int i=0; i<sprite_renderables.size(); i++) {
    // get a pointer to the current sprites quad
    sf::Vertex* quad = &vertices_[i*4];
    const SpriteRenderable& sprite = sprite_renderables[i];

    //
    // set quad veticies
    //

    // top left
    quad[0].position = sf::Vector2f(sprite.screen_rect.left,
                                    sprite.screen_rect.top);
    // top right
    quad[1].position = sf::Vector2f(sprite.screen_rect.left + sprite.screen_rect.width,
                                    sprite.screen_rect.top);
    // bottom right
    quad[2].position = sf::Vector2f(sprite.screen_rect.left + sprite.screen_rect.width,
                                    sprite.screen_rect.top + sprite.screen_rect.height);
    // bottom left
    quad[3].position = sf::Vector2f(sprite.screen_rect.left,
                                    sprite.screen_rect.top + sprite.screen_rect.height);

    //
    // set quad texture coordinates(in pixel units, not normalized)
    // Here we assume we're using the entire texture
    //

    // top left
    quad[0].texCoords = sf::Vector2f(sprite.texture_rect.left, sprite.texture_rect.top);
    // top right
    quad[1].texCoords = sf::Vector2f(sprite.texture_rect.left + sprite.texture_rect.width, sprite.texture_rect.top);
    // bottom right
    quad[2].texCoords = sf::Vector2f(sprite.texture_rect.left + sprite.texture_rect.width,
                                     sprite.texture_rect.top + sprite.texture_rect.height);
    // bottom left
    quad[3].texCoords = sf::Vector2f(sprite.texture_rect.left,
                                     sprite.texture_rect.top + sprite.texture_rect.height);
  }
}

void SpriteBatch::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // apply the transform
  states.transform *= getTransform();

  // apply the tileset texture
  states.texture = &texture_;

  // draw the vertex array
  target.draw(vertices_, states);
}
