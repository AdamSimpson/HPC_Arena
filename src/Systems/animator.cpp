#include "animator.h"
#include "Components/direction.h"
#include "Components/directional_animation.h"

// Set entity sprite based upon direction animation
void Animator::update(ecs::EntityManager &entity_manager, float dt) {

  //@todo loop over non directional animations

  // Loop through all directional animations and set entities sprite
  entity_manager.for_each<Direction, Sprite, DirectionalAnimation>([&] (Direction& direction,
                                                                        Sprite& sprite,
                                                                        DirectionalAnimation& dir_animation) {
    // Extract sprite frame from directional animation
    Animation& animation = dir_animation.animations[direction.value];
    const Sprite& animation_sprite = animation.frames[animation.current_frame];

    // Progress animation frame
    animation.current_frame_ticks++;
    if(animation.current_frame_ticks % animation.ticks_per_frame == 0) {
      animation.current_frame++;
      if(animation.current_frame >= animation.frames.size())
        animation.current_frame = 0;
    }

    // Update sprite
    sprite = animation_sprite;
  });
}