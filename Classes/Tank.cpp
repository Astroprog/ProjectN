//
//  Tank.cpp
//  ProjectN
//
//  Created by Maximilian Scheurer on 10.09.15.
//
//

#include "Tank.h"

bool Tank::initWithCapacity(float cap) {
    maximalCapacity = cap;
    _capacity = cap;
    fuelMass = _capacity;
    sprite = cocos2d::Sprite::create("gauge.png");
    arrowSprite = cocos2d::Sprite::create("arrow.png");
    return true;
}

float Tank::updateFuel(float thrust,float dt, float totalMass) {
    float deltaFuel = powf(thrust, 2) * powf(dt, 2);
    deltaFuel = deltaFuel / fuelScaleFactor;
    printf("total Mass: %f \n", totalMass);
    printf("dFuel: %f", deltaFuel);
    _capacity -= deltaFuel;
    if (_capacity > 0) {
        fuelMass = _capacity;
    }
    else {
        fillTank();
    }
    return totalMass;
}

void Tank::fillTank() {
    _capacity = maximalCapacity;
    fuelMass = _capacity;
}
