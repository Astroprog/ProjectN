//
//  SpaceShip.cpp
//  ProjectN
//
//  Created by Peter Rodenkirch on 24.08.15.
//
//

#include "SpaceShip.h"

using namespace cocos2d;

SpaceShip::SpaceShip()
{
    dOmega = 0.0f;
    velocity = Vec2::ZERO;
    gravAcceleration = Vec2::ZERO;
    dot = DrawNode::create();
    dotLine = DrawNode::create();
    orbit = new Orbit::Orbit();
    centralBody = new CelestialBody::CelestialBody();
    acceleration = 0.0f;
    thrust = 0.0f;
    maxThrust = 0.0f;
    thrustBeforeTouch = 0.0f;
    mass = 100.0f; // without fuel
    tank = new Tank();
    tank->initWithCapacity(50.f);
}

SpaceShip::SpaceShip(Sprite *initSprite)
{
    sprite = initSprite;
    dOmega = 0.0f;
    velocity = Vec2::ZERO;
    gravAcceleration = Vec2::ZERO;
    dot = DrawNode::create();
    dotLine = DrawNode::create();
    orbit = new Orbit::Orbit();
    centralBody = new CelestialBody::CelestialBody();
    acceleration = 0.0f;
    thrust = 0.0f;
    maxThrust= 0.0f;
    thrustBeforeTouch = 0.0f;
    mass = 100.0f;
    tank = new Tank();
    tank->initWithCapacity(50.f);
}

void SpaceShip::thrustWillChange()
{
    thrustBeforeTouch = thrust;
}

Vec2 SpaceShip::getDirection()
{
    float rotation = CC_DEGREES_TO_RADIANS(sprite->getRotation());
    return Vec2(sin(rotation), cos(rotation));
}

void SpaceShip::updateVelocity(float dt)
{
    acceleration = thrust / (mass + tank->fuelMass) * 100;
    velocity += acceleration * dt * getDirection() + gravAcceleration * dt;
    tank->updateFuel(thrust, dt, mass + tank->fuelMass);
}

void SpaceShip::updatePosition(float dt)
{
    position += velocity * dt;
}


//    log("%f, %f", direction.x, direction.y);
