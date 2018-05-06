#pragma once

#include "Entity/EntitiesWith.hpp"

#include <Components/Transform.h>
#include <Components/Body.h>

class Integration
{
public:
	EntitiesWith<Transform, Body> Entities;

	void Update(float dt);

};