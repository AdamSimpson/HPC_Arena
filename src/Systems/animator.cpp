#include "animator.h"
#include "Components/direction.h"
#include "Components/directional_animation.h"

// Set entity sprite based upon direction animation
void Animator::update(ecs::EntityManager &entity_manager, float dt) {

  // Loop through all directional animations and set entities current animation
  entity_manager.for_each<DirectionalAnimation, Direction, Animation>([&] (DirectionalAnimation& dir_animation,
                                                                           Direction& direction,
                                                                           Animation& animation) {
    // Set entity animation based on directional animation
    animation = dir_animation.animations[direction.value];
  });

  // Loop through all animations and set entities sprite
  entity_manager.for_each<Animation, Sprite>([&] (Animation& animation,
                                                  Sprite& sprite) {
    // Extract sprite frame from animation
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