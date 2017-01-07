//
//  CelestialBody.cpp
//  ProjectN
//
//  Created by Peter Rodenkirch on 23.08.15.
//
//

#include "CelestialBody.h"

using namespace cocos2d;

CelestialBody::CelestialBody()
{
    position = Vec2::ZERO;
    velocity = Vec2::ZERO;
    acceleration = Vec2::ZERO;
    orbit = DrawNode::create();
    dot = DrawNode::create();

    apoapsis = 0.0f;
    mass = 0.0f;
    size = 0.0f;
}

CelestialBody::CelestialBody(Sprite *initSprite)
{
    sprite = initSprite;
    position = Vec2::ZERO;
    velocity = Vec2::ZERO;
    acceleration = Vec2::ZERO;
    orbit = DrawNode::create();
    dot = DrawNode::create();
    
    apoapsis = 0.0f;
    mass = 0.0f;
    size = 0.0f;
}

void CelestialBody::updateVelocity(float dt)
{
    velocity += acceleration * dt;
}

void CelestialBody::updatePosition(float dt)
{
    position += velocity * dt;
}

