//
//  SpaceShip.h
//  ProjectN
//
//  Created by Peter Rodenkirch on 24.08.15.
//
//

#ifndef __ProjectN__SpaceShip__
#define __ProjectN__SpaceShip__

#include "cocos2d.h"
#include "Orbit.h"
#include "CelestialBody.h"
#include "Tank.h"

class SpaceShip : public cocos2d::Ref {
    
public:
    
    SpaceShip();
    SpaceShip(cocos2d::Sprite *initSprite);
    void setDOmega(float o);
    float getDOmega();
    void thrustWillChange();
    void updateVelocity(float dt);
    void updatePosition(float dt);
    cocos2d::Vec2 getDirection();
    
    
    float maxThrust;
    float thrust;
    float thrustBeforeTouch;
    float acceleration;
    float dOmega;
    float mass;
    
    cocos2d::Sprite *sprite;
    cocos2d::ParticleFire *fire;
    cocos2d::Vec2 velocity;
    cocos2d::Vec2 gravAcceleration;
    cocos2d::Vec2 position;
    cocos2d::DrawNode *dot;
    cocos2d::DrawNode *dotLine;
    Orbit *orbit;
    CelestialBody *centralBody;
    
    Tank *tank;
};

#endif /* defined(__ProjectN__SpaceShip__) */
