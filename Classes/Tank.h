//
//  Tank.h
//  ProjectN
//
//  Created by Maximilian Scheurer on 10.09.15.
//
//

#ifndef __ProjectN__Tank__
#define __ProjectN__Tank__

#include "cocos2d.h"

class Tank : public cocos2d::Ref {
public: 
    bool initWithCapacity(float cap);
    float updateFuel(float thrust,float dt, float totalMass);
    void fillTank();
    float fuelMass;
    float _capacity;
    
    const float fuelScaleFactor = 500;
    float maximalCapacity = 50.f;
    
    cocos2d::Sprite *sprite;
    cocos2d::Sprite *arrowSprite;
};


#endif /* defined(__ProjectN__Tank__) */
