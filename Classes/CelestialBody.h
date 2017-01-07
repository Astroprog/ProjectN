//
//  CelestialBody.h
//  ProjectN
//
//  Created by Peter Rodenkirch on 23.08.15.
//
//

#ifndef __ProjectN__CelestialBody__
#define __ProjectN__CelestialBody__

#include "cocos2d.h"

class CelestialBody : public cocos2d::Ref {

    
public:
    CelestialBody();
    CelestialBody(cocos2d::Sprite *initSprite);
    void updateVelocity(float dt);
    void updatePosition(float dt);
    
    cocos2d::Vec2 position;
    cocos2d::Vec2 velocity;
    cocos2d::Vec2 acceleration;
    cocos2d::DrawNode *orbit;
    cocos2d::DrawNode *dot;
    const char *title;
    float mass;
    float size;
    float apoapsis;
    cocos2d::Sprite *sprite;
};

#endif /* defined(__ProjectN__CelestialBody__) */
