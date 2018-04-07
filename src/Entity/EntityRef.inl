#pragma once
#include "../EntityRef.hpp"
#include "../World.hpp"
#include "../WorldEvents.hpp"
template<typename Component>
bool EntityRef::Has( ) const {
  return Has<Component>( std::type_index( typeid( Component ) ) );
}
template<typename Component, typename... Components>
bool EntityRef::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}

template<typename Component>
inline Component& EntityRef::Get( ) {
  return m_World->GetEntity( m_ID )->Get<Component>( );
}

template<typename Component>
inline const Component& EntityRef::Get( ) const {
  return m_World->GetEntity( m_ID )->Get<Component>( );
}

template<typename Component, typename... Args>
Component& EntityRef::Add(Args&&... args) {
  if (this->Has<Component>()) return this->Get<Component>();

  Component& component{ m_World->GetEntity(m_ID).Add(std::forward<Args>(args)...) };
  m_World->Emit(ComponentAddedEvent{ *this, std::type_index(typeid(Component)) });
  return component;
}
template<typename Component>
void EntityRef::Remove() {
  if (!this->Has<Component>()) return;

  m_World->Emit(ComponentRemovedEvent{ *this, std::type_index(typeid(Component)) });
  m_World->OnNext([=](const FrameEndEvent &){ m_World->GetEntity(m_ID).Remove<Component>(); });
}