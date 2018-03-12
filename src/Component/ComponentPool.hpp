#pragma once
#include <tuple>
#include <typeindex>
#include <slot_map.hpp>
#include "../EntityID.hpp"
class ComponentPoolBase {
public:
  ComponentPoolBase( ) = default;
  ComponentPoolBase( const ComponentPoolBase& ) = delete;
  virtual void* Get( EntityID ID ) = 0;
  virtual EntityID Clone( EntityID ID ) = 0;
  virtual std::pair<std::type_index, EntityID> Clone( EntityID ID, World& world ) = 0;
  virtual ~ComponentPoolBase( ) { }
};

template<typename Component>
class ComponentPool : public ComponentPoolBase {
public:
  ComponentPool( ) = default;
  ComponentPool( World &world);
  ~ComponentPool( ) = default;
  virtual void* Get( EntityID ID ) final;
  // @@TODO: On Clone() all ComponentAggregates holding
  // this type of component need to re-fetch pointers if reallocation occurred.
  // The easiest way to do this is probably an event triggering the re-fetch.
  // This could be emitted by the pool on the world, which aggregates could listen for.
  // "OnPointerInvalidation" event or something. Include the type index so they can early out
  // if they don't care about that kind of component.
  virtual EntityID Clone( EntityID ID ) final;
  virtual std::pair<std::type_index, EntityID> Clone( EntityID ID, World &world ) final;
  slot_map<Component> components;
};
#include "ComponentPool.inl"