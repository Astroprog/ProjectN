//
//  Orbit.h
//  ProjectN
//
//  Created by Peter Rodenkirch on 02.09.15.
//
//

#ifndef __ProjectN__Orbit__
#define __ProjectN__Orbit__

#include "cocos2d.h"

class Orbit : public cocos2d::Ref {
    
    
public:
    Orbit();
    void computeOrbitalParameters();
    void updatePosition(float delta);
    float eccentricAnomalyWithTime(float t);
    float meanAnomalyWithTime(float t);
    float eccentricAnomalyWithMeanAnomaly(float M);
    inline float angleVec(cocos2d::Vec2 vec1, cocos2d::Vec2 vec2);
    
    float radius;
    float velocity;
    float centralMass;
    float energy;
    float semiMajorAxis;
    float semiMinorAxis;
    float eccentricity;
    float trueAnomaly;
    float eccentricAnomaly;
    float meanAnomaly;
    float periapsisDistance;
    float apoapsisDistance;
    float inclinaton;
    float semiLatusRectum;
    float orbitPeriod;
    float timeOfFlight;
    
    cocos2d::DrawNode *orbitalEllipse;
    cocos2d::Vec2 radiusVec;
    cocos2d::Vec2 velocityVec;
    cocos2d::Vec2 eccentricityVec;
    cocos2d::Vec2 center;
    
    
};

#endif /* defined(__ProjectN__Orbit__) */
