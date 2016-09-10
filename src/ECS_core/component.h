#pragma once

#include "component_base.h"

namespace ecs {

// Total count of of unique Component<T>'s instantiated
extern int g_component_type_count;

// Use CRTP method to keep track of each unique component types ID
template <typename T>
class Component : public ComponentBase {
public:

  // If an instance of Component<T> hasn't been created we give
  // it the next available ID and increment the global Component<T> type count
  // Return the type_id associated with this component type
  // int type_id = Component<MyType>::type_id()
  // If type_id not created create it
  static int type_id() {
    if(!type_created_) {
      type_id_ = g_component_type_count;
      g_component_type_count++;
      type_created_ = true;
    }

    return type_id_;
  }

  virtual ~Component() = default; // will cause derived class destructor to be called

  static bool type_created_;
  static int type_id_;
};

template <typename T>
bool ecs::Component<T>::type_created_ = false ;
template <typename T>
int ecs::Component<T>::type_id_ = -1;

}
