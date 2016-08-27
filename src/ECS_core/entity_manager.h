#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <experimental/tuple> // Required for std::apply
#include "component_base.h"
#include "component.h"

namespace ecs {

const int MAX_COMPONENTS = 32;

// Store all entities and their components

class EntityManager {
private:
  // component pool:
  // Each element of components_ is a vector of a particular component type
  // To get the vector of Position component*'s': components_[Position::type_id()]
  std::array< std::vector< std::unique_ptr<ComponentBase> >,
              MAX_COMPONENTS
            > components_;

  // Map between entity ID and component pool indexes:
  // Each element of component_indicies_ is an array of indices indicating
  // Where in the components_[] vector an entities component lives
  // component_indicies_[entity_id][component_type_index] =  index in components_[type]
  std::vector< std::array<int, MAX_COMPONENTS> > component_indices_;

  // Each element of component_mask_ is a bitset of the entities components
  // component_mask_[entity_id] = mask of entity_id's component types
  std::vector< std::bitset<MAX_COMPONENTS> > component_masks_;

  // Vector of entity ID's that already exist but are available for use
  std::vector<int> vacant_ids_;

  // Array indexed by component type, containing a vector of
  // the specified components vacancies
  std::array< std::vector<int>, MAX_COMPONENTS> vacant_components_;

public:
  int max_entity_id() {
    return component_masks_.size() - 1;
  }

  int new_entity() {
    int new_entity_id;

    // Return vacanct entityID is one exists
    if(vacant_ids_.size()) {
      new_entity_id = vacant_ids_.back();
      vacant_ids_.pop_back();
    }
    else { // Create a new entityID and storage
      component_indices_.emplace_back();
      component_masks_.emplace_back();
      new_entity_id =  this->max_entity_id();
    }

    return new_entity_id;
  }

  // Invalidate the entity by adding it's ID to vacancy list
  // it's components the vacancy list, and unsetting it's component mask
  void invalidate_entity(int entity_id) {
    // Allow ID to be reused
    vacant_ids_.emplace_back(entity_id);

    //
    // Allow components belonging to entity to be reused
    //

    // Iterate over all components and mark as vacant if entity contains them
    for(int comp=0; comp<g_component_type_count; comp++) {
      // If the entity contains the component
      if(component_masks_[entity_id].test(comp)) {
        // add the index of the component in the component pool to vacancy list
        int free_index = component_indices_[entity_id][comp];
        vacant_components_[comp].emplace_back(free_index);
      }
    }

    // Unset component mask - this effectively invalides the entity
    component_masks_[entity_id].reset();
  }

  // Add component T to entity with id_
  template<typename T>
  void add_component(int entity_id) {
    const auto component_id = T::type_id();

    int component_index;
    // Check for component vacancy
    if( vacant_components_[component_id].size() ) {
      component_index = vacant_components_[component_id].back();
      vacant_components_[component_id].pop_back();
      // Reset component value
      components_[component_id][component_index] = std::make_unique<T>();
    }
    else { // Create new component
      // Add new component of type T
      components_[component_id].emplace_back(std::make_unique<T>());
      component_index = components_[component_id].size() - 1;
    }

    // Map component ID to component array index
    component_indices_[entity_id][component_id] = component_index;

    // Update entities component mask
    component_masks_[entity_id].set(component_id);
  }

  // Remove component T to entity with id_
  template<typename T>
  void remove_component(int entity_id) {
    const auto component_id = T::type_id();

    // add the index of the component in the component pool to vacancy list
    int free_index = component_indices_[entity_id][component_id];
    vacant_components_[component_id].emplace_back(free_index);

    // Remove component from entities bitmask
    component_masks_[entity_id].reset(component_id);
  }

  // fetch reference to component T
  template<typename T>
  T& fetch_component(int entity_id) {
    const auto component_id = T::type_id();

    // Must assert that the component is valid, else an invalid component_index may be returned
    assert(component_masks_[entity_id].test(component_id));

    const auto component_index = component_indices_[entity_id][component_id];
    return *(static_cast<T*>(components_[component_id][component_index].get()));
  }

  // fetch tuple of reference to components Components...
  template<typename... Components>
  auto fetch_components(int entity_id) {
    return std::forward_as_tuple(this->fetch_component<Components>(entity_id)...);
  }

  // Test if entity has specified components
  template<typename... Components>
  bool has_components(int entity_id) {
    std::bitset<MAX_COMPONENTS> test_bits;

    // Call test_bits.set(T::type_id()) for each component in Components...
    [](...){}((test_bits.set(Components::type_id()))...); // I'm 80% sure this works

    return (test_bits & component_masks_[entity_id]) == test_bits;
  }

  // Iterate through entity ID's and apply body lambda to those matching Component types
  template<typename... Components, typename Body>
  void for_each(Body body) {
    for(int id=0; id<(max_entity_id()+1); id++) {
      if( has_components<Components...>(id) ) {
        std::experimental::apply(body, fetch_components<Components...>(id));
      }
    }
  }

  // Iterate through entity ID's and apply body lambda to those matching Component types
  // with_id includes the entities ID as the first argument to body
  template<typename... Components, typename Body>
  void for_each_with_id(Body body) {
    for(int id=0; id<(max_entity_id()+1); id++) {
      if( has_components<Components...>(id) ) {
        std::tuple<int> entity_id(id);
        std::experimental::apply(body,
                                 std::tuple_cat(entity_id, fetch_components<Components...>(id)) );
      }
    }
  }

};
}
