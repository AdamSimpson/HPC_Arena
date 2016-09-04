#include "game_manager.h"
#include <SFML/Window.hpp>
#include <iostream>

#include "ECS_core/entity.h"
#include "Entities/player.h"
#include "Entities/block.h"
#include "Systems/movement.h"
#include "Systems/renderer.h"
#include "Systems/boundary.h"
#include "Systems/collision.h"

GameManager::GameManager(): window_{sf::VideoMode(1200,1800), "DebugWindow"} {//window_{sf::VideoMode::getDesktopMode(), "Arena", sf::Style::Fullscreen} {

  window_.setVerticalSyncEnabled(true);

  const int player_count = 4;
  for(int i=0; i<player_count; i++) {
    int player_id = new_entity<Player>(110*i, 100 + i*5);
  }

  for(int j=0; j<1; j++) {
    for (int i = 0; i < 1; i++) {
      int block_id = new_entity<Block>(500 + 100*j, 100 * i + 100);
    }
  }
  enable_system<Movement>();
  enable_system<Boundary>(window_.getSize());
  enable_system<Collision>(window_.getSize(), 100); // The bin spacing must be at least equal to the largest sprite dimension
  enable_system<Renderer>(window_);
}

void GameManager::play() {
  // run the program as long as the window is open

  int count = 0;
  while (window_.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window_.close();
    }

    sf::Clock clock; // starts the clock
    update_systems(1.0);
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    count++;
  }
}
